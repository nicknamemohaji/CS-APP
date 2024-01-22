	.file	"comp.c"
	.text
	.globl	comp
	.type	comp, @function
comp:
.LFB0:
	.cfi_startproc
	cmpq	%rsi, %rdi
	setl	%al
	movzbl	%al, %eax
	ret
	.cfi_endproc
.LFE0:
	.size	comp, .-comp
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
