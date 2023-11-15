#include "task.h"
#include "hwregs.h"
#include "irq.h"
#include "panic.h"
#include "string.h"
#include "timer.h"
#include "types.h"

static void idle_task() { __asm volatile("swi 02\n"); }

#define IDLE_STACK_SIZE 256
static unsigned char idle_stack[IDLE_STACK_SIZE];
static struct task_state idle_state;
static struct task_state *current_task;

static void reset_task_timer() {
  struct timer_control_t tctrl;
  tctrl.reload = 32;
  tctrl.control =
      timer_prescale_64 | TIMER_CONTROL_ENABLE_IRQ | TIMER_CONTROL_START;
  TIMER_BASE[0] = tctrl;
}

static void handle_timer_overflow(struct irq_info *info) {
  switch_task_from_irq(current_task->next, info);
  reset_task_timer();
}

void init_task_system() {
  static const char idle_name[] = "idle";
  memcpy(idle_state.name, idle_name, sizeof(idle_name));
  idle_state.regs.r[13] = (uint32_t)(idle_stack + IDLE_STACK_SIZE);
  idle_state.regs.r[15] = (uint32_t)(&idle_task);
  idle_state.regs.psr = cpu_mode_system | PSR_THUMB_MODE;
  idle_state.next = &idle_state;
  current_task = &idle_state;
  register_irq(irq_kind_timer_0_overflow, &handle_timer_overflow);
}

void start_multitasking() { reset_task_timer(); }

void add_task(struct task_state *new_task) {
  new_task->next = current_task->next;
  current_task->next = new_task;
}

void init_task_state(struct task_state *state, char *name, void (*entry)(void),
                     void *sp) {
  for (char *dst = state->name, *end = state->name + TASK_MAX_NAME_LEN;
       *name && dst != end;)
    *dst++ = *name++;

  u32 entry_addr = (u32)entry;
  // Enable Thumb mode if function is thumb
  state->regs.psr = cpu_mode_user | ((entry_addr & 1) * PSR_THUMB_MODE);
  for (int i = 0; i < 13; ++i)
    state->regs.r[i] = 0;
  state->regs.r[13] = (u32)sp;
  state->regs.r[14] = 0;
  state->regs.r[15] =
      entry_addr & (u32)~1; // Possibly remove Thumb bit from address
}

struct task_state *get_current_task() { return current_task; }

static const char register_at_index_in_irq_info[] = {4,  5, 6, 7, 8, 9,  10,
                                                     11, 0, 1, 2, 3, 12, 14};

void __attribute__((target("arm"), noinline))
store_user_context_in_current_task_from_irq(struct irq_info *info) {
  for (int i = 0; i < 14; ++i)
    current_task->regs.r[(int)register_at_index_in_irq_info[i]] = info->regs[i];
  current_task->regs.r[15] = info->r14 - 4;

  u32 captured_lr, captured_sp;
  __asm volatile(
      "msr cpsr, %c[cpu_mode_system]\n\t"
      "mov %[captured_lr], lr\n\t"
      "mov %[captured_sp], sp\n\t"
      "msr cpsr, %c[cpu_mode_irq]\n\t"
      : [captured_lr] "=r"(captured_lr), [captured_sp] "=r"(captured_sp)
      : [cpu_mode_system] "i"(cpu_mode_system), [cpu_mode_irq] "i"(
                                                    cpu_mode_irq));
  current_task->regs.r[13] = captured_sp;
  current_task->regs.r[14] = captured_lr;
  u32 captured_spsr;
  __asm volatile("mrs %[captured_spsr], spsr\n\t"
                 : [captured_spsr] "=r"(captured_spsr));
  current_task->regs.psr = captured_spsr;
}

void __attribute__((target("arm"), noinline))
switch_user_context_to_current_task_from_irq(struct irq_info *info) {
  for (int i = 0; i < 14; ++i) {
    info->regs[i] = current_task->regs.r[(int)register_at_index_in_irq_info[i]];
  }
  info->r14 = current_task->regs.r[15] + 4;

  __asm volatile("msr cpsr, %c[cpu_mode_system]\n\t"
                 "movs lr, %[task_lr]\n\t"
                 "movs sp, %[task_sp]\n\t"
                 "msr cpsr, %c[cpu_mode_irq]\n\t"
                 :
                 : [task_lr] "r"(current_task->regs.r[14]),
                   [task_sp] "r"(current_task->regs.r[13]),
                   [cpu_mode_system] "i"(cpu_mode_system),
                   [cpu_mode_irq] "i"(cpu_mode_irq));

  __asm volatile("msr spsr, %[task_psr]"
                 :
                 : [task_psr] "r"(current_task->regs.psr));
}

void switch_task_from_irq(struct task_state *task, struct irq_info *info) {
  store_user_context_in_current_task_from_irq(info);
  current_task = task;
  switch_user_context_to_current_task_from_irq(info);
}
