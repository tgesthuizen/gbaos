#include "display.h"
#include "irq.h"
#include "romfs.h"
#include "hwregs.h"

static volatile int irqs_handled;
static void handle_vblank() { ++irqs_handled; }


static void enable_interrupts() { IME = 1; }

int main() {
  init_irq();
  register_irq(irq_kind_lcd_vblank, handle_vblank);
  DIPSTAT = 0b11000;
  enable_interrupts();

  video_mode_reg = 0x403;
  printf("Hello, World!\n");
  printf("CPSR: %x\n", get_cpsr());
  printf("IF: %x\n", *(volatile unsigned short *)0x4000202);
  uint32_t cur;
  romfs_get_root(&cur);
  do {
    printf("found: %s\n", romfs_file_name(cur));
  } while (romfs_read_next(&cur, cur) == 0);
  int old_irqs_handled = irqs_handled;
  while (1) {
    if (old_irqs_handled != irqs_handled) {
      old_irqs_handled = irqs_handled;
      // printf("%d\n", old_irqs_handled);
    }
  }
  return 0;
}
