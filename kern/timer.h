#ifndef KERN_TIMER_H
#define KERN_TIMER_H

#include <stdint.h>

struct timer_control_t {
  uint16_t reload;
  uint16_t control;
};

enum timer_prescale_selection {
  timer_prescale_1,
  timer_prescale_64,
  timer_prescale_256,
  timer_prescale_1024,
};

#define TIMER_CONTROL_PRESCALE 3
#define TIMER_CONTROL_COUNT_UP_TIMING (1 << 2)
#define TIMER_CONTROL_ENABLE_IRQ (1 << 6)
#define TIMER_CONTROL_START (1 << 7)

#define TIMER_BASE ((volatile struct timer_control_t *)0x4000100)

#endif
