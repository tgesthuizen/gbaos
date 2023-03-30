#ifndef KERN_SYS_SYSCALL_H
#define KERN_SYS_SYSCALL_H

enum syscall_num {
  nr_read,
  nr_write,
  nr_open,
  nr_close,
  nr_exit,
};

#define SYSCALL_SWI 28

static unsigned int raw_syscall(int nr, unsigned int arg1, unsigned int arg2,
                                unsigned int arg3, unsigned int arg4) {
  register unsigned int asm_nr asm("r7");
  register unsigned int asm_res asm("r0");
  register unsigned int asm_arg2 asm("r1");
  register unsigned int asm_arg3 asm("r2");
  register unsigned int asm_arg4 asm("r3");

  asm volatile("swi " SYSCALL_SWI "\n"
               : "=r"(asm_res)
               : "0"(arg1), "r"(arg2), "r"(arg3), "r"(arg4));
  return asm_res;
}

#endif
