#include "panic.h"
#include "display.h"
#include "hwregs.h"

void panic(const char *file, int line, const char *msg) {
  IME = 0;
  printf("PANIC: @%s:%d: %s\n", file, line, msg);
  while (1)
    __asm volatile("swi 02");
}
