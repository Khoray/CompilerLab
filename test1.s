	.text
	.comm	a0_g,4,4
	.comm	a1_g,4,4
	.comm	a2_g,4,4
	.comm	a3_g,4,4
	.comm	a4_g,4,4
	.comm	a5_g,4,4
	.comm	a6_g,4,4
	.comm	a7_g,4,4
	.comm	a8_g,4,4
	.comm	a9_g,4,4
	.comm	a10_g,4,4
	.comm	a11_g,4,4
	.comm	a12_g,4,4
	.comm	a13_g,4,4
	.comm	a14_g,4,4
	.comm	a15_g,4,4
	.comm	a16_g,4,4
	.comm	a17_g,4,4
	.comm	a18_g,4,4
	.comm	a19_g,4,4
	.comm	a20_g,4,4
	.comm	a21_g,4,4
	.comm	a22_g,4,4
	.comm	a23_g,4,4
	.comm	a24_g,4,4
	.comm	a25_g,4,4
	.comm	a26_g,4,4
	.comm	a27_g,4,4
	.comm	a28_g,4,4
	.comm	a29_g,4,4
	.comm	a30_g,4,4
	.comm	a31_g,4,4
	.comm	a32_g,4,4
	.comm	a33_g,4,4
	.comm	a34_g,4,4
	.comm	a35_g,4,4
	.comm	a36_g,4,4
	.comm	a37_g,4,4
	.comm	a38_g,4,4
	.comm	a39_g,4,4
	.align	1
	.global	testParam8
	.type	testParam8, @function
testParam8:
	addi sp, sp, -84
	sw ra, 80(sp)
	sw s0, 76(sp)
	addi s0, sp, 84
	lw t0, -20(s0)
	mv t0, a0
	lw t1, -24(s0)
	mv t1, a1
	lw t2, -28(s0)
	mv t2, a2
	sw t0, -20(s0)
	lw t0, -32(s0)
	mv t0, a3
	sw a0, -36(s0)
	lw a0, -40(s0)
	mv a0, a4
	sw a6, -44(s0)
	lw a6, -48(s0)
	mv a6, a5
	sw a7, -52(s0)
	lw a7, -44(s0)
	sw t1, -24(s0)
	lw t1, -56(s0)
	mv t1, a7
	sw a1, -60(s0)
	lw a1, -52(s0)
	sw t2, -28(s0)
	lw t2, -64(s0)
	mv t2, a1
	sw a2, -68(s0)
	lw a2, -20(s0)
	sw t0, -32(s0)
	lw t0, -72(s0)
	mv t0, a2
	sw a3, -76(s0)
	lw a3, -24(s0)
	add t0, t0, a3
	sw a0, -40(s0)
	lw a0, -28(s0)
	add t0, t0, a0
	sw a4, -80(s0)
	lw a4, -32(s0)
	add t0, t0, a4
	sw a5, -84(s0)
	lw a5, -40(s0)
	add t0, t0, a5
	add t0, t0, a6
	add t0, t0, t1
	add t0, t0, t2
	sw a0, -28(s0)
	mv a0, t0
	lw s0, 76(sp)
	lw ra, 80(sp)
	addi sp, sp, 84
	jr ra
	sw a0, -72(s0)
	li a0, 0
	lw s0, 76(sp)
	lw ra, 80(sp)
	addi sp, sp, 84
	jr ra
	.size	testParam8, .-testParam8
	.align	1
	.global	testParam16
	.type	testParam16, @function
