#ifndef KERN_HWREGS_H
#define KERN_HWREGS_H

#define IE (*(volatile unsigned short *)0x4000200)
#define IF (*(volatile unsigned short *)0x4000202)
#define IRQ_HANDLER_ADDR (*(void *volatile *)0x3007FFC)
#define IME (*(volatile unsigned *)0x4000208)
#define DIPSTAT (*(volatile unsigned short *)0x4000004)

__attribute__((target("arm"), noinline)) unsigned int get_cpsr();
__attribute__((target("arm"), noinline)) void set_cpsr(unsigned int cpsr);

enum cpu_modes {
  cpu_mode_user = 0x10,
  cpu_mode_fiq = 0x11,
  cpu_mode_irq = 0x12,
  cpu_mode_supervisor = 0x13,
  cpu_mode_abort = 0x17,
  cpu_mode_undefined = 0x1B,
  cpu_mode_system = 0x1F,
};

#define PSR_MODE_MASK (0b11111)
#define PSR_FIQ_DISABLED (1 << 6)
#define PSR_IRQ_DISABLED (1 << 7)
#define PSR_ABORT_DISABLED (1 << 8)
#define PSR_ENDIAN (1 << 9)

#endif
