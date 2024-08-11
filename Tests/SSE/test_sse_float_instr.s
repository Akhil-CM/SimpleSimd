	.file	"test_sse_float_instr.cpp"
	.text
	.section	.rodata
	.type	_ZStL19piecewise_construct, @object
	.size	_ZStL19piecewise_construct, 1
_ZStL19piecewise_construct:
	.zero	1
#APP
	.globl _ZSt21ios_base_library_initv
#NO_APP
	.section	.text._ZN3KFP4SIMD6Detail9type_castIDv4_fDv2_xEET_RKT0_,"axG",@progbits,_ZN3KFP4SIMD6Detail9type_castIDv4_fDv2_xEET_RKT0_,comdat
	.weak	_ZN3KFP4SIMD6Detail9type_castIDv4_fDv2_xEET_RKT0_
	.type	_ZN3KFP4SIMD6Detail9type_castIDv4_fDv2_xEET_RKT0_, @function
_ZN3KFP4SIMD6Detail9type_castIDv4_fDv2_xEET_RKT0_:
.LFB7715:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movdqa	(%rax), %xmm0
	movaps	%xmm0, -16(%rbp)
	movaps	-16(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7715:
	.size	_ZN3KFP4SIMD6Detail9type_castIDv4_fDv2_xEET_RKT0_, .-_ZN3KFP4SIMD6Detail9type_castIDv4_fDv2_xEET_RKT0_
	.section	.text._ZN3KFP4SIMD6Detail7ANDBitsIDv4_fS3_S3_EET_RKT0_RKT1_,"axG",@progbits,_ZN3KFP4SIMD6Detail7ANDBitsIDv4_fS3_S3_EET_RKT0_RKT1_,comdat
	.weak	_ZN3KFP4SIMD6Detail7ANDBitsIDv4_fS3_S3_EET_RKT0_RKT1_
	.type	_ZN3KFP4SIMD6Detail7ANDBitsIDv4_fS3_S3_EET_RKT0_RKT1_, @function
_ZN3KFP4SIMD6Detail7ANDBitsIDv4_fS3_S3_EET_RKT0_RKT1_:
.LFB7724:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	-48(%rbp), %rax
	movaps	(%rax), %xmm0
	movq	-40(%rbp), %rax
	movaps	(%rax), %xmm1
	movaps	%xmm1, -32(%rbp)
	movaps	%xmm0, -16(%rbp)
	movaps	-32(%rbp), %xmm0
	andps	-16(%rbp), %xmm0
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7724:
	.size	_ZN3KFP4SIMD6Detail7ANDBitsIDv4_fS3_S3_EET_RKT0_RKT1_, .-_ZN3KFP4SIMD6Detail7ANDBitsIDv4_fS3_S3_EET_RKT0_RKT1_
	.section	.text._ZN3KFP4SIMD6Detail3absIDv4_fS3_EET_RKT0_,"axG",@progbits,_ZN3KFP4SIMD6Detail3absIDv4_fS3_EET_RKT0_,comdat
	.weak	_ZN3KFP4SIMD6Detail3absIDv4_fS3_EET_RKT0_
	.type	_ZN3KFP4SIMD6Detail3absIDv4_fS3_EET_RKT0_, @function
_ZN3KFP4SIMD6Detail3absIDv4_fS3_EET_RKT0_:
.LFB7749:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -56(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	call	_ZN3KFP4SIMD6Detail7getMaskILNS1_4MASKE0EEEDv2_xv
	movaps	%xmm0, -48(%rbp)
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN3KFP4SIMD6Detail9type_castIDv4_fDv2_xEET_RKT0_
	movaps	%xmm0, -32(%rbp)
	leaq	-32(%rbp), %rdx
	movq	-56(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_ZN3KFP4SIMD6Detail7ANDBitsIDv4_fS3_S3_EET_RKT0_RKT1_
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L9
	call	__stack_chk_fail@PLT
.L9:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7749:
	.size	_ZN3KFP4SIMD6Detail3absIDv4_fS3_EET_RKT0_, .-_ZN3KFP4SIMD6Detail3absIDv4_fS3_EET_RKT0_
	.text
	.globl	_Z6getABSN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEE
	.type	_Z6getABSN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEE, @function
_Z6getABSN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEE:
.LFB8100:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-24(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_ZN3KFP4SIMD3absERKNS0_13SimdClassBaseIfLNS0_3ABIE1EEE
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L12
	call	__stack_chk_fail@PLT
.L12:
	movq	-24(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8100:
	.size	_Z6getABSN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEE, .-_Z6getABSN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEE
	.section	.rodata
.LC0:
	.string	"\n\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8101:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	$4, %esi
	leaq	_ZSt4cout(%rip), %rax
	movq	%rax, %rdi
	call	_ZNSolsEi@PLT
	movq	%rax, %rdx
	leaq	.LC0(%rip), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@PLT
	movss	.LC1(%rip), %xmm0
	movss	%xmm0, -20(%rbp)
	movss	-20(%rbp), %xmm0
	shufps	$0, %xmm0, %xmm0
	movaps	%xmm0, -16(%rbp)
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8101:
	.size	main, .-main
	.section	.text._ZN3KFP4SIMD6Detail7getMaskILNS1_4MASKE0EEEDv2_xv,"axG",@progbits,_ZN3KFP4SIMD6Detail7getMaskILNS1_4MASKE0EEEDv2_xv,comdat
	.weak	_ZN3KFP4SIMD6Detail7getMaskILNS1_4MASKE0EEEDv2_xv
	.type	_ZN3KFP4SIMD6Detail7getMaskILNS1_4MASKE0EEEDv2_xv, @function
_ZN3KFP4SIMD6Detail7getMaskILNS1_4MASKE0EEEDv2_xv:
.LFB8327:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$2147483647, -20(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, -16(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, -8(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, -4(%rbp)
	movl	-16(%rbp), %eax
	movd	-12(%rbp), %xmm0
	movdqa	%xmm0, %xmm1
	pinsrd	$1, %eax, %xmm1
	movl	-8(%rbp), %eax
	movd	-4(%rbp), %xmm0
	pinsrd	$1, %eax, %xmm0
	punpcklqdq	%xmm1, %xmm0
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8327:
	.size	_ZN3KFP4SIMD6Detail7getMaskILNS1_4MASKE0EEEDv2_xv, .-_ZN3KFP4SIMD6Detail7getMaskILNS1_4MASKE0EEEDv2_xv
	.section	.text._ZN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEC2IvLPv0EEERKDv4_f,"axG",@progbits,_ZN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEC5IvLPv0EEERKDv4_f,comdat
	.align 2
	.weak	_ZN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEC2IvLPv0EEERKDv4_f
	.type	_ZN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEC2IvLPv0EEERKDv4_f, @function
_ZN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEC2IvLPv0EEERKDv4_f:
.LFB8342:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rax
	movaps	(%rax), %xmm0
	movq	-8(%rbp), %rax
	movaps	%xmm0, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8342:
	.size	_ZN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEC2IvLPv0EEERKDv4_f, .-_ZN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEC2IvLPv0EEERKDv4_f
	.weak	_ZN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEC1IvLPv0EEERKDv4_f
	.set	_ZN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEC1IvLPv0EEERKDv4_f,_ZN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEC2IvLPv0EEERKDv4_f
	.section	.text._ZN3KFP4SIMD3absERKNS0_13SimdClassBaseIfLNS0_3ABIE1EEE,"axG",@progbits,_ZN3KFP4SIMD3absERKNS0_13SimdClassBaseIfLNS0_3ABIE1EEE,comdat
	.weak	_ZN3KFP4SIMD3absERKNS0_13SimdClassBaseIfLNS0_3ABIE1EEE
	.type	_ZN3KFP4SIMD3absERKNS0_13SimdClassBaseIfLNS0_3ABIE1EEE, @function
_ZN3KFP4SIMD3absERKNS0_13SimdClassBaseIfLNS0_3ABIE1EEE:
.LFB8345:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN3KFP4SIMD6Detail3absIDv4_fS3_EET_RKT0_
	movaps	%xmm0, -32(%rbp)
	leaq	-32(%rbp), %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_ZN3KFP4SIMD13SimdClassBaseIfLNS0_3ABIE1EEC1IvLPv0EEERKDv4_f
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L23
	call	__stack_chk_fail@PLT
.L23:
	movq	-40(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8345:
	.size	_ZN3KFP4SIMD3absERKNS0_13SimdClassBaseIfLNS0_3ABIE1EEE, .-_ZN3KFP4SIMD3absERKNS0_13SimdClassBaseIfLNS0_3ABIE1EEE
	.section	.rodata
	.align 4
.LC1:
	.long	1092616192
	.ident	"GCC: (GNU) 14.2.1 20240805"
	.section	.note.GNU-stack,"",@progbits
