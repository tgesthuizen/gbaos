.cpu arm7tdmi
.arm

.section ".rom_header","ax"
.global rom_header, __boot_method, __slave_number
.type rom_header, %object
rom_header:
  b       rom_header_end

  .fill   156,1,0      @ Nintendo Logo Character Data (8000004h)
  .fill   16,1,0       @ Game Title
  .byte   0x30,0x31    @ Maker Code (80000B0h)
  .byte   0x96         @ Fixed Value (80000B2h)
  .byte   0x00         @ Main Unit Code (80000B3h)
  .byte   0x00         @ Device Type (80000B4h)
  .fill   7,1,0        @ unused
  .byte   0x00         @ Software Version No (80000BCh)
  .byte   0xf0         @ Complement Check (80000BDh)
  .byte   0x00,0x00    @ Checksum (80000BEh)

rom_header_end:
  b       start_vector @ This branch must be here for proper
                       @ positioning of the following header.
__boot_method:
  .byte   0            @ boot method (0=ROM boot, 3=Multiplay boot)
__slave_number:
  .byte   0            @ slave # (1=slave#1, 2=slave#2, 3=slave#3)

  .byte   0            @ reserved
  .byte   0            @ reserved
  .word   0            @ reserved
  .word   0            @ reserved
  .word   0            @ reserved
  .word   0            @ reserved
  .word   0            @ reserved
  .word   0            @ reserved

.size rom_header, .-rom_header

.section ".text.startup", "ax"
.global start_vector
.type start_vector, %function
.func start_vector, start_vector
start_vector:
  mov r0, #0x4000000   @ REG_BASE
  str r0, [r0, #0x208] @ IME = 0, disable interrupts

  mov r0, #0x12        @ Switch to IRQ Mode
  msr cpsr, r0
  ldr sp, =__sp_irq    @ Set IRQ stack
  mov r0, #0x1f        @ Switch to System Mode
  msr cpsr, r0
  ldr sp, =__sp_sys    @ Set system stack

	add r0, pc, #1
	bx  r0
.thumb
thumb_entry:
	@ Copy .data to EWRAM
	ldr r0, =__data_lma
	ldr r1, =__data_start
	ldr r2, =__data_end
	bl  __boot_memcpy

	@ Zero .bss section
	ldr r0, =__bss_start
	ldr r1, =__bss_end
	mov r2, #0
	bl  __boot_memset

	@Zero .sbss section
	ldr r0, =__sbss_start
	ldr r1, =__sbss_end
	mov r2, #0
	bl  __boot_memset

	mov r0, #0 @ argc
	mov r1, #0 @ argv
	mov r2, #0 @ env
	ldr r7, =main
	bl  _bx_trampoline
.L1:
	b   .L1
_bx_trampoline:
	bx  r7
.pool
.endfunc
.size start_vector, .-start_vector

.type __boot_memcpy, %function
.func __boot_memcpy, __boot_memcpy
__boot_memcpy:
	sub   r2, r2, r1
	tst   r2, r2
	beq   .L3
.L2:
	ldmia r0!, {r3}
	stmia r1!, {r3}
	sub   r2, #4
	bne   .L2
.L3:
	bx    lr
.endfunc
.size __boot_memcpy, .-__boot_memcpy

.type __boot_memset, %function
.func __boot_memset, __boot_memset
__boot_memset:
	sub   r1, r1, r0
	tst   r1, r1
	beq   .L5
.L4:
	stmia r0!, {r2}
	sub   r1, #4
	bne   .L4
.L5:
	bx    lr
.size __boot_memset, .-__boot_memset
