OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(start_vector)
MEMORY {
  rom   : ORIGIN = 0x08000000, LENGTH = 32M
  iwram : ORIGIN = 0x03000000, LENGTH = 32K
  ewram : ORIGIN = 0x02000000, LENGTH = 256K
}
SECTIONS
{
  .rom_header :
  {
    KEEP (*(.rom_header))
  } >rom =0xff

  .text ALIGN(4) :
  {
    *(.text.startup)
    *(.text)
    *(.text.*)
  } >rom = 0xff

  .rodata :
  {
     *(.rodata)
     *all.rodata*(*)
     *(.roda)
     *(.rodata.*)
     *(.gnu.linkonce.r*)
     SORT(CONSTRUCTORS)
  } >rom = 0xff

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >rom
  __exidx_start = .;
  .ARM.exidx   : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } >rom
  __exidx_end = .;

  .romfs ALIGN(512):
  {
	*(.romfs)
  } >rom

  .data :
  {
    __data_start = ABSOLUTE(.);
    *(.data*)
    *(.gnu.linkonce.d*)
    CONSTRUCTORS
    . = ALIGN(4);
    __data_end = ABSOLUTE(.);
  } >ewram AT>rom = 0xff
  __data_lma = LOADADDR(.data);

  .bss (NOLOAD) :
  {
    __bss_start = ABSOLUTE(.);
    *(.dynbss)
    *(.gnu.linkonce.b*)
    *(.bss*)
    *(COMMON)
    __bss_end = ABSOLUTE(.);
  } >ewram
  .sbss (NOLOAD):
  {
    __sbss_start = ABSOLUTE(.);
    *(.sbss*)
    __sbss_end  = ABSOLUTE(.);
  } >ewram

  .stacks (NOLOAD):
  {
    . = ALIGN(16);
    . = . + 0x100;
    __sp_irq = .;
    . = . + 0x100;
    __sp_sys = .;
  } >iwram

  /* Stabs debugging sections.  */
  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }
  /*      DWARF debug sections.
          Symbols in the DWARF debugging sections are relative to the beginning
          of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  .stack 0x80000 : { _stack = .; *(.stack) }
  /* These must appear regardless of  .  */
  .note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
  .ARM.attributes 0 : { KEEP (*(.ARM.attributes)) }
  /DISCARD/ : { *(.note.GNU-stack) }
}
