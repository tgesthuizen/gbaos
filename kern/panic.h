#ifndef KASSERT_H
#define KASSERT_H

#define assert(x)                                                              \
  ((x) ? (void)0 : panic(__FILE__, __LINE__, "Assertion failed: " #x))

void __attribute__((noreturn))
panic(const char *file, int line, const char *msg);

#endif
