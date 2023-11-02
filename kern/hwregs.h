#ifndef KERN_HWREGS_H
#define KERN_HWREGS_H

#define IE (*(volatile unsigned short *)0x4000200)
#define IF (*(volatile unsigned short *)0x4000202)
#define IRQ_HANDLER_ADDR (*(void *volatile *)0x3007FFC)
#define IME (*(volatile unsigned *)0x4000208)
#define DIPSTAT (*(volatile unsigned short *)0x4000004)

__attribute__((target("arm"), noinline)) unsigned int get_cpsr();
__attribute__((target("arm"), noinline)) void
set_cpsr(unsigned int cpsr);

#endif
