	.file	"test2.c"
	.option nopic
	.text
	.comm	a,40,4
	.align	1
	.globl	cal
	.type	cal, @function
cal:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-20(s0)
	sw	a1,-24(s0)
	sw	a2,-28(s0)
	sw	a3,-32(s0)
	sw	a4,-36(s0)
	sw	a5,-40(s0)
	sw	a6,-44(s0)
	sw	a7,-48(s0)
	li	a5,1
	sw	a5,4(s0)
	lw	a4,0(s0)
	lw	a5,4(s0)
	add	a5,a4,a5
	mv	a0,a5
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	cal, .-cal
	.align	1
	.globl	cal2
	.type	cal2, @function
cal2:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-20(s0)
	sw	a1,-24(s0)
	sw	a2,-28(s0)
	sw	a3,-32(s0)
	sw	a4,-36(s0)
	sw	a5,-40(s0)
	sw	a6,-44(s0)
	sw	a7,-48(s0)
	lw	a4,0(s0)
	lw	a5,4(s0)
	add	a4,a4,a5
	lw	a5,8(s0)
	add	a5,a4,a5
	mv	a0,a5
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	cal2, .-cal2
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-48
	sw	ra,44(sp)
	sw	s0,40(sp)
	addi	s0,sp,48
	li	a5,1
	sw	a5,-20(s0)
	lui	a5,%hi(a)
	addi	a5,a5,%lo(a)
	lw	a0,0(a5)
	lui	a5,%hi(a)
	addi	a5,a5,%lo(a)
	lw	a1,4(a5)
	lui	a5,%hi(a)
	addi	a5,a5,%lo(a)
	lw	a2,8(a5)
	lui	a5,%hi(a)
	addi	a5,a5,%lo(a)
	lw	a3,12(a5)
	lui	a5,%hi(a)
	addi	a5,a5,%lo(a)
	lw	t1,16(a5)
	lui	a5,%hi(a)
	addi	a5,a5,%lo(a)
	lw	t3,20(a5)
	lui	a5,%hi(a)
	addi	a5,a5,%lo(a)
	lw	a6,24(a5)
	lui	a5,%hi(a)
	addi	a5,a5,%lo(a)
	lw	a7,28(a5)
	lui	a5,%hi(a)
	addi	a5,a5,%lo(a)
	lw	a5,32(a5)
	lui	a4,%hi(a)
	addi	a4,a4,%lo(a)
	lw	a4,36(a4)
	sw	a4,4(sp)
	sw	a5,0(sp)
	mv	a5,t3
	mv	a4,t1
	call	cal
	lw	a5,-20(s0)
	sw	a5,-24(s0)
	li	a5,0
	mv	a0,a5
	lw	ra,44(sp)
	lw	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 9.2.0"
	.section	.note.GNU-stack,"",@progbits
