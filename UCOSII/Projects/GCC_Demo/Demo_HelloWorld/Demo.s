	.file	"Demo.c"
	.text
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
LC0:
	.ascii "System Start!! \0"
LC1:
	.ascii "%d\12\0"
LC2:
	.ascii "c > 2 \0"
LC3:
	.ascii "pause\0"
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB23:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$32, %esp
	call	___main
	movl	$1, 28(%esp)
	movl	$2, 24(%esp)
	movl	$0, 20(%esp)
	movl	$LC0, (%esp)
	call	_puts
	movl	28(%esp), %edx
	movl	24(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 20(%esp)
	movl	20(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$LC1, (%esp)
	call	_printf
	cmpl	$2, 20(%esp)
	jle	L2
	movl	$LC2, (%esp)
	call	_puts
L2:
	call	_DoNothing
	movl	$LC3, (%esp)
	call	_system
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE23:
	.ident	"GCC: (MinGW.org GCC Build-2) 9.2.0"
	.def	_puts;	.scl	2;	.type	32;	.endef
	.def	_printf;	.scl	2;	.type	32;	.endef
	.def	_DoNothing;	.scl	2;	.type	32;	.endef
	.def	_system;	.scl	2;	.type	32;	.endef
