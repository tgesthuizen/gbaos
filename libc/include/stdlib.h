#ifndef LIBC_STDLIB_H
#define LIBC_STDLIB_H

void abort(void);
void exit(int code);
void _Exit(int code);
int atexit(void (*func)(void));

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#endif
