#include "irq.h"
#include "hwregs.h"
#include "task.h"

static irq_handler irq_table[irq_kind_max];

static __attribute__((target("arm"))) void handle_irq(struct irq_info *info) {
  const unsigned short if_copy = IF;
  const unsigned short ie_copy = IE;
  for (int i = 0; i < irq_kind_max; ++i) {
    const int mask = 1 << i;
    if (if_copy & mask && ie_copy & mask) {
      IF = mask;
      const irq_handler h = irq_table[i];
      if (h)
        return h(info);
    }
  }
}

/**
 * Pushes remaining GP registers on the stack.
 * This way the pushed registers in combination with the registers the BIOS
 * saved combine to form the irq_info struct on the stack.
 * It is passed to handle_irq and restored afterwards to allow to switch
 * context.
 */
static __attribute__((target("arm"), naked)) void irq_entry() {
  __asm("stmfd sp!, {r4-r11}\n\t"
        "movs r0, sp\n\t"
        "stmfd sp!, {r12, lr}\n\t"
        "bl %c[handle_irq]\n\t"
        "ldr r0, [sp, #4]\n\t"
        "adds sp, #(10 * 4)\n\t"
        "bx r0\n"
        :
        : [handle_irq] "i"(handle_irq)
        : "r0", "memory", "cc");
}

void init_irq() { IRQ_HANDLER_ADDR = &irq_entry; }

void register_irq(enum irq_kind kind, irq_handler handler) {
  irq_table[kind] = handler;
  IE |= 1 << kind;
}
