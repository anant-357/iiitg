	.file	"a.c"
	.text
	.section	.rodata
.LC0:
	.string	"\nThe greater number is: %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$46, -8(%rbp)
	movl	$-50, -4(%rbp)
	movl	-8(%rbp), %eax
	cmpl	-4(%rbp), %eax
	jle	.L2
	movl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	jmp	.L3
.L2:
	movl	-4(%rbp), %eax
	movl	%eax, -12(%rbp)
.L3:
	movl	-12(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 13.2.1 20230801"
	.section	.note.GNU-stack,"",@progbits
