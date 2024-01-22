.LC0:
	.string	"%d\n"
.LC1:
	.string	"end"

for_loop:
	pushq	%rbx
	movl	$0, %ebx
	jmp	.L2
.L3:
	movl	%ebx, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	addl	$1, %ebx
.L2:
	cmpl	$9, %ebx
	jle	.L3
	movl	$.LC1, %edi
	call	puts
	popq	%rbx
	ret
