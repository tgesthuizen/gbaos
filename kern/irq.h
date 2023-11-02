#ifndef KERN_IRQ_H
#define KERN_IRQ_H

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

typedef void (*irq_handler)();

void init_irq();
void register_irq(enum irq_kind kind, irq_handler handler);

#endif