testParam16:
	addi sp, sp, -116
	sw ra, 112(sp)
	sw s0, 108(sp)
	addi s0, sp, 116
	lw t0, -20(s0)
	mv t0, a0
	lw t1, -24(s0)
	mv t1, a1
	lw t2, -28(s0)
	mv t2, a2
	sw t0, -20(s0)
	lw t0, -32(s0)
	mv t0, a3
	sw a0, -36(s0)
	lw a0, -40(s0)
	mv a0, a4
	sw a6, -44(s0)
	lw a6, -48(s0)
	mv a6, a5
	sw a7, -52(s0)
	lw a7, -44(s0)
	sw t1, -24(s0)
	lw t1, -56(s0)
	mv t1, a7
	sw a1, -60(s0)
	lw a1, -52(s0)
	sw t2, -28(s0)
	lw t2, -64(s0)
	mv t2, a1
	sw a2, -68(s0)
	lw a2, 0(s0)
	sw t0, -32(s0)
	lw t0, -72(s0)
	mv t0, a2
	sw a3, -76(s0)
	lw a3, 4(s0)
	sw a0, -40(s0)
	lw a0, -80(s0)
	mv a0, a3
	sw a4, -84(s0)
	lw a4, 8(s0)
	sw a5, -88(s0)
	lw a5, -92(s0)
	mv a5, a4
	sw a6, -48(s0)
	lw a6, 12(s0)
	sw t1, -56(s0)
	lw t1, -96(s0)
	mv t1, a6
	sw a7, -44(s0)
	lw a7, 16(s0)
	sw t2, -64(s0)
	lw t2, -100(s0)
	mv t2, a7
	sw a1, -52(s0)
	lw a1, 20(s0)
	sw t0, -72(s0)
	lw t0, -104(s0)
	mv t0, a1
	sw a2, 0(s0)
	lw a2, 24(s0)
	sw a0, -80(s0)
	lw a0, -108(s0)
	mv a0, a2
	sw a3, 4(s0)
	lw a3, 28(s0)
	sw a4, 8(s0)
	lw a4, -112(s0)
	mv a4, a3
	sw a5, -92(s0)
	lw a5, -20(s0)
	sw t1, -96(s0)
	lw t1, -116(s0)
	mv t1, a5
	sw a6, 12(s0)
	lw a6, -24(s0)
	add t1, t1, a6
	sw t2, -100(s0)
	lw t2, -28(s0)
	add t1, t1, t2
	sw a7, 16(s0)
	lw a7, -32(s0)
	sub t1, t1, a7
	sw t0, -104(s0)
	lw t0, -40(s0)
	sub t1, t1, t0
	sw a1, 20(s0)
	lw a1, -48(s0)
	sub t1, t1, a1
	sw a0, -108(s0)
	lw a0, -56(s0)
	sub t1, t1, a0
	sw a2, 24(s0)
	lw a2, -64(s0)
	sub t1, t1, a2
	sw a3, 28(s0)
	lw a3, -72(s0)
	add t1, t1, a3
	sw a4, -112(s0)
	lw a4, -80(s0)
	add t1, t1, a4
	sw a5, -20(s0)
	lw a5, -92(s0)
	add t1, t1, a5
	sw a6, -24(s0)
	lw a6, -96(s0)
	add t1, t1, a6
	sw t2, -28(s0)
	lw t2, -100(s0)
	add t1, t1, t2
	sw a7, -32(s0)
	lw a7, -104(s0)
	add t1, t1, a7
	sw t0, -40(s0)
	lw t0, -108(s0)
	add t1, t1, t0
	sw a1, -48(s0)
	lw a1, -112(s0)
	add t1, t1, a1
	sw a0, -56(s0)
	mv a0, t1
	lw s0, 108(sp)
	lw ra, 112(sp)
	addi sp, sp, 116
	jr ra
	sw a0, -116(s0)
	li a0, 0
	lw s0, 108(sp)
	lw ra, 112(sp)
	addi sp, sp, 116
	jr ra
	.size	testParam16, .-testParam16
	.align	1
	.global	testParam32
	.type	testParam32, @function
