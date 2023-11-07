#ifndef KERN_IRQ_H
#define KERN_IRQ_H

#include <stdint.h>

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
      uint32_t r4;
      uint32_t r5;
      uint32_t r6;
      uint32_t r7;
      uint32_t r8;
      uint32_t r9;
      uint32_t r10;
      uint32_t r11;
      uint32_t r0;
      uint32_t r1;
      uint32_t r2;
      uint32_t r3;
      uint32_t r12;
      uint32_t r14;
    };
    uint32_t regs[14];
  };
};

typedef void (*irq_handler)(struct irq_info *info);

void init_irq();
void register_irq(enum irq_kind kind, irq_handler handler);

#endif
