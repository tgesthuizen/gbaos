#include <errno.h>

static int __the_errno;
volatile int *__aeabi_errno_addr() { return &__the_errno; }
