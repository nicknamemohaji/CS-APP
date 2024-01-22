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
	jmp	.L9
.L4:
	cmpq	$106, %rsi
	je	.L7
	leaq	1000(%rdi), %rax
	cmpq	$1000, %rsi
	je	.L2
	cmpq	$104, %rsi
	jne	.L9
	jmp	.L7
.L5:
	leaq	(%rdi,%rdi,2), %rax
	leaq	(%rdi,%rax,4), %rax
	jmp	.L2
.L3:
	leaq	11(%rdi), %rax
	jmp	.L2
.L7:
	movq	%rdi, %rax
	imulq	%rdi, %rax
	jmp	.L2
.L9:
	movl	$0, %eax
.L2:
	movq	%rax, (%rdx)
	ret