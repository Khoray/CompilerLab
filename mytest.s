	.text
	.comm	a_g,4,4
	.comm	b_g,4,4
	.comm	d_g,4,4
	.align	1
	.global	set_a
	.type	set_a, @function
set_a:
	addi sp, sp, -24
	sw ra, 20(sp)
	sw s0, 16(sp)
	addi s0, sp, 24
	mv t0, a0
	mv t1, t0
	mv t0, t1
	sw t1, a_g, t6
	sw a0, -24(s0)
	mv a0, t0
	lw s0, 16(sp)
	lw ra, 20(sp)
	addi sp, sp, 24
	jr ra
	sw a0, -20(s0)
	li a0, 0
	lw s0, 16(sp)
	lw ra, 20(sp)
	addi sp, sp, 24
	jr ra
	.size	set_a, .-set_a
	.align	1
	.global	set_b
	.type	set_b, @function
set_b:
	addi sp, sp, -24
	sw ra, 20(sp)
	sw s0, 16(sp)
	addi s0, sp, 24
	mv t0, a0
	mv t1, t0
	mv t0, t1
	sw t1, b_g, t6
	sw a0, -24(s0)
	mv a0, t0
	lw s0, 16(sp)
	lw ra, 20(sp)
	addi sp, sp, 24
	jr ra
	sw a0, -20(s0)
	li a0, 0
	lw s0, 16(sp)
	lw ra, 20(sp)
	addi sp, sp, 24
	jr ra
	.size	set_b, .-set_b
	.align	1
	.global	set_d
	.type	set_d, @function
set_d:
	addi sp, sp, -24
	sw ra, 20(sp)
	sw s0, 16(sp)
	addi s0, sp, 24
	mv t0, a0
	mv t1, t0
	mv t0, t1
	sw t1, d_g, t6
	sw a0, -24(s0)
	mv a0, t0
	lw s0, 16(sp)
	lw ra, 20(sp)
	addi sp, sp, 24
	jr ra
	sw a0, -20(s0)
	li a0, 0
	lw s0, 16(sp)
	lw ra, 20(sp)
	addi sp, sp, 24
	jr ra
	.size	set_d, .-set_d
	.align	1
	.global	main
	.type	main, @function
main:
	addi sp, sp, -56
	sw ra, 52(sp)
	sw s0, 48(sp)
	addi s0, sp, 56
	call global
	li t0, 2
	li t1, 3
	li t2, 0
	sw t0, a_g, t6
	sw t1, b_g, t6
	sw t2, -20(s0)
	lw a0, -20(s0)
	call set_a
	mv t0, a0
	seqz t0, t0
	sw t0, -24(s0)
	bnez t0, .L0
	li t0, 1
	sw t0, -28(s0)
	lw a0, -28(s0)
	call set_b
	mv t0, a0
	seqz t0, t0
	sw t0, -20(s0)
	bnez t0, .L0
	j .L1
.L0:
	j .L1
.L1:
	lw t0, a_g
	mv t1, t0
	sw t0, a_g, t6
	sw t1, -20(s0)
	lw a0, -20(s0)
	call putint
	li t0, 32
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
	lw t0, b_g
	mv t1, t0
	sw t0, b_g, t6
	sw t1, -20(s0)
	lw a0, -20(s0)
	call putint
	li t0, 32
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
	li t0, 2
	li t1, 3
	li t2, 0
	sw t0, a_g, t6
	sw t1, b_g, t6
	sw t2, -20(s0)
	lw a0, -20(s0)
	call set_a
	mv t0, a0
	seqz t0, t0
	sw t0, -24(s0)
	bnez t0, .L2
	li t0, 1
	sw t0, -28(s0)
	lw a0, -28(s0)
	call set_b
	mv t0, a0
	seqz t0, t0
	sw t0, -20(s0)
	bnez t0, .L2
	j .L3
.L2:
	j .L3
