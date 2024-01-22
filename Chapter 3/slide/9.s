.LC0:
	.string	"%d\n"
.LC1:
	.string	"end"

do_while_loop:
	pushq	%rbx
	movl	$0, %ebx
.L2:
	movl	%ebx, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	addl	$1, %ebx
	cmpl	$9, %ebx
	jle	.L2
	movl	$.LC1, %edi
	call	puts
	popq	%rbx
	ret
