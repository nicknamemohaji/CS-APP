min:
	cmpl	%esi, %edi
	jge	.L3
	movl	%edi, %eax
	ret
.L3:
	movl	%esi, %eax
	ret