.L3:
	lw t0, a_g
	mv t1, t0
	sw t0, a_g, t6
	sw t1, -20(s0)
	lw a0, -20(s0)
	call putint
	li t0, 32
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
	lw t0, b_g
	mv t1, t0
	sw t0, b_g, t6
	sw t1, -20(s0)
	lw a0, -20(s0)
	call putint
	li t0, 10
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
	li t0, 2
	li t1, 3
	sw t0, d_g, t6
	sw t1, -28(s0)
	lw a0, -28(s0)
	call set_d
	mv t0, a0
	seqz t0, t0
	sw t0, -20(s0)
	bnez t0, .L4
	j .L5
.L4:
	j .L5
.L5:
	lw t0, d_g
	mv t1, t0
	sw t0, d_g, t6
	sw t1, -20(s0)
	lw a0, -20(s0)
	call putint
	li t0, 32
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
	j .L7
	li t0, 4
	sw t0, -20(s0)
	lw a0, -20(s0)
	call set_d
	mv t0, a0
	seqz t0, t0
	sw t0, -24(s0)
	bnez t0, .L6
	j .L7
.L6:
	j .L7
.L7:
	lw t0, d_g
	mv t1, t0
	sw t0, d_g, t6
	sw t1, -20(s0)
	lw a0, -20(s0)
	call putint
	li t0, 10
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
	j .L8
	j .L9
.L8:
	li t0, 65
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
.L9:
	j .L10
	li t0, 66
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
.L10:
	j .L11
	li t0, 67
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
.L11:
	j .L12
	j .L13
.L12:
	li t0, 68
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
.L13:
	j .L14
	li t0, 69
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
.L14:
	j .L15
	j .L16
.L15:
	li t0, 70
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
.L16:
	li t0, 10
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
	li t0, 0
	li t1, 1
	li t2, 2
	li a0, 3
	li a1, 4
	sw t0, -32(s0)
	sw t1, -36(s0)
	sw t2, -40(s0)
	sw a0, -44(s0)
	sw a1, -48(s0)
.L17:
	lw t0, -32(s0)
	mv t1, t0
	seqz t1, t1
	sw t0, -32(s0)
	sw t1, -24(s0)
	bnez t1, .L18
	lw t0, -36(s0)
	mv t1, t0
	seqz t1, t1
	sw t0, -36(s0)
	sw t1, -20(s0)
	bnez t1, .L18
	j .L19
.L18:
	j .L20
.L19:
	li t0, 32
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
	j .L17
.L20:
	lw t0, -32(s0)
	mv t1, t0
	seqz t1, t1
	sw t0, -32(s0)
	sw t1, -24(s0)
	bnez t1, .L21
	j .L23
.L21:
	lw t0, -36(s0)
	mv t1, t0
	seqz t1, t1
	sw t0, -36(s0)
	sw t1, -24(s0)
	bnez t1, .L22
	j .L23
.L22:
	j .L24
.L23:
	li t0, 67
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
.L24:
	lw t0, -32(s0)
	mv t1, t0
	lw t2, -36(s0)
	mv a0, t2
	mv a1, t1
	sub a2, a1, a0
	sltz a1, a2
	seqz a1, a1
	seqz a1, a1
	sw t0, -32(s0)
	sw t2, -36(s0)
	sw t1, -20(s0)
	sw a0, -28(s0)
	sw a2, -52(s0)
	sw a1, -24(s0)
	bnez a1, .L25
	j .L27
.L25:
	lw t0, -36(s0)
	mv t1, t0
	lw t2, -32(s0)
	mv a0, t2
	mv a1, t1
	sub a2, a1, a0
	sgtz a1, a2
	seqz a1, a1
	seqz a1, a1
	sw t0, -36(s0)
	sw t2, -32(s0)
	sw t1, -20(s0)
	sw a0, -28(s0)
	sw a2, -52(s0)
	sw a1, -24(s0)
	bnez a1, .L26
	j .L27
.L26:
	j .L28