testParam32:
	addi sp, sp, -180
	sw ra, 176(sp)
	sw s0, 172(sp)
	addi s0, sp, 180
	lw t0, -20(s0)
	mv t0, a0
	lw t1, -24(s0)
	mv t1, a1
	lw t2, -28(s0)
	mv t2, a2
	sw t0, -20(s0)
	lw t0, -32(s0)
	mv t0, a3
	sw a0, -36(s0)
	lw a0, -40(s0)
	mv a0, a4
	sw a6, -44(s0)
	lw a6, -48(s0)
	mv a6, a5
	sw a7, -52(s0)
	lw a7, -44(s0)
	sw t1, -24(s0)
	lw t1, -56(s0)
	mv t1, a7
	sw a1, -60(s0)
	lw a1, -52(s0)
	sw t2, -28(s0)
	lw t2, -64(s0)
	mv t2, a1
	sw a2, -68(s0)
	lw a2, 0(s0)
	sw t0, -32(s0)
	lw t0, -72(s0)
	mv t0, a2
	sw a3, -76(s0)
	lw a3, 4(s0)
	sw a0, -40(s0)
	lw a0, -80(s0)
	mv a0, a3
	sw a4, -84(s0)
	lw a4, 8(s0)
	sw a5, -88(s0)
	lw a5, -92(s0)
	mv a5, a4
	sw a6, -48(s0)
	lw a6, 12(s0)
	sw t1, -56(s0)
	lw t1, -96(s0)
	mv t1, a6
	sw a7, -44(s0)
	lw a7, 16(s0)
	sw t2, -64(s0)
	lw t2, -100(s0)
	mv t2, a7
	sw a1, -52(s0)
	lw a1, 20(s0)
	sw t0, -72(s0)
	lw t0, -104(s0)
	mv t0, a1
	sw a2, 0(s0)
	lw a2, 24(s0)
	sw a0, -80(s0)
	lw a0, -108(s0)
	mv a0, a2
	sw a3, 4(s0)
	lw a3, 28(s0)
	sw a4, 8(s0)
	lw a4, -112(s0)
	mv a4, a3
	sw a5, -92(s0)
	lw a5, 32(s0)
	sw t1, -96(s0)
	lw t1, -116(s0)
	mv t1, a5
	sw a6, 12(s0)
	lw a6, 36(s0)
	sw t2, -100(s0)
	lw t2, -120(s0)
	mv t2, a6
	sw a7, 16(s0)
	lw a7, 40(s0)
	sw t0, -104(s0)
	lw t0, -124(s0)
	mv t0, a7
	sw a1, 20(s0)
	lw a1, 44(s0)
	sw a0, -108(s0)
	lw a0, -128(s0)
	mv a0, a1
	sw a2, 24(s0)
	lw a2, 48(s0)
	sw a3, 28(s0)
	lw a3, -132(s0)
	mv a3, a2
	sw a4, -112(s0)
	lw a4, 52(s0)
	sw t1, -116(s0)
	lw t1, -136(s0)
	mv t1, a4
	sw a5, 32(s0)
	lw a5, 56(s0)
	sw t2, -120(s0)
	lw t2, -140(s0)
	mv t2, a5
	sw a6, 36(s0)
	lw a6, 60(s0)
	sw t0, -124(s0)
	lw t0, -144(s0)
	mv t0, a6
	sw a7, 40(s0)
	lw a7, 64(s0)
	sw a0, -128(s0)
	lw a0, -148(s0)
	mv a0, a7
	sw a1, 44(s0)
	lw a1, 68(s0)
	sw a2, 48(s0)
	lw a2, -152(s0)
	mv a2, a1
	sw a3, -132(s0)
	lw a3, 72(s0)
	sw t1, -136(s0)
	lw t1, -156(s0)
	mv t1, a3
	sw a4, 52(s0)
	lw a4, 76(s0)
	sw t2, -140(s0)
	lw t2, -160(s0)
	mv t2, a4
	sw a5, 56(s0)
	lw a5, 80(s0)
	sw t0, -144(s0)
	lw t0, -164(s0)
	mv t0, a5
	sw a6, 60(s0)
	lw a6, 84(s0)
	sw a0, -148(s0)
	lw a0, -168(s0)
	mv a0, a6
	sw a7, 64(s0)
	lw a7, 88(s0)
	sw a1, 68(s0)
	lw a1, -172(s0)
	mv a1, a7
	sw a2, -152(s0)
	lw a2, 92(s0)
	sw t1, -156(s0)
	lw t1, -176(s0)
	mv t1, a2
	sw a3, 72(s0)
	lw a3, -20(s0)
	sw t2, -160(s0)
	lw t2, -180(s0)
	mv t2, a3
	sw a4, 76(s0)
	lw a4, -24(s0)
	add t2, t2, a4
	sw t0, -164(s0)
	lw t0, -28(s0)
	add t2, t2, t0
	sw a5, 80(s0)
	lw a5, -32(s0)
	add t2, t2, a5
	sw a0, -168(s0)
	lw a0, -40(s0)
	add t2, t2, a0
	sw a6, 84(s0)
	lw a6, -48(s0)
	add t2, t2, a6
	sw a1, -172(s0)
	lw a1, -56(s0)
	add t2, t2, a1
	sw a7, 88(s0)
	lw a7, -64(s0)
	add t2, t2, a7
	sw t1, -176(s0)
	lw t1, -72(s0)
	add t2, t2, t1
	sw a2, 92(s0)
	lw a2, -80(s0)
	add t2, t2, a2
	sw a3, -20(s0)
	lw a3, -92(s0)
	add t2, t2, a3
	sw a4, -24(s0)
	lw a4, -96(s0)
	add t2, t2, a4
	sw t0, -28(s0)
	lw t0, -100(s0)
	add t2, t2, t0
	sw a5, -32(s0)
	lw a5, -104(s0)
	add t2, t2, a5
	sw a0, -40(s0)
	lw a0, -108(s0)
	add t2, t2, a0
	sw a6, -48(s0)
	lw a6, -112(s0)
	add t2, t2, a6
	sw a1, -56(s0)
	lw a1, -116(s0)
	add t2, t2, a1
	sw a7, -64(s0)
	lw a7, -120(s0)
	add t2, t2, a7
	sw t1, -72(s0)
	lw t1, -124(s0)
	sub t2, t2, t1
	sw a2, -80(s0)
	lw a2, -128(s0)
	sub t2, t2, a2
	sw a3, -92(s0)
	lw a3, -132(s0)
	sub t2, t2, a3
	sw a4, -96(s0)
	lw a4, -136(s0)
	sub t2, t2, a4
	sw t0, -100(s0)
	lw t0, -140(s0)
	sub t2, t2, t0
	sw a5, -104(s0)
	lw a5, -144(s0)
	add t2, t2, a5
	sw a0, -108(s0)
	lw a0, -148(s0)
	add t2, t2, a0
	sw a6, -112(s0)
	lw a6, -152(s0)
	add t2, t2, a6
	sw a1, -116(s0)
	lw a1, -156(s0)
	add t2, t2, a1
	sw a7, -120(s0)
	lw a7, -160(s0)
	add t2, t2, a7
	sw t1, -124(s0)
	lw t1, -164(s0)
	add t2, t2, t1
	sw a2, -128(s0)
	lw a2, -168(s0)
	add t2, t2, a2
	sw a3, -132(s0)
	lw a3, -172(s0)
	add t2, t2, a3
	sw a4, -136(s0)
	lw a4, -176(s0)
	add t2, t2, a4
	sw a0, -148(s0)
	mv a0, t2
	lw s0, 172(sp)
	lw ra, 176(sp)
	addi sp, sp, 180
	jr ra
	sw a0, -180(s0)
	li a0, 0
	lw s0, 172(sp)
	lw ra, 176(sp)
	addi sp, sp, 180
	jr ra
	.size	testParam32, .-testParam32
	.align	1
	.global	main
	.type	main, @function
