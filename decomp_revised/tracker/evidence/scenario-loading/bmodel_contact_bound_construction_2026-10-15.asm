# Hash-pinned retail x86 disassembly excerpts
# FDNYFirefighter.exe SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
# Generated with objdump -d --no-show-raw-insn and bounded address ranges.


inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  43b7a0:      	subl	$108, %esp
  43b7a3:      	pushl	%ebx
  43b7a4:      	pushl	%ebp
  43b7a5:      	pushl	%esi
  43b7a6:      	movl	%ecx, %ebp
  43b7a8:      	pushl	%edi
  43b7a9:      	leal	48(%esp), %ecx
  43b7ad:      	movl	%ebp, 20(%esp)
  43b7b1:      	calll	0x481950 <.text+0x80950>
  43b7b6:      	pushl	$1232348144             # imm = 0x497423F0
  43b7bb:      	pushl	$1232348144             # imm = 0x497423F0
  43b7c0:      	pushl	$1232348144             # imm = 0x497423F0
  43b7c5:      	leal	36(%esp), %eax
  43b7c9:      	leal	576(%ebp), %esi
  43b7cf:      	pushl	%eax
  43b7d0:      	movl	%esi, %ecx
  43b7d2:      	calll	0x42d280 <.text+0x2c280>
  43b7d7:      	pushl	$3379831792             # imm = 0xC97423F0
  43b7dc:      	pushl	$3379831792             # imm = 0xC97423F0
  43b7e1:      	pushl	$3379831792             # imm = 0xC97423F0
  43b7e6:      	leal	36(%esp), %ecx
  43b7ea:      	leal	588(%ebp), %ebx
  43b7f0:      	pushl	%ecx
  43b7f1:      	movl	%ebx, %ecx
  43b7f3:      	calll	0x42d280 <.text+0x2c280>
  43b7f8:      	movl	452(%ebp), %eax
  43b7fe:      	xorl	%edi, %edi
  43b800:      	testl	%eax, %eax
  43b802:      	movl	%edi, 16(%esp)
  43b806:      	jle	0x43b9db <.text+0x3a9db>
  43b80c:      	leal	(%esp), %esp
  43b810:      	testl	%edi, %edi
  43b812:      	jl	0x43b81c <.text+0x3a81c>
  43b814:      	cmpl	452(%ebp), %edi
  43b81a:      	jl	0x43b85a <.text+0x3a85a>
  43b81c:      	xorl	%eax, %eax
  43b81e:      	leal	24(%esp), %edx
  43b822:      	movl	$6, %ecx
  43b827:      	leal	24(%esp), %edi
  43b82b:      	rep		stosl	%eax, %es:(%edi)
  43b82d:      	pushl	%edx
  43b82e:      	movl	$1, 28(%esp)
  43b836:      	movl	$5102532, 32(%esp)      # imm = 0x4DDBC4
  43b83e:      	movl	$5101660, 36(%esp)      # imm = 0x4DD85C
  43b846:      	movl	$461, 40(%esp)          # imm = 0x1CD
  43b84e:      	calll	0x422060 <.text+0x21060>
  43b853:      	movl	20(%esp), %edi
  43b857:      	addl	$4, %esp
  43b85a:      	movl	5257848, %eax
  43b85f:      	movl	4(%eax), %eax
  43b862:      	movb	352(%eax), %cl
  43b868:      	testb	%cl, %cl
  43b86a:      	je	0x43b873 <.text+0x3a873>
  43b86c:      	addl	$180, %eax
  43b871:      	jmp	0x43b876 <.text+0x3a876>
  43b873:      	addl	$8, %eax
  43b876:      	movl	448(%ebp), %ecx
  43b87c:      	movl	(%ecx,%edi,4), %edx
  43b87f:      	movl	120(%eax), %eax
  43b882:      	pushl	%edx
  43b883:      	pushl	%eax
  43b884:      	leal	56(%esp), %ecx
  43b888:      	calll	0x4818c0 <.text+0x808c0>
  43b88d:      	xorl	%edx, %edx
  43b88f:      	nop
  43b890:      	movl	%edx, %eax
  43b892:      	subl	$0, %eax
  43b895:      	je	0x43b8b0 <.text+0x3a8b0>
  43b897:      	decl	%eax
  43b898:      	je	0x43b8aa <.text+0x3a8aa>
  43b89a:      	decl	%eax
  43b89b:      	je	0x43b8a4 <.text+0x3a8a4>
  43b89d:      	movl	$5253996, %ecx          # imm = 0x502B6C
  43b8a2:      	jmp	0x43b8b4 <.text+0x3a8b4>
  43b8a4:      	leal	56(%esp), %ecx
  43b8a8:      	jmp	0x43b8b4 <.text+0x3a8b4>
  43b8aa:      	leal	52(%esp), %ecx
  43b8ae:      	jmp	0x43b8b4 <.text+0x3a8b4>
  43b8b0:      	leal	48(%esp), %ecx
  43b8b4:      	movl	%edx, %eax
  43b8b6:      	subl	$0, %eax
  43b8b9:      	je	0x43b8d2 <.text+0x3a8d2>
  43b8bb:      	decl	%eax
  43b8bc:      	je	0x43b8cd <.text+0x3a8cd>
  43b8be:      	decl	%eax
  43b8bf:      	je	0x43b8c8 <.text+0x3a8c8>
  43b8c1:      	movl	$5253996, %eax          # imm = 0x502B6C
  43b8c6:      	jmp	0x43b8d4 <.text+0x3a8d4>
  43b8c8:      	leal	8(%esi), %eax
  43b8cb:      	jmp	0x43b8d4 <.text+0x3a8d4>
  43b8cd:      	leal	4(%esi), %eax
  43b8d0:      	jmp	0x43b8d4 <.text+0x3a8d4>
  43b8d2:      	movl	%esi, %eax
  43b8d4:      	flds	(%ecx)
  43b8d6:      	fcomps	(%eax)
  43b8d8:      	fnstsw	%ax
  43b8da:      	testb	$5, %ah
  43b8dd:      	jp	0x43b927 <.text+0x3a927>
  43b8df:      	movl	%edx, %eax
  43b8e1:      	subl	$0, %eax
  43b8e4:      	je	0x43b8ff <.text+0x3a8ff>
  43b8e6:      	decl	%eax
  43b8e7:      	je	0x43b8f9 <.text+0x3a8f9>
  43b8e9:      	decl	%eax
  43b8ea:      	je	0x43b8f3 <.text+0x3a8f3>
  43b8ec:      	movl	$5253996, %ecx          # imm = 0x502B6C
  43b8f1:      	jmp	0x43b903 <.text+0x3a903>
  43b8f3:      	leal	56(%esp), %ecx
  43b8f7:      	jmp	0x43b903 <.text+0x3a903>
  43b8f9:      	leal	52(%esp), %ecx
  43b8fd:      	jmp	0x43b903 <.text+0x3a903>
  43b8ff:      	leal	48(%esp), %ecx
  43b903:      	movl	%edx, %eax
  43b905:      	subl	$0, %eax
  43b908:      	je	0x43b921 <.text+0x3a921>
  43b90a:      	decl	%eax
  43b90b:      	je	0x43b91c <.text+0x3a91c>
  43b90d:      	decl	%eax
  43b90e:      	je	0x43b917 <.text+0x3a917>
  43b910:      	movl	$5253996, %eax          # imm = 0x502B6C
  43b915:      	jmp	0x43b923 <.text+0x3a923>
  43b917:      	leal	8(%esi), %eax
  43b91a:      	jmp	0x43b923 <.text+0x3a923>
  43b91c:      	leal	4(%esi), %eax
  43b91f:      	jmp	0x43b923 <.text+0x3a923>
  43b921:      	movl	%esi, %eax
  43b923:      	movl	(%ecx), %ecx
  43b925:      	movl	%ecx, (%eax)
  43b927:      	movl	%edx, %eax
  43b929:      	subl	$0, %eax
  43b92c:      	je	0x43b947 <.text+0x3a947>
  43b92e:      	decl	%eax
  43b92f:      	je	0x43b941 <.text+0x3a941>
  43b931:      	decl	%eax
  43b932:      	je	0x43b93b <.text+0x3a93b>
  43b934:      	movl	$5253996, %ecx          # imm = 0x502B6C
  43b939:      	jmp	0x43b94b <.text+0x3a94b>
  43b93b:      	leal	68(%esp), %ecx
  43b93f:      	jmp	0x43b94b <.text+0x3a94b>
  43b941:      	leal	64(%esp), %ecx
  43b945:      	jmp	0x43b94b <.text+0x3a94b>
  43b947:      	leal	60(%esp), %ecx
  43b94b:      	movl	%edx, %eax
  43b94d:      	subl	$0, %eax
  43b950:      	je	0x43b969 <.text+0x3a969>
  43b952:      	decl	%eax
  43b953:      	je	0x43b964 <.text+0x3a964>
  43b955:      	decl	%eax
  43b956:      	je	0x43b95f <.text+0x3a95f>
  43b958:      	movl	$5253996, %eax          # imm = 0x502B6C
  43b95d:      	jmp	0x43b96b <.text+0x3a96b>
  43b95f:      	leal	8(%ebx), %eax
  43b962:      	jmp	0x43b96b <.text+0x3a96b>
  43b964:      	leal	4(%ebx), %eax
  43b967:      	jmp	0x43b96b <.text+0x3a96b>
  43b969:      	movl	%ebx, %eax
  43b96b:      	flds	(%ecx)
  43b96d:      	fcomps	(%eax)
  43b96f:      	fnstsw	%ax
  43b971:      	testb	$65, %ah
  43b974:      	jne	0x43b9be <.text+0x3a9be>
  43b976:      	movl	%edx, %eax
  43b978:      	subl	$0, %eax
  43b97b:      	je	0x43b996 <.text+0x3a996>
  43b97d:      	decl	%eax
  43b97e:      	je	0x43b990 <.text+0x3a990>
  43b980:      	decl	%eax
  43b981:      	je	0x43b98a <.text+0x3a98a>
  43b983:      	movl	$5253996, %ecx          # imm = 0x502B6C
  43b988:      	jmp	0x43b99a <.text+0x3a99a>
  43b98a:      	leal	68(%esp), %ecx
  43b98e:      	jmp	0x43b99a <.text+0x3a99a>
  43b990:      	leal	64(%esp), %ecx
  43b994:      	jmp	0x43b99a <.text+0x3a99a>
  43b996:      	leal	60(%esp), %ecx
  43b99a:      	movl	%edx, %eax
  43b99c:      	subl	$0, %eax
  43b99f:      	je	0x43b9b8 <.text+0x3a9b8>
  43b9a1:      	decl	%eax
  43b9a2:      	je	0x43b9b3 <.text+0x3a9b3>
  43b9a4:      	decl	%eax
  43b9a5:      	je	0x43b9ae <.text+0x3a9ae>
  43b9a7:      	movl	$5253996, %eax          # imm = 0x502B6C
  43b9ac:      	jmp	0x43b9ba <.text+0x3a9ba>
  43b9ae:      	leal	8(%ebx), %eax
  43b9b1:      	jmp	0x43b9ba <.text+0x3a9ba>
  43b9b3:      	leal	4(%ebx), %eax
  43b9b6:      	jmp	0x43b9ba <.text+0x3a9ba>
  43b9b8:      	movl	%ebx, %eax
  43b9ba:      	movl	(%ecx), %ecx
  43b9bc:      	movl	%ecx, (%eax)
  43b9be:      	incl	%edx
  43b9bf:      	cmpl	$3, %edx
  43b9c2:      	jl	0x43b890 <.text+0x3a890>
  43b9c8:      	movl	452(%ebp), %eax
  43b9ce:      	incl	%edi
  43b9cf:      	cmpl	%eax, %edi
  43b9d1:      	movl	%edi, 16(%esp)
  43b9d5:      	jl	0x43b810 <.text+0x3a810>
  43b9db:      	leal	72(%esp), %edx
  43b9df:      	pushl	%edx
  43b9e0:      	movl	%esi, %ecx
  43b9e2:      	calll	0x4819a0 <.text+0x809a0>
  43b9e7:      	leal	240(%ebp), %ecx
  43b9ed:      	movl	%ecx, %edi
  43b9ef:      	movl	%eax, %edx
  43b9f1:      	movl	(%edx), %ebp
  43b9f3:      	movl	%ebp, (%edi)
  43b9f5:      	movl	4(%edx), %ebp
  43b9f8:      	movl	%ebp, 4(%edi)
  43b9fb:      	movl	8(%edx), %edx
  43b9fe:      	movl	%edx, 8(%edi)
  43ba01:      	movl	12(%eax), %edx
  43ba04:      	movl	%edx, 12(%ecx)
  43ba07:      	leal	16(%ecx), %edi
  43ba0a:      	leal	16(%eax), %edx
  43ba0d:      	movl	(%edx), %ebp
  43ba0f:      	movl	%ebp, (%edi)
  43ba11:      	movl	4(%edx), %ebp
  43ba14:      	movl	%ebp, 4(%edi)
  43ba17:      	movl	8(%edx), %edx
  43ba1a:      	movl	%edx, 8(%edi)
  43ba1d:      	leal	28(%ecx), %edi
  43ba20:      	leal	28(%eax), %edx
  43ba23:      	movl	(%edx), %ebp
  43ba25:      	movl	%ebp, (%edi)
  43ba27:      	movl	4(%edx), %ebp
  43ba2a:      	movl	%ebp, 4(%edi)
  43ba2d:      	movl	8(%edx), %edx
  43ba30:      	movl	%edx, 8(%edi)
  43ba33:      	addl	$40, %ecx
  43ba36:      	addl	$40, %eax
  43ba39:      	movl	(%eax), %edx
  43ba3b:      	movl	%edx, (%ecx)
  43ba3d:      	movl	4(%eax), %edx
  43ba40:      	movl	%edx, 4(%ecx)
  43ba43:      	movl	8(%eax), %eax
  43ba46:      	movl	%eax, 8(%ecx)
  43ba49:      	movl	(%esi), %edx
  43ba4b:      	movl	20(%esp), %eax
  43ba4f:      	leal	632(%eax), %ecx
  43ba55:      	movl	%edx, (%ecx)
  43ba57:      	movl	4(%esi), %edx
  43ba5a:      	movl	%edx, 4(%ecx)
  43ba5d:      	movl	8(%esi), %edx
  43ba60:      	movl	%edx, 8(%ecx)
  43ba63:      	movl	(%ebx), %edx
  43ba65:      	leal	644(%eax), %ecx
  43ba6b:      	movl	%edx, (%ecx)
  43ba6d:      	movl	4(%ebx), %edx
  43ba70:      	movl	%edx, 4(%ecx)
  43ba73:      	movl	8(%ebx), %edx
  43ba76:      	movl	%edx, 8(%ecx)
  43ba79:      	pushl	$1107296256             # imm = 0x42000000
  43ba7e:      	leal	608(%eax), %ecx
  43ba84:      	calll	0x43b470 <.text+0x3a470>
  43ba89:      	popl	%edi
  43ba8a:      	popl	%esi
  43ba8b:      	popl	%ebp
  43ba8c:      	popl	%ebx
  43ba8d:      	addl	$108, %esp


inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  4817a0:      	flds	(%ecx)
  4817a2:      	movl	4(%esp), %edx
  4817a6:      	fsubs	16(%esp)
  4817aa:      	pushl	%esi
  4817ab:      	flds	(%edx)
  4817ad:      	pushl	%edi
  4817ae:      	movl	20(%esp), %edi
  4817b2:      	fadds	(%edi)
  4817b4:      	fcompp
  4817b6:      	fnstsw	%ax
  4817b8:      	testb	$5, %ah
  4817bb:      	jnp	0x481834 <.text+0x80834>
  4817bd:      	flds	24(%esp)
  4817c1:      	movl	16(%esp), %esi
  4817c5:      	fadds	12(%ecx)
  4817c8:      	flds	(%edx)
  4817ca:      	fadds	(%esi)
  4817cc:      	fcompp
  4817ce:      	fnstsw	%ax
  4817d0:      	testb	$65, %ah
  4817d3:      	je	0x481834 <.text+0x80834>
  4817d5:      	flds	4(%ecx)
  4817d8:      	fsubs	24(%esp)
  4817dc:      	flds	4(%edi)
  4817df:      	fadds	4(%edx)
  4817e2:      	fcompp
  4817e4:      	fnstsw	%ax
  4817e6:      	testb	$5, %ah
  4817e9:      	jnp	0x481834 <.text+0x80834>
  4817eb:      	flds	24(%esp)
  4817ef:      	fadds	16(%ecx)
  4817f2:      	flds	4(%esi)
  4817f5:      	fadds	4(%edx)
  4817f8:      	fcompp
  4817fa:      	fnstsw	%ax
  4817fc:      	testb	$65, %ah
  4817ff:      	je	0x481834 <.text+0x80834>
  481801:      	flds	8(%ecx)
  481804:      	fsubs	24(%esp)
  481808:      	flds	8(%edi)
  48180b:      	fadds	8(%edx)
  48180e:      	fcompp
  481810:      	fnstsw	%ax
  481812:      	testb	$5, %ah
  481815:      	jnp	0x481834 <.text+0x80834>
  481817:      	flds	24(%esp)
  48181b:      	fadds	20(%ecx)
  48181e:      	flds	8(%esi)
  481821:      	fadds	8(%edx)
  481824:      	fcompp
  481826:      	fnstsw	%ax
  481828:      	testb	$65, %ah
  48182b:      	je	0x481834 <.text+0x80834>
  48182d:      	popl	%edi
  48182e:      	movb	$1, %al
  481830:      	popl	%esi
  481831:      	retl	$16
  481834:      	popl	%edi
  481835:      	xorb	%al, %al
  481837:      	popl	%esi


inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  4818c0:      	movl	8(%esp), %edx
  4818c4:      	pushl	%esi
  4818c5:      	leal	140(%edx), %eax
  4818cb:      	pushl	%edi
  4818cc:      	movl	(%eax), %edi
  4818ce:      	movl	%ecx, %esi
  4818d0:      	movl	%edi, (%esi)
  4818d2:      	movl	4(%eax), %edi
  4818d5:      	movl	%edi, 4(%esi)
  4818d8:      	movl	8(%eax), %eax
  4818db:      	movl	%eax, 8(%esi)
  4818de:      	leal	12(%ecx), %eax
  4818e1:      	addl	$152, %edx
  4818e7:      	movl	(%edx), %edi
  4818e9:      	movl	%eax, %esi
  4818eb:      	movl	%edi, (%esi)
  4818ed:      	movl	4(%edx), %edi
  4818f0:      	movl	%edi, 4(%esi)
  4818f3:      	movl	8(%edx), %edx
  4818f6:      	movl	%edx, 8(%esi)
  4818f9:      	flds	(%ecx)
  4818fb:      	fadds	5122344
  481901:      	popl	%edi
  481902:      	popl	%esi
  481903:      	fstps	(%ecx)
  481905:      	flds	4(%ecx)
  481908:      	fadds	5122344
  48190e:      	fstps	4(%ecx)
  481911:      	flds	8(%ecx)
  481914:      	fadds	5122344
  48191a:      	fstps	8(%ecx)
  48191d:      	flds	(%eax)
  48191f:      	fsubs	5122344
  481925:      	fstps	(%eax)
  481927:      	flds	4(%eax)
  48192a:      	fsubs	5122344
  481930:      	fstps	4(%eax)
  481933:      	flds	8(%eax)
  481936:      	fsubs	5122344
  48193c:      	fstps	8(%eax)
  48193f:      	retl	$8


inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  4819a0:      	subl	$76, %esp
  4819a3:      	pushl	%esi
  4819a4:      	movl	%ecx, %esi
  4819a6:      	pushl	%edi
  4819a7:      	leal	32(%esp), %ecx
  4819ab:      	calll	0x43b440 <.text+0x3a440>
  4819b0:      	pushl	$1056964608             # imm = 0x3F000000
  4819b5:      	leal	12(%esi), %edi
  4819b8:      	pushl	%edi
  4819b9:      	leal	16(%esp), %eax
  4819bd:      	pushl	%esi
  4819be:      	pushl	%eax
  4819bf:      	calll	0x42d050 <.text+0x2c050>
  4819c4:      	addl	$12, %esp
  4819c7:      	pushl	%eax
  4819c8:      	leal	28(%esp), %ecx
  4819cc:      	pushl	%ecx
  4819cd:      	calll	0x446390 <.text+0x45390>
  4819d2:      	movl	(%eax), %edx
  4819d4:      	movl	4(%eax), %ecx
  4819d7:      	movl	%edx, 44(%esp)
  4819db:      	movl	8(%eax), %edx
  4819de:      	leal	44(%esp), %eax
  4819e2:      	pushl	%eax
  4819e3:      	movl	%ecx, 52(%esp)
  4819e7:      	leal	36(%esp), %ecx
  4819eb:      	pushl	%esi
  4819ec:      	pushl	%ecx
  4819ed:      	movl	%edx, 64(%esp)
  4819f1:      	calll	0x43b2d0 <.text+0x3a2d0>
  4819f6:      	movl	(%eax), %edx
  4819f8:      	movl	4(%eax), %ecx
  4819fb:      	movl	%edx, 72(%esp)
  4819ff:      	movl	8(%eax), %edx
  481a02:      	leal	56(%esp), %eax
  481a06:      	pushl	%eax
  481a07:      	movl	%ecx, 80(%esp)
  481a0b:      	leal	48(%esp), %ecx
  481a0f:      	pushl	%edi
  481a10:      	pushl	%ecx
  481a11:      	movl	%edx, 92(%esp)
  481a15:      	calll	0x43b2d0 <.text+0x3a2d0>
  481a1a:      	movl	(%eax), %edx
  481a1c:      	movl	4(%eax), %ecx
  481a1f:      	movl	124(%esp), %esi
  481a23:      	movl	%edx, 96(%esp)
  481a27:      	movl	8(%eax), %edx
  481a2a:      	addl	$36, %esp
  481a2d:      	leal	32(%esp), %eax
  481a31:      	movl	%ecx, 64(%esp)
  481a35:      	pushl	%eax
  481a36:      	movl	%esi, %ecx
  481a38:      	movl	%edx, 72(%esp)
  481a3c:      	calll	0x43b510 <.text+0x3a510>
  481a41:      	popl	%edi
  481a42:      	movl	%esi, %eax
  481a44:      	popl	%esi
  481a45:      	addl	$76, %esp


inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  43b470:      	flds	24(%ecx)
  43b473:      	fsubs	4(%esp)
  43b477:      	fstps	24(%ecx)
  43b47a:      	flds	28(%ecx)
  43b47d:      	fsubs	4(%esp)
  43b481:      	fstps	28(%ecx)
  43b484:      	flds	32(%ecx)
  43b487:      	fsubs	4(%esp)
  43b48b:      	fstps	32(%ecx)
  43b48e:      	flds	4(%esp)
  43b492:      	fadds	36(%ecx)
  43b495:      	fstps	36(%ecx)
  43b498:      	flds	4(%esp)
  43b49c:      	fadds	40(%ecx)
  43b49f:      	fstps	40(%ecx)
  43b4a2:      	flds	4(%esp)
  43b4a6:      	fadds	44(%ecx)
  43b4a9:      	fstps	44(%ecx)
  43b4ac:      	retl	$4


inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  43b2d0:      	subl	$12, %esp
  43b2d3:      	movl	20(%esp), %eax
  43b2d7:      	flds	(%eax)
  43b2d9:      	movl	24(%esp), %ecx
  43b2dd:      	fsubs	(%ecx)
  43b2df:      	fstps	(%esp)
  43b2e3:      	movl	(%esp), %edx
  43b2e7:      	flds	4(%eax)
  43b2ea:      	fsubs	4(%ecx)
  43b2ed:      	fstps	4(%esp)
  43b2f1:      	flds	8(%eax)
  43b2f4:      	movl	16(%esp), %eax
  43b2f8:      	fsubs	8(%ecx)
  43b2fb:      	movl	%eax, %ecx
  43b2fd:      	movl	%edx, (%ecx)
  43b2ff:      	movl	4(%esp), %edx
  43b303:      	movl	%edx, 4(%ecx)
  43b306:      	fstps	8(%esp)
  43b30a:      	movl	8(%esp), %edx
  43b30e:      	movl	%edx, 8(%ecx)
  43b311:      	addl	$12, %esp
  43b314:      	retl


inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  43b440:      	movl	%ecx, %eax
  43b442:      	xorl	%ecx, %ecx
  43b444:      	movl	%ecx, (%eax)
  43b446:      	movl	%ecx, 4(%eax)
  43b449:      	movl	%ecx, 8(%eax)
  43b44c:      	movl	%ecx, 16(%eax)
  43b44f:      	movl	%ecx, 20(%eax)
  43b452:      	movl	%ecx, 24(%eax)
  43b455:      	movl	%ecx, 28(%eax)
  43b458:      	movl	%ecx, 32(%eax)
  43b45b:      	movl	%ecx, 36(%eax)
  43b45e:      	movl	%ecx, 40(%eax)
  43b461:      	movl	%ecx, 44(%eax)
  43b464:      	movl	%ecx, 48(%eax)
  43b467:      	movl	%ecx, 12(%eax)
  43b46a:      	retl


inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  43b510:      	pushl	%esi
  43b511:      	movl	%ecx, %eax
  43b513:      	movl	8(%esp), %ecx
  43b517:      	pushl	%edi
  43b518:      	movl	%ecx, %edx
  43b51a:      	movl	(%edx), %edi
  43b51c:      	movl	%eax, %esi
  43b51e:      	movl	%edi, (%esi)
  43b520:      	movl	4(%edx), %edi
  43b523:      	movl	%edi, 4(%esi)
  43b526:      	movl	8(%edx), %edx
  43b529:      	movl	%edx, 8(%esi)
  43b52c:      	movl	12(%ecx), %edx
  43b52f:      	movl	%edx, 12(%eax)
  43b532:      	leal	16(%ecx), %edx
  43b535:      	movl	(%edx), %edi
  43b537:      	leal	16(%eax), %esi
  43b53a:      	movl	%edi, (%esi)
  43b53c:      	movl	4(%edx), %edi
  43b53f:      	movl	%edi, 4(%esi)
  43b542:      	movl	8(%edx), %edx
  43b545:      	movl	%edx, 8(%esi)
  43b548:      	leal	28(%ecx), %edx
  43b54b:      	movl	(%edx), %edi
  43b54d:      	leal	28(%eax), %esi
  43b550:      	movl	%edi, (%esi)
  43b552:      	movl	4(%edx), %edi
  43b555:      	movl	%edi, 4(%esi)
  43b558:      	movl	8(%edx), %edx
  43b55b:      	movl	%edx, 8(%esi)
  43b55e:      	addl	$40, %ecx
  43b561:      	movl	(%ecx), %esi
  43b563:      	leal	40(%eax), %edx
  43b566:      	movl	%esi, (%edx)
  43b568:      	movl	4(%ecx), %esi
  43b56b:      	movl	%esi, 4(%edx)
  43b56e:      	movl	8(%ecx), %ecx
  43b571:      	popl	%edi
  43b572:      	movl	%ecx, 8(%edx)
  43b575:      	popl	%esi
  43b576:      	retl	$4


inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  42d050:      	subl	$12, %esp
  42d053:      	movl	20(%esp), %eax
  42d057:      	flds	(%eax)
  42d059:      	movl	24(%esp), %ecx
  42d05d:      	fadds	(%ecx)
  42d05f:      	fstps	(%esp)
  42d063:      	movl	(%esp), %edx
  42d067:      	flds	4(%eax)
  42d06a:      	fadds	4(%ecx)
  42d06d:      	fstps	4(%esp)
  42d071:      	flds	8(%eax)
  42d074:      	movl	16(%esp), %eax
  42d078:      	fadds	8(%ecx)
  42d07b:      	movl	%eax, %ecx
  42d07d:      	movl	%edx, (%ecx)
  42d07f:      	movl	4(%esp), %edx
  42d083:      	movl	%edx, 4(%ecx)
  42d086:      	fstps	8(%esp)
  42d08a:      	movl	8(%esp), %edx
  42d08e:      	movl	%edx, 8(%ecx)
  42d091:      	addl	$12, %esp
  42d094:      	retl


inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  446390:      	subl	$12, %esp
  446393:      	movl	20(%esp), %eax
  446397:      	movl	(%eax), %ecx
  446399:      	movl	4(%eax), %edx
  44639c:      	movl	8(%eax), %eax
  44639f:      	movl	%ecx, (%esp)
  4463a3:      	flds	(%esp)
  4463a7:      	fmuls	24(%esp)
  4463ab:      	movl	%edx, 4(%esp)
  4463af:      	movl	%eax, 8(%esp)
  4463b3:      	movl	16(%esp), %eax
  4463b7:      	fstps	(%esp)
  4463bb:      	movl	(%esp), %edx
  4463bf:      	flds	4(%esp)
  4463c3:      	movl	%eax, %ecx
  4463c5:      	fmuls	24(%esp)
  4463c9:      	movl	%edx, (%ecx)
  4463cb:      	fstps	4(%esp)
  4463cf:      	movl	4(%esp), %edx
  4463d3:      	flds	8(%esp)
  4463d7:      	movl	%edx, 4(%ecx)
  4463da:      	fmuls	24(%esp)
  4463de:      	fstps	8(%esp)
  4463e2:      	movl	8(%esp), %edx
  4463e6:      	movl	%edx, 8(%ecx)
  4463e9:      	addl	$12, %esp
  4463ec:      	retl
