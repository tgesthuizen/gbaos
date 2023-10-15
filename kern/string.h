#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#include <stddef.h>

void *memcpy(void *restrict dst_void, const void *restrict src_void,
             size_t length);
void *memmove(void *dst_void, const void *src_void, size_t length);
inline void memmove32(void *dst_void, const void *src_void, size_t count) {
  unsigned *dst = dst_void;
  const unsigned *src = src_void;
  while (count--)
    *dst++ = *src++;
}
inline void vmemmove32(volatile void *dst_void, const volatile void *src_void,
                       size_t count) {
  volatile unsigned *dst = dst_void;
  const volatile unsigned *src = src_void;
  while (count--)
    *dst++ = *src++;
}

#endif
