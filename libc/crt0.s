	.cpu arm7tdmi
	.thumb

	.section ".text", "ax"
	.global _start
	.type _start, %function
	.func
_start:
	mov r1, #0
	mov lr, r1
	mov r4, r9
	push {r0, r4, r7, lr}
	ldr r0, .Lbss_start_got
	ldr r0, [r0, r4]
	ldr r2, .Lbss_end_got
	ldr r2, [r2, r4]
	bl  memset
	mov r0, #0
	mov r1, #0
	mov r2, #0
	bl  main
	mov r7, #0
	mov r0, #0
	swi 28     @ exit(0)

	.align 2
.Lbss_start_got:
	.word __bss_start__(GOT)
.Lbss_end_got:
	.word __bss_end__(GOT)

	.endfunc
