#include "display.h"

static void handle_irq() { putc('x'); }

int main(int argc, char **argv) {
  video_mode_reg = 0x403;
  printf("Hello, World!\n");
  *(volatile unsigned short *)0x4000200 = 1;
  *(volatile unsigned short *)0x4000202 = *(volatile unsigned short *)0x4000202;
  *(void *volatile *)0x3007FFC = &handle_irq;
  *(volatile unsigned *)4000208 = 1;
  return 0;
}
