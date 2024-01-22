	.file	"prob_3.6.c"
	.text
	.globl	scale3
	.type	scale3, @function
scale3:
.LFB0:
	.cfi_startproc
	leal	(%rdi,%rdi,4), %edi
	leal	(%rdi,%rsi,2), %esi
	leal	(%rsi,%rdx,8), %eax
	ret
	.cfi_endproc
.LFE0:
	.size	scale3, .-scale3
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
