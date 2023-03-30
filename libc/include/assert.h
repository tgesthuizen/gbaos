#ifndef LIBC_ASSERT_H
#define LIBC_ASSERT_H

void __aeabi_assert(const char *expr, const char *file, int line);
#define assert(__e) ((__e) ? (void)0 : __aeabi_assert(#__e, __FILE__, __LINE__))

#endif
