#include "display.h"

#define IE (*(volatile unsigned short *)0x4000200)
#define IF (*(volatile unsigned short *)0x4000202)
#define IRQ_HANDLER_ADDR (*(void *volatile *)0x3007FFC)
#define IME (*(volatile unsigned *)0x4000208)
#define DIPSTAT (*(volatile unsigned short *)0x4000004)

enum irq_kind {
  irq_kind_lcd_vblank,
  irq_kind_lcd_hblank,
  irq_kind_lcd_vcounter_match,
  irq_kind_timer_0_overflow,
  irq_kind_timer_1_overflow,
  irq_kind_timer_2_overflow,
  irq_kind_timer_3_overflow,
  irq_kind_serial_communication,
  irq_kind_dma_0,
  irq_kind_dma_1,
  irq_kind_dma_2,
  irq_kind_dma_3,
  irq_kind_keypad,
  irq_kind_gamepak,

  irq_kind_max,
};

typedef void (*irq_handler)();

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

static volatile int irqs_handled;
static void handle_vblank() { ++irqs_handled; }

static __attribute__((target("arm"), noinline)) unsigned int get_cpsr() {
  unsigned int res;
  asm volatile("mrs %0, cpsr\n" : "=r"(res));
  return res;
}

static __attribute__((target("arm"), noinline)) void
set_cpsr(unsigned int cpsr) {
  asm volatile("msr cpsr, %0\n" : : "r"(cpsr));
}

int main() {
  IE = 1 << irq_kind_lcd_vblank;
  irq_table[irq_kind_lcd_vblank] = handle_vblank;
  IF = *(volatile unsigned short *)0x4000202;
  IRQ_HANDLER_ADDR = &handle_irq;
  DIPSTAT = 0b11000;
  IME = 1;

  video_mode_reg = 0x403;
  printf("Hello, World!\n");
  printf("CPSR: %x\n", get_cpsr());
  printf("IF: %x\n", *(volatile unsigned short *)0x4000202);
  int old_irqs_handled = irqs_handled;
  while (1) {
    if (old_irqs_handled != irqs_handled) {
      old_irqs_handled = irqs_handled;
      printf("%d\n", old_irqs_handled);
    }
  }
  return 0;
}
