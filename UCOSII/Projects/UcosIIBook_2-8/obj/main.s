	.file	"main.c"
	.text
	.section .rdata,"dr"
LC0:
	.ascii "function_1 run \0"
	.text
	.globl	_fun_1
	.def	_fun_1;	.scl	2;	.type	32;	.endef
_fun_1:
LFB26:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	$0, -12(%ebp)
	jmp	L2
L3:
	movl	$LC0, (%esp)
	call	_puts
	addl	$1, -12(%ebp)
L2:
	cmpl	$9, -12(%ebp)
	jle	L3
	nop
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE26:
	.section .rdata,"dr"
LC1:
	.ascii "function_2 run \0"
	.text
	.globl	_fun_2
	.def	_fun_2;	.scl	2;	.type	32;	.endef
_fun_2:
LFB27:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	$0, -12(%ebp)
	jmp	L5
L6:
	movl	$LC1, (%esp)
	call	_puts
	addl	$1, -12(%ebp)
L5:
	cmpl	$9, -12(%ebp)
	jle	L6
	nop
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE27:
	.section .rdata,"dr"
LC2:
	.ascii "function_3 run \0"
	.text
	.globl	_fun_3
	.def	_fun_3;	.scl	2;	.type	32;	.endef
_fun_3:
LFB28:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	$0, -12(%ebp)
	jmp	L8
L9:
	movl	$LC2, (%esp)
	call	_puts
	addl	$1, -12(%ebp)
L8:
	cmpl	$9, -12(%ebp)
	jle	L9
	nop
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE28:
	.globl	_CreateTCB
	.def	_CreateTCB;	.scl	2;	.type	32;	.endef
_CreateTCB:
LFB29:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	12(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	16(%ebp), %eax
	movl	%eax, -12(%ebp)
	movl	20(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	24(%ebp), %eax
	movl	%eax, -4(%ebp)
	movl	8(%ebp), %eax
	movl	-16(%ebp), %edx
	movl	%edx, (%eax)
	movl	-12(%ebp), %edx
	movl	%edx, 4(%eax)
	movl	-8(%ebp), %edx
	movl	%edx, 8(%eax)
	movl	-4(%ebp), %edx
	movl	%edx, 12(%eax)
	movl	8(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE29:
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
LC3:
	.ascii "F1\0"
LC4:
	.ascii "F2\0"
LC5:
	.ascii "F3\0"
LC6:
	.ascii "Input CodeName:  \0"
LC7:
	.ascii "%s\0"
LC8:
	.ascii "No %s function! \12\0"
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB30:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	addl	$-128, %esp
	call	___main
	leal	32(%esp), %eax
	movl	$_fun_1, 16(%esp)
	movl	$1, 12(%esp)
	movl	$2, 8(%esp)
	movl	$LC3, 4(%esp)
	movl	%eax, (%esp)
	call	_CreateTCB
	movl	32(%esp), %eax
	movl	%eax, 60(%esp)
	movl	36(%esp), %eax
	movl	%eax, 64(%esp)
	movl	40(%esp), %eax
	movl	%eax, 68(%esp)
	movl	44(%esp), %eax
	movl	%eax, 72(%esp)
	leal	32(%esp), %eax
	movl	$_fun_2, 16(%esp)
	movl	$4, 12(%esp)
	movl	$2, 8(%esp)
	movl	$LC4, 4(%esp)
	movl	%eax, (%esp)
	call	_CreateTCB
	movl	32(%esp), %eax
	movl	%eax, 76(%esp)
	movl	36(%esp), %eax
	movl	%eax, 80(%esp)
	movl	40(%esp), %eax
	movl	%eax, 84(%esp)
	movl	44(%esp), %eax
	movl	%eax, 88(%esp)
	leal	32(%esp), %eax
	movl	$_fun_3, 16(%esp)
	movl	$5, 12(%esp)
	movl	$2, 8(%esp)
	movl	$LC5, 4(%esp)
	movl	%eax, (%esp)
	call	_CreateTCB
	movl	32(%esp), %eax
	movl	%eax, 92(%esp)
	movl	36(%esp), %eax
	movl	%eax, 96(%esp)
	movl	40(%esp), %eax
	movl	%eax, 100(%esp)
	movl	44(%esp), %eax
	movl	%eax, 104(%esp)
	movl	$LC6, (%esp)
	call	_puts
	leal	110(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$LC7, (%esp)
	call	_scanf
	movl	$0, 124(%esp)
	movl	$0, 120(%esp)
	jmp	L13
L16:
	movl	120(%esp), %eax
	sall	$4, %eax
	leal	128(%esp), %ecx
	addl	%ecx, %eax
	subl	$68, %eax
	movl	(%eax), %eax
	leal	110(%esp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_strcmp
	testl	%eax, %eax
	jne	L14
	movl	120(%esp), %eax
	sall	$4, %eax
	leal	128(%esp), %edx
	addl	%edx, %eax
	subl	$56, %eax
	movl	(%eax), %eax
	call	*%eax
	movl	$1, 124(%esp)
L14:
	cmpl	$2, 120(%esp)
	jne	L15
	cmpl	$0, 124(%esp)
	jne	L15
	leal	110(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$LC8, (%esp)
	call	_printf
L15:
	addl	$1, 120(%esp)
L13:
	cmpl	$2, 120(%esp)
	jle	L16
	movl	$1, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE30:
	.ident	"GCC: (MinGW.org GCC Build-2) 9.2.0"
	.def	_puts;	.scl	2;	.type	32;	.endef
	.def	_scanf;	.scl	2;	.type	32;	.endef
	.def	_strcmp;	.scl	2;	.type	32;	.endef
	.def	_printf;	.scl	2;	.type	32;	.endef
