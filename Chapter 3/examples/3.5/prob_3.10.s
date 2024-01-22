	.file	"prob_3.10.c"
	.text
	.globl	arith3
	.type	arith3, @function
arith3:
.LFB0:
	.cfi_startproc
	movl	%edx, %eax
	orl	%esi, %eax
	sarw	$9, %ax
	notl	%eax
	subl	%esi, %eax
	ret
	.cfi_endproc
.LFE0:
	.size	arith3, .-arith3
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
