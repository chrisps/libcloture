	.text
	.file	"util_test.cpp"
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:
	mov	eax, 1
	mov	ecx, dword ptr [rip + bbb]
	.align	16, 0x90
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	lea	edx, [rax - 1]
	xor	esi, esi
	#APP
	bt	ecx, edx
	adc	esi, esi

	#NO_APP
	xor	edi, edi
	#APP
	bt	edx, 2
	adc	edi, edi

	#NO_APP
	test	edi, esi
	jne	.LBB0_3
# BB#2:                                 #   in Loop: Header=BB0_1 Depth=1
	cmp	eax, 32
	lea	eax, [rax + 1]
                                        # kill: EAX<def> EAX<kill> RAX<def>
	jb	.LBB0_1
.LBB0_3:
	mov	eax, 1
	ret
.Ltmp0:
	.size	main, .Ltmp0-main
	.cfi_endproc

	.type	meme,@object            # @meme
	.bss
	.globl	meme
	.align	16
meme:
	.zero	256
	.size	meme, 256

	.type	bbb,@object             # @bbb
	.data
	.globl	bbb
	.align	4
bbb:
	.long	11                      # 0xb
	.size	bbb, 4


	.ident	"Ubuntu clang version 3.6.1-svn232753-1~exp1 (branches/release_36) (based on LLVM 3.6.1)"
	.section	".note.GNU-stack","",@progbits