main:
	addi sp, sp, -148
	sw ra, 144(sp)
	sw s0, 140(sp)
	addi s0, sp, 148
	call global
	lw t0, a0_g
	li t0, 0
	lw t1, a1_g
	li t1, 1
	lw t2, a2_g
	li t2, 2
	lw a0, a3_g
	li a0, 3
	lw a1, a4_g
	li a1, 4
	lw a2, a5_g
	li a2, 5
	lw a3, a6_g
	li a3, 6
	lw a4, a7_g
	li a4, 7
	lw a5, a8_g
	li a5, 8
	lw a6, a9_g
	li a6, 9
	lw a7, a10_g
	li a7, 0
	sw t0, a0_g, t6
	lw t0, a11_g
	li t0, 1
	sw t1, a1_g, t6
	lw t1, a12_g
	li t1, 2
	sw t2, a2_g, t6
	lw t2, a13_g
	li t2, 3
	sw a0, a3_g, t6
	lw a0, a14_g
	li a0, 4
	sw a1, a4_g, t6
	lw a1, a15_g
	li a1, 5
	sw a2, a5_g, t6
	lw a2, a16_g
	li a2, 6
	sw a3, a6_g, t6
	lw a3, a17_g
	li a3, 7
	sw a4, a7_g, t6
	lw a4, a18_g
	li a4, 8
	sw a5, a8_g, t6
	lw a5, a19_g
	li a5, 9
	sw a6, a9_g, t6
	lw a6, a20_g
	li a6, 0
	sw a7, a10_g, t6
	lw a7, a21_g
	li a7, 1
	sw t0, a11_g, t6
	lw t0, a22_g
	li t0, 2
	sw t1, a12_g, t6
	lw t1, a23_g
	li t1, 3
	sw t2, a13_g, t6
	lw t2, a24_g
	li t2, 4
	sw a0, a14_g, t6
	lw a0, a25_g
	li a0, 5
	sw a1, a15_g, t6
	lw a1, a26_g
	li a1, 6
	sw a2, a16_g, t6
	lw a2, a27_g
	li a2, 7
	sw a3, a17_g, t6
	lw a3, a28_g
	li a3, 8
	sw a4, a18_g, t6
	lw a4, a29_g
	li a4, 9
	sw a5, a19_g, t6
	lw a5, a30_g
	li a5, 0
	sw a6, a20_g, t6
	lw a6, a31_g
	li a6, 1
	sw a7, a21_g, t6
	lw a7, a32_g
	li a7, 4
	sw t0, a22_g, t6
	lw t0, a33_g
	li t0, 5
	sw t1, a23_g, t6
	lw t1, a34_g
	li t1, 6
	sw t2, a24_g, t6
	lw t2, a35_g
	li t2, 7
	sw a0, a25_g, t6
	lw a0, a36_g
	li a0, 8
	sw a1, a26_g, t6
	lw a1, a37_g
	li a1, 9
	sw a2, a27_g, t6
	lw a2, a38_g
	li a2, 0
	sw a3, a28_g, t6
	lw a3, a39_g
	li a3, 1
	sw a4, a29_g, t6
	lw a4, a0_g
	sw a5, a30_g, t6
	lw a5, -20(s0)
	mv a5, a4
	sw a6, a31_g, t6
	lw a6, a1_g
	sw a7, a32_g, t6
	lw a7, -24(s0)
	mv a7, a6
	sw t0, a33_g, t6
	lw t0, a2_g
	sw t1, a34_g, t6
	lw t1, -28(s0)
	mv t1, t0
	sw t2, a35_g, t6
	lw t2, a3_g
	sw a0, a36_g, t6
	lw a0, -32(s0)
	mv a0, t2
	sw a1, a37_g, t6
	lw a1, a4_g
	sw a2, a38_g, t6
	lw a2, -36(s0)
	mv a2, a1
	sw a3, a39_g, t6
	lw a3, a5_g
	sw a4, a0_g, t6
	lw a4, -40(s0)
	mv a4, a3
	sw a5, -20(s0)
	lw a5, a6_g
	sw a6, a1_g, t6
	lw a6, -44(s0)
	mv a6, a5
	sw a7, -24(s0)
	lw a7, a7_g
	sw t0, a2_g, t6
	lw t0, -48(s0)
	mv t0, a7
	sw t1, -28(s0)
	lw t1, a8_g
	sw t2, a3_g, t6
	lw t2, -52(s0)
	mv t2, t1
	sw a0, -32(s0)
	lw a0, a9_g
	sw a1, a4_g, t6
	lw a1, -56(s0)
	mv a1, a0
	sw a2, -36(s0)
	lw a2, a10_g
	sw a3, a5_g, t6
	lw a3, -60(s0)
	mv a3, a2
	sw a4, -40(s0)
	lw a4, a11_g
	sw a5, a6_g, t6
	lw a5, -64(s0)
	mv a5, a4
	sw a6, -44(s0)
	lw a6, a12_g
	sw t0, -48(s0)
	lw t0, -68(s0)
	mv t0, a6
	sw a7, a7_g, t6
	lw a7, a13_g
	sw t1, a8_g, t6
	lw t1, -72(s0)
	mv t1, a7
	sw t2, -52(s0)
	lw t2, a14_g
	sw a0, a9_g, t6
	lw a0, -76(s0)
	mv a0, t2
	sw a1, -56(s0)
	lw a1, a15_g
	sw a2, a10_g, t6
	lw a2, -80(s0)
	mv a2, a1
	sw a3, -60(s0)
	lw a3, a16_g
	sw a4, a11_g, t6
	lw a4, -84(s0)
	mv a4, a3
	sw a5, -64(s0)
	lw a5, a17_g
	sw t0, -68(s0)
	lw t0, -88(s0)
	mv t0, a5
	sw a6, a12_g, t6
	lw a6, a18_g
	sw t1, -72(s0)
	lw t1, -92(s0)
	mv t1, a6
	sw a7, a13_g, t6
	lw a7, a19_g
	sw t2, a14_g, t6
	lw t2, -96(s0)
	mv t2, a7
	sw a0, -76(s0)
	lw a0, a20_g
	sw a1, a15_g, t6
	lw a1, -100(s0)
	mv a1, a0
	sw a2, -80(s0)
	lw a2, a21_g
	sw a3, a16_g, t6
	lw a3, -104(s0)
	mv a3, a2
	sw a4, -84(s0)
	lw a4, a22_g
	sw t0, -88(s0)
	lw t0, -108(s0)
	mv t0, a4
	sw a5, a17_g, t6
	lw a5, a23_g
	sw t1, -92(s0)
	lw t1, -112(s0)
	mv t1, a5
	sw a6, a18_g, t6
	lw a6, a24_g
	sw t2, -96(s0)
	lw t2, -116(s0)
	mv t2, a6
	sw a7, a19_g, t6
	lw a7, a25_g
	sw a0, a20_g, t6
	lw a0, -120(s0)
	mv a0, a7
	sw a1, -100(s0)
	lw a1, a26_g
	sw a2, a21_g, t6
	lw a2, -124(s0)
	mv a2, a1
	sw a3, -104(s0)
	lw a3, a27_g
	sw t0, -108(s0)
	lw t0, -128(s0)
	mv t0, a3
	sw a4, a22_g, t6
	lw a4, a28_g
	sw t1, -112(s0)
	lw t1, -132(s0)
	mv t1, a4
	sw a5, a23_g, t6
	lw a5, a29_g
	sw t2, -116(s0)
	lw t2, -136(s0)
	mv t2, a5
	sw a6, a24_g, t6
	lw a6, a30_g
	sw a0, -120(s0)
	lw a0, -140(s0)
	mv a0, a6
	sw a7, a25_g, t6
	lw a7, a31_g
	sw a1, a26_g, t6
	lw a1, -144(s0)
	mv a1, a7
	sw a2, -124(s0)
	sw t0, -128(s0)
	sw a3, a27_g, t6
	sw t1, -132(s0)
	sw a4, a28_g, t6
	sw t2, -136(s0)
	sw a5, a29_g, t6
	sw a0, -140(s0)
	sw a6, a30_g, t6
	sw a1, -144(s0)
	sw a7, a31_g, t6
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
	lw t0, -60(s0)
	sw t0, 8(sp)
	sw t0, -60(s0)
	lw t0, -64(s0)
	sw t0, 12(sp)
	sw t0, -64(s0)
	lw t0, -68(s0)
	sw t0, 16(sp)
	sw t0, -68(s0)
	lw t0, -72(s0)
	sw t0, 20(sp)
	sw t0, -72(s0)
	lw t0, -76(s0)
	sw t0, 24(sp)
	sw t0, -76(s0)
	lw t0, -80(s0)
	sw t0, 28(sp)
	sw t0, -80(s0)
	lw t0, -84(s0)
	sw t0, 32(sp)
	sw t0, -84(s0)
	lw t0, -88(s0)
	sw t0, 36(sp)
	sw t0, -88(s0)
	lw t0, -92(s0)
	sw t0, 40(sp)
	sw t0, -92(s0)
	lw t0, -96(s0)
	sw t0, 44(sp)
	sw t0, -96(s0)
	lw t0, -100(s0)
	sw t0, 48(sp)
	sw t0, -100(s0)
	lw t0, -104(s0)
	sw t0, 52(sp)
	sw t0, -104(s0)
	lw t0, -108(s0)
	sw t0, 56(sp)
	sw t0, -108(s0)
	lw t0, -112(s0)
	sw t0, 60(sp)
	sw t0, -112(s0)
	lw t0, -116(s0)
	sw t0, 64(sp)
	sw t0, -116(s0)
	lw t0, -120(s0)
	sw t0, 68(sp)
	sw t0, -120(s0)
	lw t0, -124(s0)
	sw t0, 72(sp)
	sw t0, -124(s0)
	lw t0, -128(s0)
	sw t0, 76(sp)
	sw t0, -128(s0)
	lw t0, -132(s0)
	sw t0, 80(sp)
	sw t0, -132(s0)
	lw t0, -136(s0)
	sw t0, 84(sp)
	sw t0, -136(s0)
	lw t0, -140(s0)
	sw t0, 88(sp)
	sw t0, -140(s0)
	lw t0, -144(s0)
	sw t0, 92(sp)
	sw t0, -144(s0)
	call testParam32
	lw t0, -148(s0)
	mv t0, a0
	lw t1, a0_g
	mv t1, t0
	lw t2, -20(s0)
	mv t2, t1
	sw t0, -148(s0)
	sw t1, a0_g, t6
	sw t2, -20(s0)
	lw a0, -20(s0)
	call putint
	li a0, 0
	lw s0, 140(sp)
	lw ra, 144(sp)
	addi sp, sp, 148
	jr ra
	li a0, 0
	lw s0, 140(sp)
	lw ra, 144(sp)
	addi sp, sp, 148
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
	lw t0, a0_g
	li t0, 0
	lw t1, a1_g
	li t1, 0
	lw t2, a2_g
	li t2, 0
	lw a0, a3_g
	li a0, 0
	lw a1, a4_g
	li a1, 0
	lw a2, a5_g
	li a2, 0
	lw a3, a6_g
	li a3, 0
	lw a4, a7_g
	li a4, 0
	lw a5, a8_g
	li a5, 0
	lw a6, a9_g
	li a6, 0
	lw a7, a10_g
	li a7, 0
	sw t0, a0_g, t6
	lw t0, a11_g
	li t0, 0
	sw t1, a1_g, t6
	lw t1, a12_g
	li t1, 0
	sw t2, a2_g, t6
	lw t2, a13_g
	li t2, 0
	sw a0, a3_g, t6
	lw a0, a14_g
	li a0, 0
	sw a1, a4_g, t6
	lw a1, a15_g
	li a1, 0
	sw a2, a5_g, t6
	lw a2, a16_g
	li a2, 0
	sw a3, a6_g, t6
	lw a3, a17_g
	li a3, 0
	sw a4, a7_g, t6
	lw a4, a18_g
	li a4, 0
	sw a5, a8_g, t6
	lw a5, a19_g
	li a5, 0
	sw a6, a9_g, t6
	lw a6, a20_g
	li a6, 0
	sw a7, a10_g, t6
	lw a7, a21_g
	li a7, 0
	sw t0, a11_g, t6
	lw t0, a22_g
	li t0, 0
	sw t1, a12_g, t6
	lw t1, a23_g
	li t1, 0
	sw t2, a13_g, t6
	lw t2, a24_g
	li t2, 0
	sw a0, a14_g, t6
	lw a0, a25_g
	li a0, 0
	sw a1, a15_g, t6
	lw a1, a26_g
	li a1, 0
	sw a2, a16_g, t6
	lw a2, a27_g
	li a2, 0
	sw a3, a17_g, t6
	lw a3, a28_g
	li a3, 0
	sw a4, a18_g, t6
	lw a4, a29_g
	li a4, 0
	sw a5, a19_g, t6
	lw a5, a30_g
	li a5, 0
	sw a6, a20_g, t6
	lw a6, a31_g
	li a6, 0
	sw a7, a21_g, t6
	lw a7, a32_g
	li a7, 0
	sw t0, a22_g, t6
	lw t0, a33_g
	li t0, 0
	sw t1, a23_g, t6
	lw t1, a34_g
	li t1, 0
	sw t2, a24_g, t6
	lw t2, a35_g
	li t2, 0
	sw a0, a25_g, t6
	lw a0, a36_g
	li a0, 0
	sw a1, a26_g, t6
	lw a1, a37_g
	li a1, 0
	sw a2, a27_g, t6
	lw a2, a38_g
	li a2, 0
	sw a3, a28_g, t6
	lw a3, a39_g
	li a3, 0
	sw a4, a29_g, t6
	sw a5, a30_g, t6
	sw a6, a31_g, t6
	sw a7, a32_g, t6
	sw t0, a33_g, t6
	sw t1, a34_g, t6
	sw t2, a35_g, t6
	sw a0, a36_g, t6
	sw a1, a37_g, t6
	sw a2, a38_g, t6
	sw a3, a39_g, t6
	lw s0, 8(sp)
	lw ra, 12(sp)
	addi sp, sp, 16
	jr ra
	.size	global, .-global
