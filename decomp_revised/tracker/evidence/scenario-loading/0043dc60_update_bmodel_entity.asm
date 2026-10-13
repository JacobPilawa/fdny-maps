
inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  43dc60: 6a ff                        	push	-1
  43dc62: 64 a1 00 00 00 00            	mov	eax, dword ptr fs:[0]
  43dc68: 68 db ea 4c 00               	push	5040859
  43dc6d: 50                           	push	eax
  43dc6e: 64 89 25 00 00 00 00         	mov	dword ptr fs:[0], esp
  43dc75: 81 ec cc 00 00 00            	sub	esp, 204
  43dc7b: 53                           	push	ebx
  43dc7c: 55                           	push	ebp
  43dc7d: 56                           	push	esi
  43dc7e: 8b f1                        	mov	esi, ecx
  43dc80: 8b 86 a0 01 00 00            	mov	eax, dword ptr [esi + 416]
  43dc86: 83 f8 01                     	cmp	eax, 1
  43dc89: 57                           	push	edi
  43dc8a: 0f 8c ef 05 00 00            	jl	0x43e27f <.text+0x3d27f>
  43dc90: d9 86 b4 01 00 00            	fld	dword ptr [esi + 436]
  43dc96: 33 ff                        	xor	edi, edi
  43dc98: d8 a6 c4 01 00 00            	fsub	dword ptr [esi + 452]
  43dc9e: 89 7e 70                     	mov	dword ptr [esi + 112], edi
  43dca1: d9 e1                        	fabs
  43dca3: dc 1d 28 fa 4d 00            	fcomp	qword ptr [5110312]
  43dca9: df e0                        	fnstsw	ax
  43dcab: f6 c4 41                     	test	ah, 65
  43dcae: 75 04                        	jne	0x43dcb4 <.text+0x3ccb4>
  43dcb0: b0 01                        	mov	al, 1
  43dcb2: eb 02                        	jmp	0x43dcb6 <.text+0x3ccb6>
  43dcb4: 32 c0                        	xor	al, al
  43dcb6: 8d 6e dc                     	lea	ebp, [esi - 36]
  43dcb9: 50                           	push	eax
  43dcba: 8b cd                        	mov	ecx, ebp
  43dcbc: e8 df e4 ff ff               	call	0x43c1a0 <.text+0x3b1a0>
  43dcc1: a0 18 19 50 00               	mov	al, byte ptr [5249304]
  43dcc6: 84 c0                        	test	al, al
  43dcc8: 75 28                        	jne	0x43dcf2 <.text+0x3ccf2>
  43dcca: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43dcd1: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43dcdb: 89 3d 0c 19 50 00            	mov	dword ptr [5249292], edi
  43dce1: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43dceb: c6 05 14 19 50 00 00         	mov	byte ptr [5249300], 0
  43dcf2: 68 30 0b 4e 00               	push	5114672
  43dcf7: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43dcfb: c7 44 24 14 08 19 50 00      	mov	dword ptr [esp + 20], 5249288
  43dd03: e8 78 40 fc ff               	call	0x401d80 <.text+0xd80>
  43dd08: 8b 0d 78 3a 50 00            	mov	ecx, dword ptr [5257848]
  43dd0e: 8d 44 24 10                  	lea	eax, [esp + 16]
  43dd12: 50                           	push	eax
  43dd13: 89 bc 24 e8 00 00 00         	mov	dword ptr [esp + 232], edi
  43dd1a: e8 91 bf 05 00               	call	0x499cb0 <.text+0x98cb0>
  43dd1f: 8b c8                        	mov	ecx, eax
  43dd21: e8 2a a8 ff ff               	call	0x438550 <.text+0x37550>
  43dd26: 8a d8                        	mov	bl, al
  43dd28: 8b 44 24 10                  	mov	eax, dword ptr [esp + 16]
  43dd2c: 3d 08 19 50 00               	cmp	eax, 5249288
  43dd31: c7 84 24 e4 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 228], 4294967295
  43dd3c: 74 13                        	je	0x43dd51 <.text+0x3cd51>
  43dd3e: ff 08                        	dec	dword ptr [eax]
  43dd40: 8b 4c 24 10                  	mov	ecx, dword ptr [esp + 16]
  43dd44: 39 39                        	cmp	dword ptr [ecx], edi
  43dd46: 7f 09                        	jg	0x43dd51 <.text+0x3cd51>
  43dd48: 8d 4c 24 10                  	lea	ecx, [esp + 16]
  43dd4c: e8 6f 34 fc ff               	call	0x4011c0 <.text+0x1c0>
  43dd51: 84 db                        	test	bl, bl
  43dd53: 74 08                        	je	0x43dd5d <.text+0x3cd5d>
  43dd55: 57                           	push	edi
  43dd56: 8b cd                        	mov	ecx, ebp
  43dd58: e8 43 e4 ff ff               	call	0x43c1a0 <.text+0x3b1a0>
  43dd5d: 8d 46 dc                     	lea	eax, [esi - 36]
  43dd60: f7 d8                        	neg	eax
  43dd62: 1b c0                        	sbb	eax, eax
  43dd64: 8d be 3c 01 00 00            	lea	edi, [esi + 316]
  43dd6a: 23 c6                        	and	eax, esi
  43dd6c: 57                           	push	edi
  43dd6d: 50                           	push	eax
  43dd6e: 8d 8e 54 01 00 00            	lea	ecx, [esi + 340]
  43dd74: e8 c7 2a 03 00               	call	0x470840 <.text+0x6f840>
  43dd79: 33 db                        	xor	ebx, ebx
  43dd7b: 53                           	push	ebx
  43dd7c: 8b cf                        	mov	ecx, edi
  43dd7e: e8 7d 25 05 00               	call	0x490300 <.text+0x8f300>
  43dd83: 85 c0                        	test	eax, eax
  43dd85: 74 09                        	je	0x43dd90 <.text+0x3cd90>
  43dd87: c6 86 18 02 00 00 01         	mov	byte ptr [esi + 536], 1
  43dd8e: eb 13                        	jmp	0x43dda3 <.text+0x3cda3>
  43dd90: 53                           	push	ebx
  43dd91: 8b cf                        	mov	ecx, edi
  43dd93: e8 08 26 05 00               	call	0x4903a0 <.text+0x8f3a0>
  43dd98: 85 c0                        	test	eax, eax
  43dd9a: 74 07                        	je	0x43dda3 <.text+0x3cda3>
  43dd9c: c6 86 18 02 00 00 00         	mov	byte ptr [esi + 536], 0
  43dda3: 39 9e 34 02 00 00            	cmp	dword ptr [esi + 564], ebx
  43dda9: 74 20                        	je	0x43ddcb <.text+0x3cdcb>
  43ddab: 8a 86 18 02 00 00            	mov	al, byte ptr [esi + 536]
  43ddb1: 84 c0                        	test	al, al
  43ddb3: 8d 4e 7c                     	lea	ecx, [esi + 124]
  43ddb6: 74 07                        	je	0x43ddbf <.text+0x3cdbf>
  43ddb8: e8 93 b2 01 00               	call	0x459050 <.text+0x58050>
  43ddbd: eb 05                        	jmp	0x43ddc4 <.text+0x3cdc4>
  43ddbf: e8 cc b2 01 00               	call	0x459090 <.text+0x58090>
  43ddc4: 8b cd                        	mov	ecx, ebp
  43ddc6: e8 65 f5 ff ff               	call	0x43d330 <.text+0x3c330>
  43ddcb: 8b 85 ec 00 00 00            	mov	eax, dword ptr [ebp + 236]
  43ddd1: 39 58 34                     	cmp	dword ptr [eax + 52], ebx
  43ddd4: 0f 84 fd 00 00 00            	je	0x43ded7 <.text+0x3ced7>
  43ddda: 39 58 38                     	cmp	dword ptr [eax + 56], ebx
  43dddd: 0f 85 f4 00 00 00            	jne	0x43ded7 <.text+0x3ced7>
  43dde3: d9 85 ec 01 00 00            	fld	dword ptr [ebp + 492]
  43dde9: dc 15 68 0b 4e 00            	fcom	qword ptr [5114728]
  43ddef: df e0                        	fnstsw	ax
  43ddf1: f6 c4 05                     	test	ah, 5
  43ddf4: 7a 13                        	jp	0x43de09 <.text+0x3ce09>
  43ddf6: dc 1d 60 0b 4e 00            	fcomp	qword ptr [5114720]
  43ddfc: df e0                        	fnstsw	ax
  43ddfe: f6 c4 41                     	test	ah, 65
  43de01: 0f 84 d0 00 00 00            	je	0x43ded7 <.text+0x3ced7>
  43de07: eb 02                        	jmp	0x43de0b <.text+0x3ce0b>
  43de09: dd d8                        	fstp	st(0)
  43de0b: 8a 86 18 02 00 00            	mov	al, byte ptr [esi + 536]
  43de11: 84 c0                        	test	al, al
  43de13: 0f 84 be 00 00 00            	je	0x43ded7 <.text+0x3ced7>
  43de19: 8b 15 78 3a 50 00            	mov	edx, dword ptr [5257848]
  43de1f: 8b 4a 30                     	mov	ecx, dword ptr [edx + 48]
  43de22: 8b 11                        	mov	edx, dword ptr [ecx]
  43de24: 8d 44 24 20                  	lea	eax, [esp + 32]
  43de28: 50                           	push	eax
  43de29: ff 52 18                     	call	dword ptr [edx + 24]
  43de2c: 8b c8                        	mov	ecx, eax
  43de2e: e8 3d 09 06 00               	call	0x49e770 <.text+0x9d770>
  43de33: a1 78 3a 50 00               	mov	eax, dword ptr [5257848]
  43de38: 8b 48 30                     	mov	ecx, dword ptr [eax + 48]
  43de3b: 8b 11                        	mov	edx, dword ptr [ecx]
  43de3d: ff 52 18                     	call	dword ptr [edx + 24]
  43de40: 8b 48 28                     	mov	ecx, dword ptr [eax + 40]
  43de43: 83 c0 28                     	add	eax, 40
  43de46: 89 4c 24 14                  	mov	dword ptr [esp + 20], ecx
  43de4a: 8b 50 04                     	mov	edx, dword ptr [eax + 4]
  43de4d: 89 54 24 18                  	mov	dword ptr [esp + 24], edx
  43de51: 8b 40 08                     	mov	eax, dword ptr [eax + 8]
  43de54: 8b 55 00                     	mov	edx, dword ptr [ebp]
  43de57: 8b cd                        	mov	ecx, ebp
  43de59: 89 44 24 1c                  	mov	dword ptr [esp + 28], eax
  43de5d: ff 52 18                     	call	dword ptr [edx + 24]
  43de60: 50                           	push	eax
  43de61: 8d 84 24 9c 00 00 00         	lea	eax, [esp + 156]
  43de68: 50                           	push	eax
  43de69: e8 42 99 05 00               	call	0x4977b0 <.text+0x967b0>
  43de6e: 8b 45 00                     	mov	eax, dword ptr [ebp]
  43de71: 83 c4 08                     	add	esp, 8
  43de74: 8d 4c 24 20                  	lea	ecx, [esp + 32]
  43de78: 51                           	push	ecx
  43de79: 8d 54 24 18                  	lea	edx, [esp + 24]
  43de7d: 52                           	push	edx
  43de7e: 8b cd                        	mov	ecx, ebp
  43de80: ff 50 18                     	call	dword ptr [eax + 24]
  43de83: 83 c0 28                     	add	eax, 40
  43de86: 50                           	push	eax
  43de87: 8d 4c 24 44                  	lea	ecx, [esp + 68]
  43de8b: 51                           	push	ecx
  43de8c: e8 3f d4 ff ff               	call	0x43b2d0 <.text+0x3a2d0>
  43de91: 83 c4 0c                     	add	esp, 12
  43de94: 8b c8                        	mov	ecx, eax
  43de96: e8 05 9c 05 00               	call	0x497aa0 <.text+0x96aa0>
  43de9b: d8 1d 24 f2 4d 00            	fcomp	dword ptr [5108260]
  43dea1: df e0                        	fnstsw	ax
  43dea3: f6 c4 05                     	test	ah, 5
  43dea6: 7a 2f                        	jp	0x43ded7 <.text+0x3ced7>
  43dea8: 8b 55 00                     	mov	edx, dword ptr [ebp]
  43deab: 6a 01                        	push	1
  43dead: 8b cd                        	mov	ecx, ebp
  43deaf: ff 52 18                     	call	dword ptr [edx + 24]
  43deb2: 83 c0 28                     	add	eax, 40
  43deb5: 50                           	push	eax
  43deb6: 8d 44 24 1c                  	lea	eax, [esp + 28]
  43deba: 50                           	push	eax
  43debb: e8 70 22 05 00               	call	0x490130 <.text+0x8f130>
  43dec0: 83 c4 0c                     	add	esp, 12
  43dec3: 84 c0                        	test	al, al
  43dec5: 74 10                        	je	0x43ded7 <.text+0x3ced7>
  43dec7: 8b 0d 78 3a 50 00            	mov	ecx, dword ptr [5257848]
  43decd: 8b 41 30                     	mov	eax, dword ptr [ecx + 48]
  43ded0: c6 80 d4 05 00 00 01         	mov	byte ptr [eax + 1492], 1
  43ded7: 8b cf                        	mov	ecx, edi
  43ded9: e8 52 25 05 00               	call	0x490430 <.text+0x8f430>
  43dede: 8b cf                        	mov	ecx, edi
  43dee0: e8 eb cf ff ff               	call	0x43aed0 <.text+0x39ed0>
  43dee5: 8b 46 4c                     	mov	eax, dword ptr [esi + 76]
  43dee8: 3b c3                        	cmp	eax, ebx
  43deea: 0f 84 8f 03 00 00            	je	0x43e27f <.text+0x3d27f>
  43def0: 83 f8 05                     	cmp	eax, 5
  43def3: 8b 96 b4 01 00 00            	mov	edx, dword ptr [esi + 436]
  43def9: 89 54 24 10                  	mov	dword ptr [esp + 16], edx
  43defd: 0f 85 b5 00 00 00            	jne	0x43dfb8 <.text+0x3cfb8>
  43df03: d9 05 04 2b 50 00            	fld	dword ptr [5253892]
  43df09: d8 8e b8 01 00 00            	fmul	dword ptr [esi + 440]
  43df0f: d8 86 b4 01 00 00            	fadd	dword ptr [esi + 436]
  43df15: d9 96 b4 01 00 00            	fst	dword ptr [esi + 436]
  43df1b: d8 9e c8 01 00 00            	fcomp	dword ptr [esi + 456]
  43df21: df e0                        	fnstsw	ax
  43df23: f6 c4 01                     	test	ah, 1
  43df26: 75 3b                        	jne	0x43df63 <.text+0x3cf63>
  43df28: d9 86 c8 01 00 00            	fld	dword ptr [esi + 456]
  43df2e: d8 a6 c4 01 00 00            	fsub	dword ptr [esi + 452]
  43df34: eb 0a                        	jmp	0x43df40 <.text+0x3cf40>
  43df36: 8d a4 24 00 00 00 00         	lea	esp, [esp]
  43df3d: 8d 49 00                     	lea	ecx, [ecx]
  43df40: d9 86 b4 01 00 00            	fld	dword ptr [esi + 436]
  43df46: d8 e1                        	fsub	st, st(1)
  43df48: d9 9e b4 01 00 00            	fstp	dword ptr [esi + 436]
  43df4e: d9 86 b4 01 00 00            	fld	dword ptr [esi + 436]
  43df54: d8 9e c8 01 00 00            	fcomp	dword ptr [esi + 456]
  43df5a: df e0                        	fnstsw	ax
  43df5c: f6 c4 01                     	test	ah, 1
  43df5f: 74 df                        	je	0x43df40 <.text+0x3cf40>
  43df61: dd d8                        	fstp	st(0)
  43df63: d9 86 b4 01 00 00            	fld	dword ptr [esi + 436]
  43df69: d8 9e c4 01 00 00            	fcomp	dword ptr [esi + 452]
  43df6f: df e0                        	fnstsw	ax
  43df71: f6 c4 41                     	test	ah, 65
  43df74: 0f 8a e2 00 00 00            	jp	0x43e05c <.text+0x3d05c>
  43df7a: d9 86 c8 01 00 00            	fld	dword ptr [esi + 456]
  43df80: d8 a6 c4 01 00 00            	fsub	dword ptr [esi + 452]
  43df86: eb 08                        	jmp	0x43df90 <.text+0x3cf90>
  43df88: 8d a4 24 00 00 00 00         	lea	esp, [esp]
  43df8f: 90                           	nop
  43df90: d9 c0                        	fld	st(0)
  43df92: d8 86 b4 01 00 00            	fadd	dword ptr [esi + 436]
  43df98: d9 9e b4 01 00 00            	fstp	dword ptr [esi + 436]
  43df9e: d9 86 b4 01 00 00            	fld	dword ptr [esi + 436]
  43dfa4: d8 9e c4 01 00 00            	fcomp	dword ptr [esi + 452]
  43dfaa: df e0                        	fnstsw	ax
  43dfac: f6 c4 41                     	test	ah, 65
  43dfaf: 7b df                        	jnp	0x43df90 <.text+0x3cf90>
  43dfb1: dd d8                        	fstp	st(0)
  43dfb3: e9 a4 00 00 00               	jmp	0x43e05c <.text+0x3d05c>
  43dfb8: 83 f8 03                     	cmp	eax, 3
  43dfbb: 75 44                        	jne	0x43e001 <.text+0x3d001>
  43dfbd: d9 05 04 2b 50 00            	fld	dword ptr [5253892]
  43dfc3: d8 8e b8 01 00 00            	fmul	dword ptr [esi + 440]
  43dfc9: d8 86 b4 01 00 00            	fadd	dword ptr [esi + 436]
  43dfcf: d8 96 c8 01 00 00            	fcom	dword ptr [esi + 456]
  43dfd5: d9 96 b4 01 00 00            	fst	dword ptr [esi + 436]
  43dfdb: df e0                        	fnstsw	ax
  43dfdd: f6 c4 01                     	test	ah, 1
  43dfe0: 75 04                        	jne	0x43dfe6 <.text+0x3cfe6>
  43dfe2: dd d8                        	fstp	st(0)
  43dfe4: eb 67                        	jmp	0x43e04d <.text+0x3d04d>
  43dfe6: d8 9e c4 01 00 00            	fcomp	dword ptr [esi + 452]
  43dfec: df e0                        	fnstsw	ax
  43dfee: f6 c4 41                     	test	ah, 65
  43dff1: 7a 69                        	jp	0x43e05c <.text+0x3d05c>
  43dff3: 8b 8e c4 01 00 00            	mov	ecx, dword ptr [esi + 452]
  43dff9: 89 8e b4 01 00 00            	mov	dword ptr [esi + 436], ecx
  43dfff: eb 58                        	jmp	0x43e059 <.text+0x3d059>
  43e001: 83 f8 04                     	cmp	eax, 4
  43e004: 75 56                        	jne	0x43e05c <.text+0x3d05c>
  43e006: d9 05 04 2b 50 00            	fld	dword ptr [5253892]
  43e00c: d8 8e b8 01 00 00            	fmul	dword ptr [esi + 440]
  43e012: d8 ae b4 01 00 00            	fsubr	dword ptr [esi + 436]
  43e018: d9 96 b4 01 00 00            	fst	dword ptr [esi + 436]
  43e01e: d8 9e c4 01 00 00            	fcomp	dword ptr [esi + 452]
  43e024: df e0                        	fnstsw	ax
  43e026: f6 c4 41                     	test	ah, 65
  43e029: 7a 0f                        	jp	0x43e03a <.text+0x3d03a>
  43e02b: 8b 96 c4 01 00 00            	mov	edx, dword ptr [esi + 452]
  43e031: 89 96 b4 01 00 00            	mov	dword ptr [esi + 436], edx
  43e037: 89 5e 4c                     	mov	dword ptr [esi + 76], ebx
  43e03a: d9 86 b4 01 00 00            	fld	dword ptr [esi + 436]
  43e040: d8 9e c8 01 00 00            	fcomp	dword ptr [esi + 456]
  43e046: df e0                        	fnstsw	ax
  43e048: f6 c4 01                     	test	ah, 1
  43e04b: 75 0f                        	jne	0x43e05c <.text+0x3d05c>
  43e04d: 8b 86 c8 01 00 00            	mov	eax, dword ptr [esi + 456]
  43e053: 89 86 b4 01 00 00            	mov	dword ptr [esi + 436], eax
  43e059: 89 5e 4c                     	mov	dword ptr [esi + 76], ebx
  43e05c: 8b 86 a0 01 00 00            	mov	eax, dword ptr [esi + 416]
  43e062: 33 ed                        	xor	ebp, ebp
  43e064: 85 c0                        	test	eax, eax
  43e066: 0f 8e 0c 01 00 00            	jle	0x43e178 <.text+0x3d178>
  43e06c: 8d 64 24 00                  	lea	esp, [esp]
  43e070: 85 ed                        	test	ebp, ebp
  43e072: 7c 08                        	jl	0x43e07c <.text+0x3d07c>
  43e074: 3b ae a0 01 00 00            	cmp	ebp, dword ptr [esi + 416]
  43e07a: 7c 3a                        	jl	0x43e0b6 <.text+0x3d0b6>
  43e07c: 33 c0                        	xor	eax, eax
  43e07e: b9 06 00 00 00               	mov	ecx, 6
  43e083: 8d 7c 24 38                  	lea	edi, [esp + 56]
  43e087: f3 ab                        	rep		stosd	dword ptr es:[edi], eax
  43e089: 8d 4c 24 38                  	lea	ecx, [esp + 56]
  43e08d: 51                           	push	ecx
  43e08e: c7 44 24 3c 01 00 00 00      	mov	dword ptr [esp + 60], 1
  43e096: c7 44 24 40 c4 db 4d 00      	mov	dword ptr [esp + 64], 5102532
  43e09e: c7 44 24 44 5c d8 4d 00      	mov	dword ptr [esp + 68], 5101660
  43e0a6: c7 44 24 48 cd 01 00 00      	mov	dword ptr [esp + 72], 461
  43e0ae: e8 ad 3f fe ff               	call	0x422060 <.text+0x21060>
  43e0b3: 83 c4 04                     	add	esp, 4
  43e0b6: 85 ed                        	test	ebp, ebp
  43e0b8: 8b 96 9c 01 00 00            	mov	edx, dword ptr [esi + 412]
  43e0be: 8b 1c aa                     	mov	ebx, dword ptr [edx + 4*ebp]
  43e0c1: 7c 08                        	jl	0x43e0cb <.text+0x3d0cb>
  43e0c3: 3b ae a0 01 00 00            	cmp	ebp, dword ptr [esi + 416]
  43e0c9: 7c 3a                        	jl	0x43e105 <.text+0x3d105>
  43e0cb: 33 c0                        	xor	eax, eax
  43e0cd: b9 06 00 00 00               	mov	ecx, 6
  43e0d2: 8d 7c 24 20                  	lea	edi, [esp + 32]
  43e0d6: f3 ab                        	rep		stosd	dword ptr es:[edi], eax
  43e0d8: 8d 44 24 20                  	lea	eax, [esp + 32]
  43e0dc: 50                           	push	eax
  43e0dd: c7 44 24 24 01 00 00 00      	mov	dword ptr [esp + 36], 1
  43e0e5: c7 44 24 28 c4 db 4d 00      	mov	dword ptr [esp + 40], 5102532
  43e0ed: c7 44 24 2c 5c d8 4d 00      	mov	dword ptr [esp + 44], 5101660
  43e0f5: c7 44 24 30 cd 01 00 00      	mov	dword ptr [esp + 48], 461
  43e0fd: e8 5e 3f fe ff               	call	0x422060 <.text+0x21060>
  43e102: 83 c4 04                     	add	esp, 4
  43e105: 8b 8e 9c 01 00 00            	mov	ecx, dword ptr [esi + 412]
  43e10b: 8b 14 a9                     	mov	edx, dword ptr [ecx + 4*ebp]
  43e10e: 52                           	push	edx
  43e10f: ff 15 94 c0 4d 00            	call	dword ptr [5095572]
  43e115: 83 c4 04                     	add	esp, 4
  43e118: 85 c0                        	test	eax, eax
  43e11a: 74 4d                        	je	0x43e169 <.text+0x3d169>
  43e11c: 33 d2                        	xor	edx, edx
  43e11e: 8b c8                        	mov	ecx, eax
  43e120: ff 15 90 c0 4d 00            	call	dword ptr [5095568]
  43e126: 8b 8e b4 01 00 00            	mov	ecx, dword ptr [esi + 436]
  43e12c: 51                           	push	ecx
  43e12d: 8d 54 24 6c                  	lea	edx, [esp + 108]
  43e131: 8b c8                        	mov	ecx, eax
  43e133: ff 15 a0 c0 4d 00            	call	dword ptr [5095584]
  43e139: 8b 15 78 3a 50 00            	mov	edx, dword ptr [5257848]
  43e13f: 8b 42 04                     	mov	eax, dword ptr [edx + 4]
  43e142: 8a 88 60 01 00 00            	mov	cl, byte ptr [eax + 352]
  43e148: 84 c9                        	test	cl, cl
  43e14a: 74 07                        	je	0x43e153 <.text+0x3d153>
  43e14c: 05 b4 00 00 00               	add	eax, 180
  43e151: eb 03                        	jmp	0x43e156 <.text+0x3d156>
  43e153: 83 c0 08                     	add	eax, 8
  43e156: 8b 40 78                     	mov	eax, dword ptr [eax + 120]
  43e159: 8d 4c 24 68                  	lea	ecx, [esp + 104]
  43e15d: 51                           	push	ecx
  43e15e: 53                           	push	ebx
  43e15f: 50                           	push	eax
  43e160: ff 15 7c c0 4d 00            	call	dword ptr [5095548]
  43e166: 83 c4 0c                     	add	esp, 12
  43e169: 8b 86 a0 01 00 00            	mov	eax, dword ptr [esi + 416]
  43e16f: 45                           	inc	ebp
  43e170: 3b e8                        	cmp	ebp, eax
  43e172: 0f 8c f8 fe ff ff            	jl	0x43e070 <.text+0x3d070>
  43e178: 8b 96 c8 00 00 00            	mov	edx, dword ptr [esi + 200]
  43e17e: 8b 42 38                     	mov	eax, dword ptr [edx + 56]
  43e181: 85 c0                        	test	eax, eax
  43e183: 74 08                        	je	0x43e18d <.text+0x3d18d>
  43e185: 8d 4e dc                     	lea	ecx, [esi - 36]
  43e188: e8 13 d6 ff ff               	call	0x43b7a0 <.text+0x3a7a0>
  43e18d: 8b 86 28 01 00 00            	mov	eax, dword ptr [esi + 296]
  43e193: 33 ed                        	xor	ebp, ebp
  43e195: 85 c0                        	test	eax, eax
  43e197: 0f 8e e2 00 00 00            	jle	0x43e27f <.text+0x3d27f>
  43e19d: 8d 49 00                     	lea	ecx, [ecx]
  43e1a0: 85 ed                        	test	ebp, ebp
  43e1a2: 7c 08                        	jl	0x43e1ac <.text+0x3d1ac>
  43e1a4: 3b ae 28 01 00 00            	cmp	ebp, dword ptr [esi + 296]
  43e1aa: 7c 3a                        	jl	0x43e1e6 <.text+0x3d1e6>
  43e1ac: 33 c0                        	xor	eax, eax
  43e1ae: b9 06 00 00 00               	mov	ecx, 6
  43e1b3: 8d 7c 24 50                  	lea	edi, [esp + 80]
  43e1b7: f3 ab                        	rep		stosd	dword ptr es:[edi], eax
  43e1b9: 8d 44 24 50                  	lea	eax, [esp + 80]
  43e1bd: 50                           	push	eax
  43e1be: c7 44 24 54 01 00 00 00      	mov	dword ptr [esp + 84], 1
  43e1c6: c7 44 24 58 c4 db 4d 00      	mov	dword ptr [esp + 88], 5102532
  43e1ce: c7 44 24 5c 5c d8 4d 00      	mov	dword ptr [esp + 92], 5101660
  43e1d6: c7 44 24 60 cd 01 00 00      	mov	dword ptr [esp + 96], 461
  43e1de: e8 7d 3e fe ff               	call	0x422060 <.text+0x21060>
  43e1e3: 83 c4 04                     	add	esp, 4
  43e1e6: 8b 8e 24 01 00 00            	mov	ecx, dword ptr [esi + 292]
  43e1ec: 8b 3c a9                     	mov	edi, dword ptr [ecx + 4*ebp]
  43e1ef: 80 3f 00                     	cmp	byte ptr [edi], 0
  43e1f2: 74 20                        	je	0x43e214 <.text+0x3d214>
  43e1f4: d9 86 b4 01 00 00            	fld	dword ptr [esi + 436]
  43e1fa: d8 67 04                     	fsub	dword ptr [edi + 4]
  43e1fd: d9 e1                        	fabs
  43e1ff: d9 86 b4 01 00 00            	fld	dword ptr [esi + 436]
  43e205: d8 64 24 10                  	fsub	dword ptr [esp + 16]
  43e209: d9 e1                        	fabs
  43e20b: de d9                        	fcompp
  43e20d: df e0                        	fnstsw	ax
  43e20f: f6 c4 41                     	test	ah, 65
  43e212: 7a 0d                        	jp	0x43e221 <.text+0x3d221>
  43e214: 8b 86 28 01 00 00            	mov	eax, dword ptr [esi + 296]
  43e21a: 45                           	inc	ebp
  43e21b: 3b e8                        	cmp	ebp, eax
  43e21d: 7c 81                        	jl	0x43e1a0 <.text+0x3d1a0>
  43e21f: eb 5e                        	jmp	0x43e27f <.text+0x3d27f>
  43e221: 8b 57 0c                     	mov	edx, dword ptr [edi + 12]
  43e224: a0 18 19 50 00               	mov	al, byte ptr [5249304]
  43e229: 84 c0                        	test	al, al
  43e22b: 52                           	push	edx
  43e22c: 51                           	push	ecx
  43e22d: 89 64 24 18                  	mov	dword ptr [esp + 24], esp
  43e231: 8b cc                        	mov	ecx, esp
  43e233: 75 2c                        	jne	0x43e261 <.text+0x3d261>
  43e235: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43e23c: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43e246: c7 05 0c 19 50 00 00 00 00 00	mov	dword ptr [5249292], 0
  43e250: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43e25a: c6 05 14 19 50 00 00         	mov	byte ptr [5249300], 0
  43e261: c7 01 08 19 50 00            	mov	dword ptr [ecx], 5249288
  43e267: 8b 47 08                     	mov	eax, dword ptr [edi + 8]
  43e26a: 3d 08 19 50 00               	cmp	eax, 5249288
  43e26f: 74 04                        	je	0x43e275 <.text+0x3d275>
  43e271: 89 01                        	mov	dword ptr [ecx], eax
  43e273: ff 00                        	inc	dword ptr [eax]
  43e275: 8b ce                        	mov	ecx, esi
  43e277: e8 94 70 03 00               	call	0x475310 <.text+0x74310>
  43e27c: c6 07 00                     	mov	byte ptr [edi], 0
  43e27f: 8b 8c 24 dc 00 00 00         	mov	ecx, dword ptr [esp + 220]
  43e286: 5f                           	pop	edi
  43e287: 5e                           	pop	esi
  43e288: 5d                           	pop	ebp
  43e289: 64 89 0d 00 00 00 00         	mov	dword ptr fs:[0], ecx
  43e290: 5b                           	pop	ebx
  43e291: 81 c4 d8 00 00 00            	add	esp, 216
  43e297: c3                           	ret
