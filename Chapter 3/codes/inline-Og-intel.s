	.file	"inline.c"
	.intel_syntax noprefix
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"x = %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	sub	rsp, 24
	.cfi_def_cfa_offset 32
#APP
# 29 "inline.c" 1
	movl $0x0, %eax;
movl $0x0, %edx;
leal 7(%edx, %edx, 4), %edx;
movl %edx, DWORD PTR [rsp+12];
# 0 "" 2
#NO_APP
	mov	edx, DWORD PTR [rsp+12]
	mov	esi, OFFSET FLAT:.LC0
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk
	add	rsp, 24
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
