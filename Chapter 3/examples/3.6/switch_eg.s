	.file	"switch_eg.c"
	.text
	.globl	switch_eg
	.type	switch_eg, @function
switch_eg:
.LFB0:
	.cfi_startproc
	subq	$100, %rsi
	cmpq	$9, %rsi
	ja	.L9
	jmp	*.L4(,%rsi,8)
	.section	.rodata
	.align 8
	.align 4
.L4:
	.quad	.L3
	.quad	.L9
	.quad	.L5
	.quad	.L6
	.quad	.L7
	.quad	.L9
	.quad	.L7
	.quad	.L9
	.quad	.L9
	.quad	.L8
	.text
.L3:
	leaq	(%rdi,%rdi,2), %rax
	leaq	(%rdi,%rax,4), %rdi
	jmp	.L2
.L5:
	addq	$10, %rdi
.L6:
	addq	$11, %rdi
	jmp	.L2
.L7:
	imulq	%rdi, %rdi
	jmp	.L2
.L8:
	leaq	(%rdi,%rdi,4), %rax
	leaq	(%rdi,%rax,4), %rdi
	addq	%rdi, %rdi
	jmp	.L2
.L9:
	movl	$0, %edi
.L2:
	movq	%rdi, (%rdx)
	ret
	.cfi_endproc
.LFE0:
	.size	switch_eg, .-switch_eg
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
