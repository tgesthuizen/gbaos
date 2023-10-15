#include <kern/syscall.h>
#include <stdlib.h>

void abort(void) {
  raw_syscall(SYS_EXIT, 127, 0, 0, 0);
  __builtin_unreachable();
}
