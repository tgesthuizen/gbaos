#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#include <stddef.h>

void *memcpy(void *restrict dst_void, const void *restrict src_void,
             size_t length);
void *memmove(void *dst_void, const void *src_void, size_t length);

#endif
