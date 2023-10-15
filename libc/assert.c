#include <assert.h>
#include <stdlib.h>

void __aeabi_assert(const char *expr, const char *file, int line) {
  (void)expr;
  (void)file;
  (void)line;
  // TODO
  abort();
}
