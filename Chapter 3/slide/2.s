min:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jge	.L2
	nop
.L3:
	movl	-4(%rbp), %eax
	jmp	.L4
.L2:
	movl	-8(%rbp), %eax
.L4:
	popq	%rbp
	ret