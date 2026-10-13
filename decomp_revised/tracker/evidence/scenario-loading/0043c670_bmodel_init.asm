
inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  43c670: 6a ff                        	push	-1
  43c672: 64 a1 00 00 00 00            	mov	eax, dword ptr fs:[0]
  43c678: 68 98 ea 4c 00               	push	5040792
  43c67d: 50                           	push	eax
  43c67e: 64 89 25 00 00 00 00         	mov	dword ptr fs:[0], esp
  43c685: 83 ec 70                     	sub	esp, 112
  43c688: 53                           	push	ebx
  43c689: 55                           	push	ebp
  43c68a: 56                           	push	esi
  43c68b: 33 db                        	xor	ebx, ebx
  43c68d: 57                           	push	edi
  43c68e: 8b f9                        	mov	edi, ecx
  43c690: c7 87 e8 01 00 00 00 00 7a 44	mov	dword ptr [edi + 488], 1148846080
  43c69a: 89 9f ec 01 00 00            	mov	dword ptr [edi + 492], ebx
  43c6a0: 89 9f 58 02 00 00            	mov	dword ptr [edi + 600], ebx
  43c6a6: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43c6ac: 75 27                        	jne	0x43c6d5 <.text+0x3b6d5>
  43c6ae: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43c6b5: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43c6bf: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43c6c5: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43c6cf: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43c6d5: 68 14 0d 4e 00               	push	5115156
  43c6da: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43c6de: c7 44 24 14 08 19 50 00      	mov	dword ptr [esp + 20], 5249288
  43c6e6: e8 95 56 fc ff               	call	0x401d80 <.text+0xd80>
  43c6eb: 6a 02                        	push	2
  43c6ed: 53                           	push	ebx
  43c6ee: 8d 44 24 18                  	lea	eax, [esp + 24]
  43c6f2: 50                           	push	eax
  43c6f3: 8d 8f 24 01 00 00            	lea	ecx, [edi + 292]
  43c6f9: 89 9c 24 94 00 00 00         	mov	dword ptr [esp + 148], ebx
  43c700: e8 eb 4b 06 00               	call	0x4a12f0 <.text+0xa02f0>
  43c705: 8b 44 24 10                  	mov	eax, dword ptr [esp + 16]
  43c709: 3d 08 19 50 00               	cmp	eax, 5249288
  43c70e: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43c719: 74 13                        	je	0x43c72e <.text+0x3b72e>
  43c71b: ff 08                        	dec	dword ptr [eax]
  43c71d: 8b 4c 24 10                  	mov	ecx, dword ptr [esp + 16]
  43c721: 39 19                        	cmp	dword ptr [ecx], ebx
  43c723: 7f 09                        	jg	0x43c72e <.text+0x3b72e>
  43c725: 8d 4c 24 10                  	lea	ecx, [esp + 16]
  43c729: e8 92 4a fc ff               	call	0x4011c0 <.text+0x1c0>
  43c72e: 8b 87 ec 00 00 00            	mov	eax, dword ptr [edi + 236]
  43c734: 88 9f 3c 02 00 00            	mov	byte ptr [edi + 572], bl
  43c73a: 8b 50 0c                     	mov	edx, dword ptr [eax + 12]
  43c73d: 8b 48 10                     	mov	ecx, dword ptr [eax + 16]
  43c740: 8b 68 08                     	mov	ebp, dword ptr [eax + 8]
  43c743: 3b eb                        	cmp	ebp, ebx
  43c745: 89 54 24 2c                  	mov	dword ptr [esp + 44], edx
  43c749: 8b 50 14                     	mov	edx, dword ptr [eax + 20]
  43c74c: 89 4c 24 30                  	mov	dword ptr [esp + 48], ecx
  43c750: 8b 48 18                     	mov	ecx, dword ptr [eax + 24]
  43c753: 89 54 24 34                  	mov	dword ptr [esp + 52], edx
  43c757: 8b 50 1c                     	mov	edx, dword ptr [eax + 28]
  43c75a: 89 4c 24 38                  	mov	dword ptr [esp + 56], ecx
  43c75e: 8b 48 20                     	mov	ecx, dword ptr [eax + 32]
  43c761: 89 54 24 3c                  	mov	dword ptr [esp + 60], edx
  43c765: 8b 50 24                     	mov	edx, dword ptr [eax + 36]
  43c768: 89 4c 24 40                  	mov	dword ptr [esp + 64], ecx
  43c76c: 8b 48 28                     	mov	ecx, dword ptr [eax + 40]
  43c76f: 89 54 24 44                  	mov	dword ptr [esp + 68], edx
  43c773: 8b 50 2c                     	mov	edx, dword ptr [eax + 44]
  43c776: 89 6c 24 28                  	mov	dword ptr [esp + 40], ebp
  43c77a: 89 4c 24 48                  	mov	dword ptr [esp + 72], ecx
  43c77e: 89 54 24 4c                  	mov	dword ptr [esp + 76], edx
  43c782: 89 5c 24 50                  	mov	dword ptr [esp + 80], ebx
  43c786: 0f 84 a9 00 00 00            	je	0x43c835 <.text+0x3b835>
  43c78c: 8d 44 24 28                  	lea	eax, [esp + 40]
  43c790: 89 44 24 10                  	mov	dword ptr [esp + 16], eax
  43c794: 8b 8f c4 01 00 00            	mov	ecx, dword ptr [edi + 452]
  43c79a: 33 f6                        	xor	esi, esi
  43c79c: 3b cb                        	cmp	ecx, ebx
  43c79e: 7e 12                        	jle	0x43c7b2 <.text+0x3b7b2>
  43c7a0: 8b 87 c0 01 00 00            	mov	eax, dword ptr [edi + 448]
  43c7a6: 39 18                        	cmp	dword ptr [eax], ebx
  43c7a8: 74 17                        	je	0x43c7c1 <.text+0x3b7c1>
  43c7aa: 46                           	inc	esi
  43c7ab: 83 c0 04                     	add	eax, 4
  43c7ae: 3b f1                        	cmp	esi, ecx
  43c7b0: 7c f4                        	jl	0x43c7a6 <.text+0x3b7a6>
  43c7b2: 8d 4e 01                     	lea	ecx, [esi + 1]
  43c7b5: 51                           	push	ecx
  43c7b6: 8d 8f c0 01 00 00            	lea	ecx, [edi + 448]
  43c7bc: e8 1f ef ff ff               	call	0x43b6e0 <.text+0x3a6e0>
  43c7c1: 8b 97 c0 01 00 00            	mov	edx, dword ptr [edi + 448]
  43c7c7: 55                           	push	ebp
  43c7c8: 89 2c b2                     	mov	dword ptr [edx + 4*esi], ebp
  43c7cb: ff 15 94 c0 4d 00            	call	dword ptr [5095572]
  43c7d1: 83 c4 04                     	add	esp, 4
  43c7d4: 3b c3                        	cmp	eax, ebx
  43c7d6: 74 47                        	je	0x43c81f <.text+0x3b81f>
  43c7d8: 8d af e4 01 00 00            	lea	ebp, [edi + 484]
  43c7de: 8d b7 e0 01 00 00            	lea	esi, [edi + 480]
  43c7e4: 55                           	push	ebp
  43c7e5: 8b d6                        	mov	edx, esi
  43c7e7: 8b c8                        	mov	ecx, eax
  43c7e9: ff 15 9c c0 4d 00            	call	dword ptr [5095580]
  43c7ef: d9 06                        	fld	dword ptr [esi]
  43c7f1: d8 9f e8 01 00 00            	fcomp	dword ptr [edi + 488]
  43c7f7: df e0                        	fnstsw	ax
  43c7f9: f6 c4 05                     	test	ah, 5
  43c7fc: 7a 08                        	jp	0x43c806 <.text+0x3b806>
  43c7fe: 8b 06                        	mov	eax, dword ptr [esi]
  43c800: 89 87 e8 01 00 00            	mov	dword ptr [edi + 488], eax
  43c806: d9 45 00                     	fld	dword ptr [ebp]
  43c809: d8 9f ec 01 00 00            	fcomp	dword ptr [edi + 492]
  43c80f: df e0                        	fnstsw	ax
  43c811: f6 c4 41                     	test	ah, 65
  43c814: 75 09                        	jne	0x43c81f <.text+0x3b81f>
  43c816: 8b 4d 00                     	mov	ecx, dword ptr [ebp]
  43c819: 89 8f ec 01 00 00            	mov	dword ptr [edi + 492], ecx
  43c81f: 8b 44 24 10                  	mov	eax, dword ptr [esp + 16]
  43c823: 8b 68 04                     	mov	ebp, dword ptr [eax + 4]
  43c826: 83 c0 04                     	add	eax, 4
  43c829: 3b eb                        	cmp	ebp, ebx
  43c82b: 89 44 24 10                  	mov	dword ptr [esp + 16], eax
  43c82f: 0f 85 5f ff ff ff            	jne	0x43c794 <.text+0x3b794>
  43c835: be 54 c5 4d 00               	mov	esi, 5096788
  43c83a: 6a 01                        	push	1
  43c83c: 8d 4c 24 3c                  	lea	ecx, [esp + 60]
  43c840: 89 74 24 2c                  	mov	dword ptr [esp + 44], esi
  43c844: e8 cf 68 06 00               	call	0x4a3118 <.text+0xa2118>
  43c849: bd 44 0c 4e 00               	mov	ebp, 5114948
  43c84e: 89 6c 24 2c                  	mov	dword ptr [esp + 44], ebp
  43c852: c7 44 24 30 45 00 00 00      	mov	dword ptr [esp + 48], 69
  43c85a: 88 5c 24 34                  	mov	byte ptr [esp + 52], bl
  43c85e: 68 00 0d 4e 00               	push	5115136
  43c863: 8d 4c 24 48                  	lea	ecx, [esp + 72]
  43c867: c7 84 24 8c 00 00 00 01 00 00 00     	mov	dword ptr [esp + 140], 1
  43c872: e8 a2 71 06 00               	call	0x4a3a19 <.text+0xa2a19>
  43c877: 8b 97 ec 00 00 00            	mov	edx, dword ptr [edi + 236]
  43c87d: 8b 42 04                     	mov	eax, dword ptr [edx + 4]
  43c880: 50                           	push	eax
  43c881: 8d 4c 24 48                  	lea	ecx, [esp + 72]
  43c885: e8 8f 71 06 00               	call	0x4a3a19 <.text+0xa2a19>
  43c88a: 68 48 c7 4d 00               	push	5097288
  43c88f: 8d 4c 24 48                  	lea	ecx, [esp + 72]
  43c893: e8 81 71 06 00               	call	0x4a3a19 <.text+0xa2a19>
  43c898: 6a 03                        	push	3
  43c89a: 8d 4c 24 2c                  	lea	ecx, [esp + 44]
  43c89e: e8 bd 4a fc ff               	call	0x401360 <.text+0x360>
  43c8a3: 8d 4c 24 28                  	lea	ecx, [esp + 40]
  43c8a7: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43c8b2: e8 09 4a fc ff               	call	0x4012c0 <.text+0x2c0>
  43c8b7: 83 bf c4 01 00 00 01         	cmp	dword ptr [edi + 452], 1
  43c8be: 0f 8d 8d 00 00 00            	jge	0x43c951 <.text+0x3b951>
  43c8c4: 6a 01                        	push	1
  43c8c6: 8d 4c 24 3c                  	lea	ecx, [esp + 60]
  43c8ca: 89 74 24 2c                  	mov	dword ptr [esp + 44], esi
  43c8ce: e8 45 68 06 00               	call	0x4a3118 <.text+0xa2118>
  43c8d3: 89 6c 24 2c                  	mov	dword ptr [esp + 44], ebp
  43c8d7: c7 44 24 30 48 00 00 00      	mov	dword ptr [esp + 48], 72
  43c8df: 88 5c 24 34                  	mov	byte ptr [esp + 52], bl
  43c8e3: 68 f8 0c 4e 00               	push	5115128
  43c8e8: 8d 4c 24 48                  	lea	ecx, [esp + 72]
  43c8ec: c7 84 24 8c 00 00 00 02 00 00 00     	mov	dword ptr [esp + 140], 2
  43c8f7: e8 1d 71 06 00               	call	0x4a3a19 <.text+0xa2a19>
  43c8fc: 8b 87 ec 00 00 00            	mov	eax, dword ptr [edi + 236]
  43c902: 8b 40 04                     	mov	eax, dword ptr [eax + 4]
  43c905: 50                           	push	eax
  43c906: 8d 4c 24 48                  	lea	ecx, [esp + 72]
  43c90a: e8 0a 71 06 00               	call	0x4a3a19 <.text+0xa2a19>
  43c90f: 68 e4 0c 4e 00               	push	5115108
  43c914: 8d 4c 24 48                  	lea	ecx, [esp + 72]
  43c918: e8 fc 70 06 00               	call	0x4a3a19 <.text+0xa2a19>
  43c91d: 53                           	push	ebx
  43c91e: 8d 4c 24 2c                  	lea	ecx, [esp + 44]
  43c922: e8 39 4a fc ff               	call	0x401360 <.text+0x360>
  43c927: 8d 4c 24 28                  	lea	ecx, [esp + 40]
  43c92b: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43c936: e8 85 49 fc ff               	call	0x4012c0 <.text+0x2c0>
  43c93b: 8b 8c 24 80 00 00 00         	mov	ecx, dword ptr [esp + 128]
  43c942: 64 89 0d 00 00 00 00         	mov	dword ptr fs:[0], ecx
  43c949: 5f                           	pop	edi
  43c94a: 5e                           	pop	esi
  43c94b: 5d                           	pop	ebp
  43c94c: 5b                           	pop	ebx
  43c94d: 83 c4 7c                     	add	esp, 124
  43c950: c3                           	ret
  43c951: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43c955: 51                           	push	ecx
  43c956: 8b cf                        	mov	ecx, edi
  43c958: e8 f3 09 06 00               	call	0x49d350 <.text+0x9c350>
  43c95d: 8b 97 ec 00 00 00            	mov	edx, dword ptr [edi + 236]
  43c963: 50                           	push	eax
  43c964: 8b 42 04                     	mov	eax, dword ptr [edx + 4]
  43c967: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43c96d: 51                           	push	ecx
  43c96e: 89 64 24 18                  	mov	dword ptr [esp + 24], esp
  43c972: 8b cc                        	mov	ecx, esp
  43c974: 75 27                        	jne	0x43c99d <.text+0x3b99d>
  43c976: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43c97d: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43c987: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43c98d: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43c997: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43c99d: 50                           	push	eax
  43c99e: c7 01 08 19 50 00            	mov	dword ptr [ecx], 5249288
  43c9a4: e8 d7 53 fc ff               	call	0x401d80 <.text+0xd80>
  43c9a9: 8d 6f 24                     	lea	ebp, [edi + 36]
  43c9ac: 8b cd                        	mov	ecx, ebp
  43c9ae: e8 6d 19 03 00               	call	0x46e320 <.text+0x6d320>
  43c9b3: 8b 87 e8 01 00 00            	mov	eax, dword ptr [edi + 488]
  43c9b9: 8b cf                        	mov	ecx, edi
  43c9bb: 89 87 d8 01 00 00            	mov	dword ptr [edi + 472], eax
  43c9c1: c7 87 dc 01 00 00 00 00 80 3f	mov	dword ptr [edi + 476], 1065353216
  43c9cb: e8 d0 ed ff ff               	call	0x43b7a0 <.text+0x3a7a0>
  43c9d0: 8d 4c 24 1c                  	lea	ecx, [esp + 28]
  43c9d4: 51                           	push	ecx
  43c9d5: 8b cf                        	mov	ecx, edi
  43c9d7: e8 a4 f2 ff ff               	call	0x43bc80 <.text+0x3ac80>
  43c9dc: 8b 10                        	mov	edx, dword ptr [eax]
  43c9de: 89 97 24 02 00 00            	mov	dword ptr [edi + 548], edx
  43c9e4: 8b 48 04                     	mov	ecx, dword ptr [eax + 4]
  43c9e7: 89 8f 28 02 00 00            	mov	dword ptr [edi + 552], ecx
  43c9ed: 8b 50 08                     	mov	edx, dword ptr [eax + 8]
  43c9f0: 89 97 2c 02 00 00            	mov	dword ptr [edi + 556], edx
  43c9f6: 8b 87 ec 00 00 00            	mov	eax, dword ptr [edi + 236]
  43c9fc: 8b 40 3c                     	mov	eax, dword ptr [eax + 60]
  43c9ff: 3b c3                        	cmp	eax, ebx
  43ca01: 0f 84 cc 00 00 00            	je	0x43cad3 <.text+0x3bad3>
  43ca07: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43ca0d: 75 27                        	jne	0x43ca36 <.text+0x3ba36>
  43ca0f: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43ca16: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43ca20: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43ca26: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43ca30: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43ca36: 50                           	push	eax
  43ca37: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43ca3b: c7 44 24 14 08 19 50 00      	mov	dword ptr [esp + 20], 5249288
  43ca43: e8 38 53 fc ff               	call	0x401d80 <.text+0xd80>
  43ca48: 8b 74 24 10                  	mov	esi, dword ptr [esp + 16]
  43ca4c: 39 5e 04                     	cmp	dword ptr [esi + 4], ebx
  43ca4f: c7 84 24 88 00 00 00 03 00 00 00     	mov	dword ptr [esp + 136], 3
  43ca5a: 74 50                        	je	0x43caac <.text+0x3baac>
  43ca5c: 53                           	push	ebx
  43ca5d: 51                           	push	ecx
  43ca5e: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43ca64: 89 64 24 1c                  	mov	dword ptr [esp + 28], esp
  43ca68: 8b c4                        	mov	eax, esp
  43ca6a: 75 27                        	jne	0x43ca93 <.text+0x3ba93>
  43ca6c: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43ca73: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43ca7d: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43ca83: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43ca8d: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43ca93: 81 fe 08 19 50 00            	cmp	esi, 5249288
  43ca99: c7 00 08 19 50 00            	mov	dword ptr [eax], 5249288
  43ca9f: 74 04                        	je	0x43caa5 <.text+0x3baa5>
  43caa1: 89 30                        	mov	dword ptr [eax], esi
  43caa3: ff 06                        	inc	dword ptr [esi]
  43caa5: 8b cd                        	mov	ecx, ebp
  43caa7: e8 64 88 03 00               	call	0x475310 <.text+0x74310>
  43caac: 81 fe 08 19 50 00            	cmp	esi, 5249288
  43cab2: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43cabd: 74 14                        	je	0x43cad3 <.text+0x3bad3>
  43cabf: 8b 0e                        	mov	ecx, dword ptr [esi]
  43cac1: 49                           	dec	ecx
  43cac2: 8b c1                        	mov	eax, ecx
  43cac4: 3b c3                        	cmp	eax, ebx
  43cac6: 89 0e                        	mov	dword ptr [esi], ecx
  43cac8: 7f 09                        	jg	0x43cad3 <.text+0x3bad3>
  43caca: 8d 4c 24 10                  	lea	ecx, [esp + 16]
  43cace: e8 ed 46 fc ff               	call	0x4011c0 <.text+0x1c0>
  43cad3: 8b 8f ec 00 00 00            	mov	ecx, dword ptr [edi + 236]
  43cad9: 8b 41 40                     	mov	eax, dword ptr [ecx + 64]
  43cadc: 3b c3                        	cmp	eax, ebx
  43cade: 0f 84 c4 00 00 00            	je	0x43cba8 <.text+0x3bba8>
  43cae4: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43caea: 75 27                        	jne	0x43cb13 <.text+0x3bb13>
  43caec: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43caf3: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43cafd: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43cb03: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43cb0d: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43cb13: bd 08 19 50 00               	mov	ebp, 5249288
  43cb18: 50                           	push	eax
  43cb19: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43cb1d: 89 6c 24 14                  	mov	dword ptr [esp + 20], ebp
  43cb21: e8 5a 52 fc ff               	call	0x401d80 <.text+0xd80>
  43cb26: 8b 74 24 10                  	mov	esi, dword ptr [esp + 16]
  43cb2a: 39 5e 04                     	cmp	dword ptr [esi + 4], ebx
  43cb2d: c7 84 24 88 00 00 00 04 00 00 00     	mov	dword ptr [esp + 136], 4
  43cb38: 74 49                        	je	0x43cb83 <.text+0x3bb83>
  43cb3a: 53                           	push	ebx
  43cb3b: 51                           	push	ecx
  43cb3c: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43cb42: 89 64 24 1c                  	mov	dword ptr [esp + 28], esp
  43cb46: 8b c4                        	mov	eax, esp
  43cb48: 75 27                        	jne	0x43cb71 <.text+0x3bb71>
  43cb4a: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43cb51: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43cb5b: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43cb61: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43cb6b: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43cb71: 3b f5                        	cmp	esi, ebp
  43cb73: 89 28                        	mov	dword ptr [eax], ebp
  43cb75: 74 04                        	je	0x43cb7b <.text+0x3bb7b>
  43cb77: 89 30                        	mov	dword ptr [eax], esi
  43cb79: ff 06                        	inc	dword ptr [esi]
  43cb7b: 8d 4f 24                     	lea	ecx, [edi + 36]
  43cb7e: e8 8d 87 03 00               	call	0x475310 <.text+0x74310>
  43cb83: 3b f5                        	cmp	esi, ebp
  43cb85: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43cb90: 74 1b                        	je	0x43cbad <.text+0x3bbad>
  43cb92: 8b 0e                        	mov	ecx, dword ptr [esi]
  43cb94: 49                           	dec	ecx
  43cb95: 8b c1                        	mov	eax, ecx
  43cb97: 3b c3                        	cmp	eax, ebx
  43cb99: 89 0e                        	mov	dword ptr [esi], ecx
  43cb9b: 7f 10                        	jg	0x43cbad <.text+0x3bbad>
  43cb9d: 8d 4c 24 10                  	lea	ecx, [esp + 16]
  43cba1: e8 1a 46 fc ff               	call	0x4011c0 <.text+0x1c0>
  43cba6: eb 05                        	jmp	0x43cbad <.text+0x3bbad>
  43cba8: bd 08 19 50 00               	mov	ebp, 5249288
  43cbad: 8b 97 ec 00 00 00            	mov	edx, dword ptr [edi + 236]
  43cbb3: 8b 42 44                     	mov	eax, dword ptr [edx + 68]
  43cbb6: 3b c3                        	cmp	eax, ebx
  43cbb8: 0f 84 bd 00 00 00            	je	0x43cc7b <.text+0x3bc7b>
  43cbbe: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43cbc4: 75 27                        	jne	0x43cbed <.text+0x3bbed>
  43cbc6: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43cbcd: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43cbd7: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43cbdd: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43cbe7: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43cbed: 50                           	push	eax
  43cbee: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43cbf2: 89 6c 24 14                  	mov	dword ptr [esp + 20], ebp
  43cbf6: e8 85 51 fc ff               	call	0x401d80 <.text+0xd80>
  43cbfb: 8b 74 24 10                  	mov	esi, dword ptr [esp + 16]
  43cbff: 39 5e 04                     	cmp	dword ptr [esi + 4], ebx
  43cc02: c7 84 24 88 00 00 00 05 00 00 00     	mov	dword ptr [esp + 136], 5
  43cc0d: 74 49                        	je	0x43cc58 <.text+0x3bc58>
  43cc0f: 53                           	push	ebx
  43cc10: 51                           	push	ecx
  43cc11: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43cc17: 89 64 24 1c                  	mov	dword ptr [esp + 28], esp
  43cc1b: 8b c4                        	mov	eax, esp
  43cc1d: 75 27                        	jne	0x43cc46 <.text+0x3bc46>
  43cc1f: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43cc26: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43cc30: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43cc36: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43cc40: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43cc46: 3b f5                        	cmp	esi, ebp
  43cc48: 89 28                        	mov	dword ptr [eax], ebp
  43cc4a: 74 04                        	je	0x43cc50 <.text+0x3bc50>
  43cc4c: 89 30                        	mov	dword ptr [eax], esi
  43cc4e: ff 06                        	inc	dword ptr [esi]
  43cc50: 8d 4f 24                     	lea	ecx, [edi + 36]
  43cc53: e8 b8 86 03 00               	call	0x475310 <.text+0x74310>
  43cc58: 3b f5                        	cmp	esi, ebp
  43cc5a: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43cc65: 74 14                        	je	0x43cc7b <.text+0x3bc7b>
  43cc67: 8b 0e                        	mov	ecx, dword ptr [esi]
  43cc69: 49                           	dec	ecx
  43cc6a: 8b c1                        	mov	eax, ecx
  43cc6c: 3b c3                        	cmp	eax, ebx
  43cc6e: 89 0e                        	mov	dword ptr [esi], ecx
  43cc70: 7f 09                        	jg	0x43cc7b <.text+0x3bc7b>
  43cc72: 8d 4c 24 10                  	lea	ecx, [esp + 16]
  43cc76: e8 45 45 fc ff               	call	0x4011c0 <.text+0x1c0>
  43cc7b: 8b 87 ec 00 00 00            	mov	eax, dword ptr [edi + 236]
  43cc81: 8b 40 48                     	mov	eax, dword ptr [eax + 72]
  43cc84: 3b c3                        	cmp	eax, ebx
  43cc86: 0f 84 bd 00 00 00            	je	0x43cd49 <.text+0x3bd49>
  43cc8c: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43cc92: 75 27                        	jne	0x43ccbb <.text+0x3bcbb>
  43cc94: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43cc9b: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43cca5: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43ccab: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43ccb5: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43ccbb: 50                           	push	eax
  43ccbc: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43ccc0: 89 6c 24 14                  	mov	dword ptr [esp + 20], ebp
  43ccc4: e8 b7 50 fc ff               	call	0x401d80 <.text+0xd80>
  43ccc9: 8b 74 24 10                  	mov	esi, dword ptr [esp + 16]
  43cccd: 39 5e 04                     	cmp	dword ptr [esi + 4], ebx
  43ccd0: c7 84 24 88 00 00 00 06 00 00 00     	mov	dword ptr [esp + 136], 6
  43ccdb: 74 49                        	je	0x43cd26 <.text+0x3bd26>
  43ccdd: 53                           	push	ebx
  43ccde: 51                           	push	ecx
  43ccdf: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43cce5: 89 64 24 1c                  	mov	dword ptr [esp + 28], esp
  43cce9: 8b c4                        	mov	eax, esp
  43cceb: 75 27                        	jne	0x43cd14 <.text+0x3bd14>
  43cced: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43ccf4: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43ccfe: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43cd04: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43cd0e: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43cd14: 3b f5                        	cmp	esi, ebp
  43cd16: 89 28                        	mov	dword ptr [eax], ebp
  43cd18: 74 04                        	je	0x43cd1e <.text+0x3bd1e>
  43cd1a: 89 30                        	mov	dword ptr [eax], esi
  43cd1c: ff 06                        	inc	dword ptr [esi]
  43cd1e: 8d 4f 24                     	lea	ecx, [edi + 36]
  43cd21: e8 ea 85 03 00               	call	0x475310 <.text+0x74310>
  43cd26: 3b f5                        	cmp	esi, ebp
  43cd28: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43cd33: 74 14                        	je	0x43cd49 <.text+0x3bd49>
  43cd35: 8b 0e                        	mov	ecx, dword ptr [esi]
  43cd37: 49                           	dec	ecx
  43cd38: 8b c1                        	mov	eax, ecx
  43cd3a: 3b c3                        	cmp	eax, ebx
  43cd3c: 89 0e                        	mov	dword ptr [esi], ecx
  43cd3e: 7f 09                        	jg	0x43cd49 <.text+0x3bd49>
  43cd40: 8d 4c 24 10                  	lea	ecx, [esp + 16]
  43cd44: e8 77 44 fc ff               	call	0x4011c0 <.text+0x1c0>
  43cd49: 8b 8f ec 00 00 00            	mov	ecx, dword ptr [edi + 236]
  43cd4f: 8b 41 4c                     	mov	eax, dword ptr [ecx + 76]
  43cd52: 3b c3                        	cmp	eax, ebx
  43cd54: 0f 84 bd 00 00 00            	je	0x43ce17 <.text+0x3be17>
  43cd5a: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43cd60: 75 27                        	jne	0x43cd89 <.text+0x3bd89>
  43cd62: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43cd69: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43cd73: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43cd79: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43cd83: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43cd89: 50                           	push	eax
  43cd8a: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43cd8e: 89 6c 24 14                  	mov	dword ptr [esp + 20], ebp
  43cd92: e8 e9 4f fc ff               	call	0x401d80 <.text+0xd80>
  43cd97: 8b 74 24 10                  	mov	esi, dword ptr [esp + 16]
  43cd9b: 39 5e 04                     	cmp	dword ptr [esi + 4], ebx
  43cd9e: c7 84 24 88 00 00 00 07 00 00 00     	mov	dword ptr [esp + 136], 7
  43cda9: 74 49                        	je	0x43cdf4 <.text+0x3bdf4>
  43cdab: 53                           	push	ebx
  43cdac: 51                           	push	ecx
  43cdad: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43cdb3: 89 64 24 1c                  	mov	dword ptr [esp + 28], esp
  43cdb7: 8b c4                        	mov	eax, esp
  43cdb9: 75 27                        	jne	0x43cde2 <.text+0x3bde2>
  43cdbb: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43cdc2: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43cdcc: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43cdd2: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43cddc: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43cde2: 3b f5                        	cmp	esi, ebp
  43cde4: 89 28                        	mov	dword ptr [eax], ebp
  43cde6: 74 04                        	je	0x43cdec <.text+0x3bdec>
  43cde8: 89 30                        	mov	dword ptr [eax], esi
  43cdea: ff 06                        	inc	dword ptr [esi]
  43cdec: 8d 4f 24                     	lea	ecx, [edi + 36]
  43cdef: e8 1c 85 03 00               	call	0x475310 <.text+0x74310>
  43cdf4: 3b f5                        	cmp	esi, ebp
  43cdf6: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43ce01: 74 14                        	je	0x43ce17 <.text+0x3be17>
  43ce03: 8b 0e                        	mov	ecx, dword ptr [esi]
  43ce05: 49                           	dec	ecx
  43ce06: 8b c1                        	mov	eax, ecx
  43ce08: 3b c3                        	cmp	eax, ebx
  43ce0a: 89 0e                        	mov	dword ptr [esi], ecx
  43ce0c: 7f 09                        	jg	0x43ce17 <.text+0x3be17>
  43ce0e: 8d 4c 24 10                  	lea	ecx, [esp + 16]
  43ce12: e8 a9 43 fc ff               	call	0x4011c0 <.text+0x1c0>
  43ce17: 8b 97 ec 00 00 00            	mov	edx, dword ptr [edi + 236]
  43ce1d: 8b 42 50                     	mov	eax, dword ptr [edx + 80]
  43ce20: 3b c3                        	cmp	eax, ebx
  43ce22: 0f 84 bd 00 00 00            	je	0x43cee5 <.text+0x3bee5>
  43ce28: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43ce2e: 75 27                        	jne	0x43ce57 <.text+0x3be57>
  43ce30: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43ce37: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43ce41: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43ce47: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43ce51: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43ce57: 50                           	push	eax
  43ce58: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43ce5c: 89 6c 24 14                  	mov	dword ptr [esp + 20], ebp
  43ce60: e8 1b 4f fc ff               	call	0x401d80 <.text+0xd80>
  43ce65: 8b 74 24 10                  	mov	esi, dword ptr [esp + 16]
  43ce69: 39 5e 04                     	cmp	dword ptr [esi + 4], ebx
  43ce6c: c7 84 24 88 00 00 00 08 00 00 00     	mov	dword ptr [esp + 136], 8
  43ce77: 74 49                        	je	0x43cec2 <.text+0x3bec2>
  43ce79: 53                           	push	ebx
  43ce7a: 51                           	push	ecx
  43ce7b: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43ce81: 89 64 24 1c                  	mov	dword ptr [esp + 28], esp
  43ce85: 8b c4                        	mov	eax, esp
  43ce87: 75 27                        	jne	0x43ceb0 <.text+0x3beb0>
  43ce89: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43ce90: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43ce9a: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43cea0: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43ceaa: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43ceb0: 3b f5                        	cmp	esi, ebp
  43ceb2: 89 28                        	mov	dword ptr [eax], ebp
  43ceb4: 74 04                        	je	0x43ceba <.text+0x3beba>
  43ceb6: 89 30                        	mov	dword ptr [eax], esi
  43ceb8: ff 06                        	inc	dword ptr [esi]
  43ceba: 8d 4f 24                     	lea	ecx, [edi + 36]
  43cebd: e8 4e 84 03 00               	call	0x475310 <.text+0x74310>
  43cec2: 3b f5                        	cmp	esi, ebp
  43cec4: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43cecf: 74 14                        	je	0x43cee5 <.text+0x3bee5>
  43ced1: 8b 0e                        	mov	ecx, dword ptr [esi]
  43ced3: 49                           	dec	ecx
  43ced4: 8b c1                        	mov	eax, ecx
  43ced6: 3b c3                        	cmp	eax, ebx
  43ced8: 89 0e                        	mov	dword ptr [esi], ecx
  43ceda: 7f 09                        	jg	0x43cee5 <.text+0x3bee5>
  43cedc: 8d 4c 24 10                  	lea	ecx, [esp + 16]
  43cee0: e8 db 42 fc ff               	call	0x4011c0 <.text+0x1c0>
  43cee5: 8b 87 ec 00 00 00            	mov	eax, dword ptr [edi + 236]
  43ceeb: 8b 40 54                     	mov	eax, dword ptr [eax + 84]
  43ceee: 3b c3                        	cmp	eax, ebx
  43cef0: 0f 84 bd 00 00 00            	je	0x43cfb3 <.text+0x3bfb3>
  43cef6: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43cefc: 75 27                        	jne	0x43cf25 <.text+0x3bf25>
  43cefe: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43cf05: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43cf0f: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43cf15: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43cf1f: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43cf25: 50                           	push	eax
  43cf26: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43cf2a: 89 6c 24 14                  	mov	dword ptr [esp + 20], ebp
  43cf2e: e8 4d 4e fc ff               	call	0x401d80 <.text+0xd80>
  43cf33: 8b 74 24 10                  	mov	esi, dword ptr [esp + 16]
  43cf37: 39 5e 04                     	cmp	dword ptr [esi + 4], ebx
  43cf3a: c7 84 24 88 00 00 00 09 00 00 00     	mov	dword ptr [esp + 136], 9
  43cf45: 74 49                        	je	0x43cf90 <.text+0x3bf90>
  43cf47: 53                           	push	ebx
  43cf48: 51                           	push	ecx
  43cf49: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43cf4f: 89 64 24 1c                  	mov	dword ptr [esp + 28], esp
  43cf53: 8b c4                        	mov	eax, esp
  43cf55: 75 27                        	jne	0x43cf7e <.text+0x3bf7e>
  43cf57: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43cf5e: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43cf68: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43cf6e: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43cf78: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43cf7e: 3b f5                        	cmp	esi, ebp
  43cf80: 89 28                        	mov	dword ptr [eax], ebp
  43cf82: 74 04                        	je	0x43cf88 <.text+0x3bf88>
  43cf84: 89 30                        	mov	dword ptr [eax], esi
  43cf86: ff 06                        	inc	dword ptr [esi]
  43cf88: 8d 4f 24                     	lea	ecx, [edi + 36]
  43cf8b: e8 80 83 03 00               	call	0x475310 <.text+0x74310>
  43cf90: 3b f5                        	cmp	esi, ebp
  43cf92: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43cf9d: 74 14                        	je	0x43cfb3 <.text+0x3bfb3>
  43cf9f: 8b 0e                        	mov	ecx, dword ptr [esi]
  43cfa1: 49                           	dec	ecx
  43cfa2: 8b c1                        	mov	eax, ecx
  43cfa4: 3b c3                        	cmp	eax, ebx
  43cfa6: 89 0e                        	mov	dword ptr [esi], ecx
  43cfa8: 7f 09                        	jg	0x43cfb3 <.text+0x3bfb3>
  43cfaa: 8d 4c 24 10                  	lea	ecx, [esp + 16]
  43cfae: e8 0d 42 fc ff               	call	0x4011c0 <.text+0x1c0>
  43cfb3: 8b 8f ec 00 00 00            	mov	ecx, dword ptr [edi + 236]
  43cfb9: 8b 41 58                     	mov	eax, dword ptr [ecx + 88]
  43cfbc: 3b c3                        	cmp	eax, ebx
  43cfbe: 0f 84 bd 00 00 00            	je	0x43d081 <.text+0x3c081>
  43cfc4: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43cfca: 75 27                        	jne	0x43cff3 <.text+0x3bff3>
  43cfcc: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43cfd3: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43cfdd: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43cfe3: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43cfed: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43cff3: 50                           	push	eax
  43cff4: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43cff8: 89 6c 24 14                  	mov	dword ptr [esp + 20], ebp
  43cffc: e8 7f 4d fc ff               	call	0x401d80 <.text+0xd80>
  43d001: 8b 74 24 10                  	mov	esi, dword ptr [esp + 16]
  43d005: 39 5e 04                     	cmp	dword ptr [esi + 4], ebx
  43d008: c7 84 24 88 00 00 00 0a 00 00 00     	mov	dword ptr [esp + 136], 10
  43d013: 74 49                        	je	0x43d05e <.text+0x3c05e>
  43d015: 53                           	push	ebx
  43d016: 51                           	push	ecx
  43d017: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43d01d: 89 64 24 1c                  	mov	dword ptr [esp + 28], esp
  43d021: 8b c4                        	mov	eax, esp
  43d023: 75 27                        	jne	0x43d04c <.text+0x3c04c>
  43d025: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43d02c: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43d036: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43d03c: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43d046: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43d04c: 3b f5                        	cmp	esi, ebp
  43d04e: 89 28                        	mov	dword ptr [eax], ebp
  43d050: 74 04                        	je	0x43d056 <.text+0x3c056>
  43d052: 89 30                        	mov	dword ptr [eax], esi
  43d054: ff 06                        	inc	dword ptr [esi]
  43d056: 8d 4f 24                     	lea	ecx, [edi + 36]
  43d059: e8 b2 82 03 00               	call	0x475310 <.text+0x74310>
  43d05e: 3b f5                        	cmp	esi, ebp
  43d060: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43d06b: 74 14                        	je	0x43d081 <.text+0x3c081>
  43d06d: 8b 0e                        	mov	ecx, dword ptr [esi]
  43d06f: 49                           	dec	ecx
  43d070: 8b c1                        	mov	eax, ecx
  43d072: 3b c3                        	cmp	eax, ebx
  43d074: 89 0e                        	mov	dword ptr [esi], ecx
  43d076: 7f 09                        	jg	0x43d081 <.text+0x3c081>
  43d078: 8d 4c 24 10                  	lea	ecx, [esp + 16]
  43d07c: e8 3f 41 fc ff               	call	0x4011c0 <.text+0x1c0>
  43d081: 8b 97 ec 00 00 00            	mov	edx, dword ptr [edi + 236]
  43d087: 8b 42 5c                     	mov	eax, dword ptr [edx + 92]
  43d08a: 3b c3                        	cmp	eax, ebx
  43d08c: 0f 84 bd 00 00 00            	je	0x43d14f <.text+0x3c14f>
  43d092: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43d098: 75 27                        	jne	0x43d0c1 <.text+0x3c0c1>
  43d09a: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43d0a1: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43d0ab: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43d0b1: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43d0bb: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43d0c1: 50                           	push	eax
  43d0c2: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43d0c6: 89 6c 24 14                  	mov	dword ptr [esp + 20], ebp
  43d0ca: e8 b1 4c fc ff               	call	0x401d80 <.text+0xd80>
  43d0cf: 8b 74 24 10                  	mov	esi, dword ptr [esp + 16]
  43d0d3: 39 5e 04                     	cmp	dword ptr [esi + 4], ebx
  43d0d6: c7 84 24 88 00 00 00 0b 00 00 00     	mov	dword ptr [esp + 136], 11
  43d0e1: 74 49                        	je	0x43d12c <.text+0x3c12c>
  43d0e3: 53                           	push	ebx
  43d0e4: 51                           	push	ecx
  43d0e5: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43d0eb: 89 64 24 1c                  	mov	dword ptr [esp + 28], esp
  43d0ef: 8b c4                        	mov	eax, esp
  43d0f1: 75 27                        	jne	0x43d11a <.text+0x3c11a>
  43d0f3: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43d0fa: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43d104: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43d10a: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43d114: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43d11a: 3b f5                        	cmp	esi, ebp
  43d11c: 89 28                        	mov	dword ptr [eax], ebp
  43d11e: 74 04                        	je	0x43d124 <.text+0x3c124>
  43d120: 89 30                        	mov	dword ptr [eax], esi
  43d122: ff 06                        	inc	dword ptr [esi]
  43d124: 8d 4f 24                     	lea	ecx, [edi + 36]
  43d127: e8 e4 81 03 00               	call	0x475310 <.text+0x74310>
  43d12c: 3b f5                        	cmp	esi, ebp
  43d12e: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43d139: 74 14                        	je	0x43d14f <.text+0x3c14f>
  43d13b: 8b 0e                        	mov	ecx, dword ptr [esi]
  43d13d: 49                           	dec	ecx
  43d13e: 8b c1                        	mov	eax, ecx
  43d140: 3b c3                        	cmp	eax, ebx
  43d142: 89 0e                        	mov	dword ptr [esi], ecx
  43d144: 7f 09                        	jg	0x43d14f <.text+0x3c14f>
  43d146: 8d 4c 24 10                  	lea	ecx, [esp + 16]
  43d14a: e8 71 40 fc ff               	call	0x4011c0 <.text+0x1c0>
  43d14f: 8b 87 ec 00 00 00            	mov	eax, dword ptr [edi + 236]
  43d155: 8b 40 60                     	mov	eax, dword ptr [eax + 96]
  43d158: 3b c3                        	cmp	eax, ebx
  43d15a: 0f 84 bd 00 00 00            	je	0x43d21d <.text+0x3c21d>
  43d160: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43d166: 75 27                        	jne	0x43d18f <.text+0x3c18f>
  43d168: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43d16f: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43d179: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43d17f: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43d189: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43d18f: 50                           	push	eax
  43d190: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  43d194: 89 6c 24 14                  	mov	dword ptr [esp + 20], ebp
  43d198: e8 e3 4b fc ff               	call	0x401d80 <.text+0xd80>
  43d19d: 8b 74 24 10                  	mov	esi, dword ptr [esp + 16]
  43d1a1: 39 5e 04                     	cmp	dword ptr [esi + 4], ebx
  43d1a4: c7 84 24 88 00 00 00 0c 00 00 00     	mov	dword ptr [esp + 136], 12
  43d1af: 74 49                        	je	0x43d1fa <.text+0x3c1fa>
  43d1b1: 53                           	push	ebx
  43d1b2: 51                           	push	ecx
  43d1b3: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43d1b9: 89 64 24 1c                  	mov	dword ptr [esp + 28], esp
  43d1bd: 8b c4                        	mov	eax, esp
  43d1bf: 75 27                        	jne	0x43d1e8 <.text+0x3c1e8>
  43d1c1: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43d1c8: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43d1d2: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43d1d8: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43d1e2: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43d1e8: 3b f5                        	cmp	esi, ebp
  43d1ea: 89 28                        	mov	dword ptr [eax], ebp
  43d1ec: 74 04                        	je	0x43d1f2 <.text+0x3c1f2>
  43d1ee: 89 30                        	mov	dword ptr [eax], esi
  43d1f0: ff 06                        	inc	dword ptr [esi]
  43d1f2: 8d 4f 24                     	lea	ecx, [edi + 36]
  43d1f5: e8 16 81 03 00               	call	0x475310 <.text+0x74310>
  43d1fa: 3b f5                        	cmp	esi, ebp
  43d1fc: c7 84 24 88 00 00 00 ff ff ff ff     	mov	dword ptr [esp + 136], 4294967295
  43d207: 74 14                        	je	0x43d21d <.text+0x3c21d>
  43d209: 8b 0e                        	mov	ecx, dword ptr [esi]
  43d20b: 49                           	dec	ecx
  43d20c: 8b c1                        	mov	eax, ecx
  43d20e: 3b c3                        	cmp	eax, ebx
  43d210: 89 0e                        	mov	dword ptr [esi], ecx
  43d212: 7f 09                        	jg	0x43d21d <.text+0x3c21d>
  43d214: 8d 4c 24 10                  	lea	ecx, [esp + 16]
  43d218: e8 a3 3f fc ff               	call	0x4011c0 <.text+0x1c0>
  43d21d: 39 9f 58 02 00 00            	cmp	dword ptr [edi + 600], ebx
  43d223: 75 58                        	jne	0x43d27d <.text+0x3c27d>
  43d225: 38 1d 18 19 50 00            	cmp	byte ptr [5249304], bl
  43d22b: 51                           	push	ecx
  43d22c: 89 64 24 18                  	mov	dword ptr [esp + 24], esp
  43d230: 8b cc                        	mov	ecx, esp
  43d232: 75 27                        	jne	0x43d25b <.text+0x3c25b>
  43d234: c6 05 18 19 50 00 01         	mov	byte ptr [5249304], 1
  43d23b: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  43d245: 89 1d 0c 19 50 00            	mov	dword ptr [5249292], ebx
  43d24b: c7 05 10 19 50 00 01 00 00 00	mov	dword ptr [5249296], 1
  43d255: 88 1d 14 19 50 00            	mov	byte ptr [5249300], bl
  43d25b: 68 e0 0c 4e 00               	push	5115104
  43d260: 89 29                        	mov	dword ptr [ecx], ebp
  43d262: e8 19 4b fc ff               	call	0x401d80 <.text+0xd80>
  43d267: 8d 4f 24                     	lea	ecx, [edi + 36]
  43d26a: e8 61 11 03 00               	call	0x46e3d0 <.text+0x6d3d0>
  43d26f: 3a c3                        	cmp	al, bl
  43d271: 74 0a                        	je	0x43d27d <.text+0x3c27d>
  43d273: c7 87 58 02 00 00 02 00 00 00	mov	dword ptr [edi + 600], 2
  43d27d: 8b 8f ec 00 00 00            	mov	ecx, dword ptr [edi + 236]
  43d283: 39 59 38                     	cmp	dword ptr [ecx + 56], ebx
  43d286: 74 07                        	je	0x43d28f <.text+0x3c28f>
  43d288: 8b cf                        	mov	ecx, edi
  43d28a: e8 71 eb ff ff               	call	0x43be00 <.text+0x3ae00>
  43d28f: 8b 8c 24 80 00 00 00         	mov	ecx, dword ptr [esp + 128]
  43d296: 5f                           	pop	edi
  43d297: 5e                           	pop	esi
  43d298: 5d                           	pop	ebp
  43d299: 64 89 0d 00 00 00 00         	mov	dword ptr fs:[0], ecx
  43d2a0: 5b                           	pop	ebx
  43d2a1: 83 c4 7c                     	add	esp, 124
  43d2a4: c3                           	ret
