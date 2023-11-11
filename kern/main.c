#include "display.h"
#include "hwregs.h"
#include "irq.h"
#include "romfs.h"
#include "task.h"

static volatile int irqs_handled;
static void handle_vblank(struct irq_info *info) {
  (void)info;
  ++irqs_handled;
}
static void enable_interrupts() { IME = 1; }

static void init_task() {
  printf("Hello, World!\n");
  printf("CPSR: %x\n", get_cpsr());
  printf("IF: %x\n", IF);
  u32 cur;
  romfs_get_root(&cur);
  do {
    printf("found: %s\n", romfs_file_name(cur));
  } while (romfs_read_next(&cur, cur) == 0);
  int old_irqs_handled = irqs_handled;
  while (1) {
    if (old_irqs_handled != irqs_handled) {
      old_irqs_handled = irqs_handled;
      // printf("%d\n", old_irqs_handled);
    }
  }
}

#define INIT_TASK_STACK_SIZE 512
static unsigned char init_task_stack[INIT_TASK_STACK_SIZE];
static struct task_state the_init_task;

int main() {
  init_irq();
  register_irq(irq_kind_lcd_vblank, handle_vblank);
  DIPSTAT = 0b11000;
  video_mode_reg = 0x403;
  enable_interrupts();
  init_task_system();
  init_task_state(&the_init_task, init_task,
                  init_task_stack + INIT_TASK_STACK_SIZE);
  add_task(&the_init_task);
  start_multitasking();
  return 0;
}
