	.file	"exchange.c"
	.text
	.globl	exchage
	.type	exchage, @function
exchage:
.LFB0:
	.cfi_startproc
	movq	(%rdi), %rax
	movq	%rsi, (%rdi)
	ret
	.cfi_endproc
.LFE0:
	.size	exchage, .-exchage
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
