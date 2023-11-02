#include "irq.h"
#include "hwregs.h"

static irq_handler irq_table[irq_kind_max];

static __attribute__((target("arm"))) void handle_irq() {
  const unsigned short if_copy = IF;
  const unsigned short ie_copy = IE;
  for (int i = 0; i < irq_kind_max; ++i) {
    const int mask = 1 << i;
    if (if_copy & mask && ie_copy & mask) {
      IF = mask;
      const irq_handler h = irq_table[i];
      if (h)
        h();
    }
  }
}

void init_irq() {
  IRQ_HANDLER_ADDR = &handle_irq;
}

void register_irq(enum irq_kind kind, irq_handler handler) {
  irq_table[kind] = handler;
  IE |= 1 << kind;
}
