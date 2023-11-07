#ifndef KERN_IRQ_H
#define KERN_IRQ_H

#include "types.h"

enum irq_kind {
  irq_kind_lcd_vblank,
  irq_kind_lcd_hblank,
  irq_kind_lcd_vcounter_match,
  irq_kind_timer_0_overflow,
  irq_kind_timer_1_overflow,
  irq_kind_timer_2_overflow,
  irq_kind_timer_3_overflow,
  irq_kind_serial_communication,
  irq_kind_dma_0,
  irq_kind_dma_1,
  irq_kind_dma_2,
  irq_kind_dma_3,
  irq_kind_keypad,
  irq_kind_gamepak,

  irq_kind_max,
};

struct irq_info {
  union {
    struct {
      u32 r4;
      u32 r5;
      u32 r6;
      u32 r7;
      u32 r8;
      u32 r9;
      u32 r10;
      u32 r11;
      u32 r0;
      u32 r1;
      u32 r2;
      u32 r3;
      u32 r12;
      u32 r14;
    };
    u32 regs[14];
  };
};

typedef void (*irq_handler)(struct irq_info *info);

void init_irq();
void register_irq(enum irq_kind kind, irq_handler handler);

#endif
