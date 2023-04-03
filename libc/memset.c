#include <string.h>

void *memset(void *s, int c, size_t n) {
  unsigned char *s_uc = s;
  const unsigned char c_uc = c;
  for (size_t i = 0; i < n; ++i)
    *s_uc++ = c_uc;
  return s;
}
