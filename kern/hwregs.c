#include "hwregs.h"

__attribute__((target("arm"), noinline)) unsigned int get_cpsr() {
  unsigned int res;
  asm volatile("mrs %0, cpsr\n" : "=r"(res));
  return res;
}

__attribute__((target("arm"), noinline)) void
set_cpsr(unsigned int cpsr) {
  asm volatile("msr cpsr, %0\n" : : "r"(cpsr));
}
