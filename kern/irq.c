#include "irq.h"
#include "hwregs.h"
#include "task.h"
#include <stdint.h>

static irq_handler irq_table[irq_kind_max];
uint32_t regs_on_irq_entry[16];

static __attribute__((target("arm"))) void handle_irq() {
  const unsigned short if_copy = IF;
  const unsigned short ie_copy = IE;
  for (int i = 0; i < irq_kind_max; ++i) {
    const int mask = 1 << i;
    if (if_copy & mask && ie_copy & mask) {
      IF = mask;
      const irq_handler h = irq_table[i];
      if (h)
        return h();
    }
  }
}

// Saves registers, calls handle_irq and returns from the interrupt
static __attribute__((target("arm"), naked)) void irq_entry() {
  __asm volatile(
      "ldr r0, %[regs_on_entry]\n\t"
      "adds r0, %[r4_offset]\n\t"
      "stmfa r0!, {r4-r11}\n\t"
      "ldmfd sp!, {r1, r2}\n\t" // r12, r14
      "stmfa r0!, {r1, r2}\n\t"
      "ldmfd sp!, {r1-r5}\n\t"
      "ldr r0, %[regs_on_entry]\n\t"
      "stmfa r0!, {r1-r5}\n\t"
      "bl %c[handle_irq]\n\t"
      "ldr r0, %[regs_on_entry]\n\t"
      "ldmfa r0!, {r0-r11,r12,r14}\n\t"
      "subs r15, r14, #4\n" ::[regs_on_entry] "m"(regs_on_irq_entry),
      [handle_irq] "i"(handle_irq), [r4_offset] "i"(sizeof(uint32_t) * 4));
}

void init_irq() { IRQ_HANDLER_ADDR = &irq_entry; }

void register_irq(enum irq_kind kind, irq_handler handler) {
  irq_table[kind] = handler;
  IE |= 1 << kind;
}
