#include "display.h"
#include "string.h"
#include "tinyprintf.h"

#include <stdarg.h>
#include <stddef.h>

static int display_x = 0;
static int display_y = 0;
// Cheat a little for performance and RAM usage
static const
#include "font8x8_basic.h"
    ;

void putc(int ch) {
  if (ch >= 128) {
    return; // TODO: Revisit when we have some sort of assert
  }

  if (ch == '\n') {
    ++display_y;
    display_x = 0;
    return;
  }

  // This is not a special case, actually draw a character
  if (display_x == TERM_WIDTH) {
    ++display_y;
    display_x = 0;
  }

  if (display_y == TERM_HEIGHT) {
    memmove(video_mem, video_mem + SCREEN_WIDTH * 8,
            sizeof *video_mem * SCREEN_WIDTH * (SCREEN_HEIGHT - 8));
    --display_y;
  }

  for (int y = 0; y < 8; ++y) {
    for (int x = 0; x < 8; ++x) {
      const int idx = (display_y * 8 + y) * TERM_WIDTH * 8 + display_x * 8 + x;
      video_mem[idx] =
          font8x8_basic[ch][y] & (1 << x) ? COLOR_BLACK : COLOR_WHITE;
    }
  }
  ++display_x;
}

static void putc_helper(void *arg, char ch) {
  (void)arg;
  putc(ch);
}

int printf(char *fmt, ...) {
  va_list list;
  va_start(list, fmt);
  tfp_format(NULL, putc_helper, fmt, list);
  return 0;
}
