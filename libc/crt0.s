	.cpu arm7tdmi
	.thumb

	.section ".text", "ax"
	.align 1
	.global _start
	.type _start, %function
	.func
_start:
	mov r1, #0
	mov lr, r1
	mov r4, r9
	push {r1, r4, r5, r6, r7, lr}
	ldr r0, .Lbss_start_got
	ldr r0, [r0, r4]
	ldr r2, .Lbss_end_got
	ldr r2, [r2, r4]
	bl  memset
	ldr r0, .Lpreinit_array_start_got
	ldr r0, [r0, r4]
	ldr r1, .Lpreinit_array_end_got
	ldr r1, [r1, r4]
	bl  run_funcs
	ldr r0, .Linit_array_start_got
	ldr r0, [r0, r4]
	ldr r1, .Linit_array_end_got
	ldr r1, [r1, r4]
	bl  run_funcs
	mov r0, #0
	mov r1, #0
	mov r2, #0
	bl  main
	mov r5, r0
	ldr r0, .Lfini_array_start_got
	ldr r0, [r0, r4]
	ldr r1, .Lfini_array_end_got
	ldr r1, [r1, r4]
	bl  run_funcs
	mov r7, #0
	mov r0, r5
	swi 28     @ exit(0)

	.align 2
.Lbss_start_got:
	.word __bss_start__(GOT)
.Lbss_end_got:
	.word __bss_end__(GOT)
.Lpreinit_array_start_got:
	.word __preinit_array_start(GOT)
.Lpreinit_array_end_got:
	.word __preinit_array_end(GOT)
.Linit_array_start_got:
	.word __init_array_start(GOT)
.Linit_array_end_got:
	.word __init_array_end(GOT)
.Lfini_array_start_got:
	.word __fini_array_start(GOT)
.Lfini_array_end_got:
	.word __fini_array_end(GOT)

	.endfunc
	.size _start,.-_start

	.align 1
	.type run_funcs, %function
	.func
run_funcs:
	push {r4,r5,r6,lr}
	mov r4, r0
	sub r5, r1, r0
.Lnext_func:
	beq .Lfuncs_done
	ldr r6, [r4]
	add r4, r4, #4
	bl  .Lbxpoline
	sub r5, r5, #4
	b   .Lnext_func
.Lfuncs_done:
	pop {r0,r4,r5,pc}
.Lbxpoline:
	bx r6

	.endfunc
	.size run_funcs,.-run_funcs


