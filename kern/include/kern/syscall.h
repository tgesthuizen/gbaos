#ifndef KERN_SYS_SYSCALL_H
#define KERN_SYS_SYSCALL_H

enum syscall_num {
  SYS_READ,
  SYS_WRITE,
  SYS_OPEN,
  SYS_CLOSE,
  SYS_EXIT,
};

#define SYSCALL_SWI 28
#define SYSCALL_SWI_STR #SYSCALL_SWI

static unsigned int raw_syscall(int nr, unsigned int arg1, unsigned int arg2,
                                unsigned int arg3, unsigned int arg4) {
  register unsigned int asm_nr asm("r7") = nr;
  register unsigned int asm_res asm("r0");
  register unsigned int asm_arg2 asm("r1") = arg2;
  register unsigned int asm_arg3 asm("r2") = arg3;
  register unsigned int asm_arg4 asm("r3") = arg4;

  asm volatile("swi 28\n"
               : "=r"(asm_res)
               : "0"(arg1), "r"(asm_arg2), "r"(asm_arg3), "r"(asm_arg4),
                 "r"(asm_nr));
  return asm_res;
}

#endif
