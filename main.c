#include "display.h"

static void handle_irq() {
  *(volatile unsigned short *)0x4000202 =
      *(volatile unsigned short *)0x4000202; // acknowledge all IRQs
  for (int i = 0; i < Width; ++i) {
    video_mem[i + 3 * Height] = COLOR_BLACK;
  }
}

int main(int argc, char **argv) {
  video_mode_reg = 0x403;
  for (int i = 0; i < Width * Height; ++i) {
    video_mem[i] = COLOR_RED;
  }

  *(volatile unsigned short *)0x4000200 = 1;
  *(volatile unsigned short *)0x4000202 = *(volatile unsigned short *)0x4000202;
  *(void *volatile *)0x3007FFC = &handle_irq;
  *(volatile unsigned *)4000208 = 1;
  return 0;
}
