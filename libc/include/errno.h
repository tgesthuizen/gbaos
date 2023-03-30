#ifndef LIBC_ERRNO_H
#define LIBC_ERRNO_H

#include <kern/errno.h>

volatile int *__aeabi_errno_addr();

#define errno (*__aeabi_errno_addr())

#endif