.L27:
	li t0, 72
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
.L28:
	lw t0, -40(s0)
	mv t1, t0
	lw t2, -36(s0)
	mv a0, t2
	mv a1, t1
	sub a2, a1, a0
	sltz a1, a2
	seqz a1, a1
	seqz a1, a1
	sw t0, -40(s0)
	sw t2, -36(s0)
	sw t1, -20(s0)
	sw a0, -28(s0)
	sw a2, -52(s0)
	sw a1, -24(s0)
	bnez a1, .L29
	lw t0, -48(s0)
	mv t1, t0
	lw t2, -44(s0)
	mv a0, t2
	mv a1, t1
	sub a2, a1, a0
	snez a1, a2
	seqz a1, a1
	sw t0, -48(s0)
	sw t2, -44(s0)
	sw t1, -28(s0)
	sw a0, -52(s0)
	sw a2, -56(s0)
	sw a1, -20(s0)
	bnez a1, .L29
	j .L30
.L29:
	j .L31
.L30:
	li t0, 73
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
.L31:
	lw t0, -32(s0)
	mv t1, t0
	lw t2, -36(s0)
	mv a0, t2
	seqz a0, a0
	mv a1, t1
	sub a2, a1, a0
	seqz a1, a2
	seqz a1, a1
	sw t0, -32(s0)
	sw t2, -36(s0)
	sw t1, -20(s0)
	sw a0, -28(s0)
	sw a2, -52(s0)
	sw a1, -24(s0)
	bnez a1, .L32
	lw t0, -44(s0)
	mv t1, t0
	mv t2, t0
	mv a0, t1
	sub a1, a0, t2
	sltz a0, a1
	seqz a0, a0
	sw t0, -44(s0)
	sw t1, -28(s0)
	sw t2, -52(s0)
	sw a1, -56(s0)
	sw a0, -20(s0)
	bnez a0, .L32
	j .L34
.L32:
	lw t0, -48(s0)
	mv t1, t0
	mv t2, t0
	mv a0, t1
	sub a1, a0, t2
	sltz a0, a1
	seqz a0, a0
	seqz a0, a0
	sw t0, -48(s0)
	sw t1, -20(s0)
	sw t2, -28(s0)
	sw a1, -52(s0)
	sw a0, -24(s0)
	bnez a0, .L33
	j .L34
.L33:
	j .L35
.L34:
	li t0, 74
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
.L35:
	lw t0, -32(s0)
	mv t1, t0
	lw t2, -36(s0)
	mv a0, t2
	seqz a0, a0
	mv a1, t1
	sub a2, a1, a0
	seqz a1, a2
	seqz a1, a1
	sw t0, -32(s0)
	sw t2, -36(s0)
	sw t1, -20(s0)
	sw a0, -28(s0)
	sw a2, -52(s0)
	sw a1, -24(s0)
	bnez a1, .L36
	j .L38
.L36:
	lw t0, -44(s0)
	mv t1, t0
	mv t2, t0
	mv a0, t1
	sub a1, a0, t2
	sltz a0, a1
	seqz a0, a0
	sw t0, -44(s0)
	sw t1, -20(s0)
	sw t2, -28(s0)
	sw a1, -52(s0)
	sw a0, -24(s0)
	bnez a0, .L37
	lw t0, -48(s0)
	mv t1, t0
	mv t2, t0
	mv a0, t1
	sub a1, a0, t2
	sltz a0, a1
	seqz a0, a0
	seqz a0, a0
	sw t0, -48(s0)
	sw t1, -28(s0)
	sw t2, -52(s0)
	sw a1, -56(s0)
	sw a0, -20(s0)
	bnez a0, .L37
	j .L38
.L37:
	j .L39
.L38:
	li t0, 75
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
.L39:
	li t0, 10
	sw t0, -20(s0)
	lw a0, -20(s0)
	call putch
	li a0, 0
	lw s0, 48(sp)
	lw ra, 52(sp)
	addi sp, sp, 56
	jr ra
	li a0, 0
	lw s0, 48(sp)
	lw ra, 52(sp)
	addi sp, sp, 56
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
	li t0, 0
	li t1, 0
	li t2, 0
	sw t0, a_g, t6
	sw t1, b_g, t6
	sw t2, d_g, t6
	lw s0, 8(sp)
	lw ra, 12(sp)
	addi sp, sp, 16
	jr ra
	.size	global, .-global
	.section	.rodata
