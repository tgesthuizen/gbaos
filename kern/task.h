#ifndef KERN_TASK_H
#define KERN_TASK_H

#include "hwregs.h"
#include "irq.h"
#include "types.h"

struct register_set {
  u32 r[16];
  u32 psr;
};

#define TASK_MAX_NAME_LEN 8

struct task_state {
  struct register_set regs;
  struct task_state *next;
  char name[TASK_MAX_NAME_LEN];
};

void init_task_system();
void add_task(struct task_state *state);


void init_task_state(struct task_state *state, char *name, void (*entry)(void),
                     void *sp);
struct task_state *get_current_task();
void switch_task_from_irq(struct task_state *state, struct irq_info *info);
void start_multitasking();

#endif
