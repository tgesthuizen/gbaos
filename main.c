#include "display.h"

static volatile int irqs_handled;

__attribute__((isr("IRQ"))) static void handle_irq() {
  putc('x');
  ++irqs_handled;
}

static unsigned int get_cpsr() {
  unsigned int res;
  asm volatile("mrs %0, cpsr\n" : "=r"(res));
  return res;
}

static void set_cpsr(unsigned int cpsr) {
  asm volatile("msr cpsr, %0\n" : : "r"(cpsr));
}

#define IE (*(volatile unsigned short *)0x4000200)
#define IF (*(volatile unsigned short *)0x4000202)
#define IRQ_HANDLER_ADDR (*(void *volatile *)0x3007FFC)
#define IME (*(volatile unsigned *)0x4000208)
#define DIPSTAT (*(volatile unsigned short *)0x4000004)

int main(int argc, char **argv) {
  IE = 1 << 0 | 1 << 12;
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
