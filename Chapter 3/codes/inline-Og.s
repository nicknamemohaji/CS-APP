	.file	"inline.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"x = %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
#APP
# 27 "inline.c" 1
	movl $0x0, %eax;
movl $0x0, %edx;
leal 7(%edx, %edx, 4), %edx;
movl %edx, 12(%rsp);
# 0 "" 2
#NO_APP
	movl	12(%rsp), %edx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
