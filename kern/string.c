#include "string.h"

/**
 * The code below is... heavily inspired by newlib's implementation of the
 * respective functions.
 * It's not the same, but let's give credit where credit is due.
 */

void *memcpy(void *restrict dest_void, const void *restrict src_void,
             size_t len) {
  char *dst = (char *)dest_void;
  char *src = (char *)src_void;

  void *const save = dest_void;

  while (len--) {
    *dst++ = *src++;
  }

  return save;
}

void *memmove(void *dest_void, const void *src_void, size_t len) {
  char *dst = dest_void;
  const char *src = src_void;

  if (src < dst && dst < src + len) {
    /* Have to copy backwards */
    src += len;
    dst += len;
    while (len--) {
      *--dst = *--src;
    }
  } else {
    while (len--) {
      *dst++ = *src++;
    }
  }

  return dest_void;
}

extern void memmove32(void *dst_void, const void *src_void, size_t count);
extern void vmemmove32(volatile void *dst_void, const volatile void *src_void,
                       size_t count);
