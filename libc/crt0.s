	.cpu arm7tdmi
	.thumb

	.section ".text", "ax"
	.global _start
	.type _start, %function
	.func
_start:
	ldr r0, =__bss_start__
	ldr r2, =__bss_end__
	sub r2, r2, r0
	mov r1, #0
	ldr r7, =memset
	bl  .Lbxpoline
	mov r0, #0
	mov r1, #0
	mov r2, #0
	ldr r7, =main
	bl  .Lbxpoline
	mov r7, #0
	mov r0, #0
	swi 28     @ exit(0)

.Lbxpoline:
	bx r7

	.endfunc
