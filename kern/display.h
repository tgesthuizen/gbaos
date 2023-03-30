#ifndef KERNEL_DISPLAY_H
#define KERNEL_DISPLAY_H

#include "types.h"

enum term_dims { TERM_WIDTH = 30, TERM_HEIGHT = 20 };

void putc(int ch);
int printf(char *fmt, ...);

enum screen_dims { SCREEN_WIDTH = 240, SCREEN_HEIGHT = 160 };

#define MEM_IO 0x04000000
#define MEM_VRAM 0x06000000

#define video_mode_reg (*(volatile u32 *)MEM_IO)
#define video_mem ((volatile u16 *)MEM_VRAM)

enum display_colors {
  COLOR_BLACK = 0x0000,
  COLOR_RED = 0x001F,
  COLOR_LIME = 0x03E0,
  COLOR_YELLOW = 0x03FF,
  COLOR_BLUE = 0x7C00,
  COLOR_MAG = 0x7C1F,
  COLOR_CYAN = 0x7FE0,
  COLOR_WHITE = 0x7FFF,
};

#endif
