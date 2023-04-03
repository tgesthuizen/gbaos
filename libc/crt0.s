	.cpu arm7tdmi
	.thumb

	.section ".text", "ax"
	.global _start
	.type _start, %function
	.func
_start:
	mov r1, #0
	mov lr, r1
	ldr r0, .Lbss_start_got
	add r0, r0, r9
	ldr r0, [r0]
	ldr r2, .Lbss_end_got
	add r2, r2, r9
	ldr r2, [r2]
	ldr r7, .Lmemset_got
	add r7, r7, r9
	bl  .Lbxpoline
	mov r0, #0
	mov r1, #0
	mov r2, #0
	ldr r7, .Lmain_got
	add r7, r7, r9
	bl  .Lbxpoline
	mov r7, #0
	mov r0, #0
	swi 28     @ exit(0)

	.align 4
.Lmemset_got:
	.word 0x0
	.reloc .Lmemset_got, R_ARM_GOT32, memset
.Lmain_got:
	.word 0x0
	.reloc .Lmain_got, R_ARM_GOT32, main
.Lbss_start_got:
	.word 0x0
	.reloc .Lbss_start_got, R_ARM_GOT32, __bss_start__	
.Lbss_end_got:
	.word 0x0
	.reloc .Lbss_end_got, R_ARM_GOT32, __bss_end__

.Lbxpoline:
	bx r7

	.endfunc
