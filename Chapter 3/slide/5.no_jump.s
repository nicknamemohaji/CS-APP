switch_eg:
	cmpq	$103, %rsi
	je	.L3
	cmpq	$103, %rsi
	jg	.L4
	cmpq	$100, %rsi
	je	.L5
	addq	$10, %rdi
	cmpq	$102, %rsi
	je	.L3
	jmp	.L8
.L4:
	cmpq	$104, %rsi
	je	.L7
	cmpq	$106, %rsi
	je	.L7
	jmp	.L8
.L5:
	leaq	(%rdi,%rdi,2), %rax
	leaq	(%rdi,%rax,4), %rdi
	jmp	.L2
.L3:
	addq	$11, %rdi
	jmp	.L2
.L7:
	imulq	%rdi, %rdi
	jmp	.L2
.L8:
	movl	$0, %edi
.L2:
	movq	%rdi, (%rdx)
	ret