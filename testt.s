	.text
	.comm	a_g,40,4
	.align	1
	.global	cal
	.type	cal, @function
cal:
	addi sp, sp, -36
	sw ra, 32(sp)
	sw s0, 28(sp)
	addi s0, sp, 36
	lw t0, 4(s0)
	li t0, 1
	lw t1, 0(s0)
	lw t2, -20(s0)
	mv t2, t1
	sw a0, -24(s0)
	lw a0, -28(s0)
	mv a0, t0
	sw a1, -32(s0)
	lw a1, -36(s0)
	mv a1, t2
	add a1, a1, a0
	sw a0, -28(s0)
	mv a0, a1
	lw s0, 28(sp)
	lw ra, 32(sp)
	addi sp, sp, 36
	jr ra
	sw a0, -36(s0)
	li a0, 0
	lw s0, 28(sp)
	lw ra, 32(sp)
	addi sp, sp, 36
	jr ra
	.size	cal, .-cal
	.align	1
	.global	cal2
	.type	cal2, @function
cal2:
	addi sp, sp, -44
	sw ra, 40(sp)
	sw s0, 36(sp)
	addi s0, sp, 44
	lw t0, 0(s0)
	lw t1, -20(s0)
	mv t1, t0
	lw t2, 4(s0)
	sw t0, 0(s0)
	lw t0, -24(s0)
	mv t0, t2
	sw t1, -20(s0)
	lw t1, 8(s0)
	sw a0, -28(s0)
	lw a0, -32(s0)
	mv a0, t1
	sw a1, -36(s0)
	lw a1, -20(s0)
	sw a2, -40(s0)
	lw a2, -44(s0)
	mv a2, a1
	add a2, a2, t0
	add a2, a2, a0
	sw a0, -32(s0)
	mv a0, a2
	lw s0, 36(sp)
	lw ra, 40(sp)
	addi sp, sp, 44
	jr ra
	sw a0, -44(s0)
	li a0, 0
	lw s0, 36(sp)
	lw ra, 40(sp)
	addi sp, sp, 44
	jr ra
	.size	cal2, .-cal2
	.align	1
	.global	main
	.type	main, @function
main:
	addi sp, sp, -60
	sw ra, 56(sp)
	sw s0, 52(sp)
	addi s0, sp, 60
	call global
	lw t0, -20(s0)
	li t0, 1
	lw t1, -24(s0)
	li t1, 2
	lw t2, -28(s0)
	li t2, 3
	lw a0, -32(s0)
	li a0, 4
	lw a1, -36(s0)
	li a1, 5
	lw a2, -40(s0)
	li a2, 6
	lw a3, -44(s0)
	li a3, 7
	lw a4, -48(s0)
	li a4, 8
	lw a5, -52(s0)
	li a5, 9
	lw a6, -56(s0)
	li a6, 10
	sw t0, -20(s0)
	sw t1, -24(s0)
	sw t2, -28(s0)
	sw a0, -32(s0)
	sw a1, -36(s0)
	sw a2, -40(s0)
	sw a3, -44(s0)
	sw a4, -48(s0)
	sw a5, -52(s0)
	sw a6, -56(s0)
	lw a0, -20(s0)
	lw a1, -24(s0)
	lw a2, -28(s0)
	lw a3, -32(s0)
	lw a4, -36(s0)
	lw a5, -40(s0)
	lw a6, -44(s0)
	lw a7, -48(s0)
	lw t0, -52(s0)
	sw t0, 0(sp)
	sw t0, -52(s0)
	lw t0, -56(s0)
	sw t0, 4(sp)
	sw t0, -56(s0)
	call cal
	sw a0, -60(s0)
	lw a0, -60(s0)
	call putint
	li a0, 0
	lw s0, 52(sp)
	lw ra, 56(sp)
	addi sp, sp, 60
	jr ra
	li a0, 0
	lw s0, 52(sp)
	lw ra, 56(sp)
	addi sp, sp, 60
	jr ra
	.size	main, .-main
	.align	1
	.global	global
	.type	global, @function
global:
	addi sp, sp, -16
	sw ra, 12(sp)
	sw s0, 8(sp)
	addi s0, sp, 16
	lw s0, 8(sp)
	lw ra, 12(sp)
	addi sp, sp, 16
	jr ra
	.size	global, .-global
