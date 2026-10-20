; Hash-pinned retail movement helper/caller x86 excerpts. Read-only evidence.
; Binary: inputs/original/binaries/FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Ghidra 12.0.4 helper decompilation: player_locomotion_helpers_fresh_2026-10-20.c
; The unaligned source block 00452570 is a manual instruction slice, not a Ghidra function.

; Yaw helper 004968B0
  4968b0: 83 ec 24                     	sub	esp, 36
  4968b3: d9 44 24 28                  	fld	dword ptr [esp + 40]
  4968b7: 53                           	push	ebx
  4968b8: d8 0d bc 22 4e 00            	fmul	dword ptr [5120700]
  4968be: 8b d9                        	mov	ebx, ecx
  4968c0: 56                           	push	esi
  4968c1: 8d 73 04                     	lea	esi, [ebx + 4]
  4968c4: d9 c0                        	fld	st(0)
  4968c6: 57                           	push	edi
  4968c7: d9 ff                        	fcos
  4968c9: 8d 7b 10                     	lea	edi, [ebx + 16]
  4968cc: 8b c6                        	mov	eax, esi
  4968ce: d9 c9                        	fxch	st(1)
  4968d0: d9 fe                        	fsin
  4968d2: d9 c0                        	fld	st(0)
  4968d4: d9 e0                        	fchs
  4968d6: d9 5c 24 24                  	fstp	dword ptr [esp + 36]
  4968da: d9 c0                        	fld	st(0)
  4968dc: d8 4e 08                     	fmul	dword ptr [esi + 8]
  4968df: d9 c2                        	fld	st(2)
  4968e1: d8 0e                        	fmul	dword ptr [esi]
  4968e3: de c1                        	faddp	st(1), st
  4968e5: d9 46 04                     	fld	dword ptr [esi + 4]
  4968e8: d8 0d 0c c5 4d 00            	fmul	dword ptr [5096716]
  4968ee: de c1                        	faddp	st(1), st
  4968f0: d9 5c 24 0c                  	fstp	dword ptr [esp + 12]
  4968f4: 8b 4c 24 0c                  	mov	ecx, dword ptr [esp + 12]
  4968f8: d9 46 08                     	fld	dword ptr [esi + 8]
  4968fb: d8 06                        	fadd	dword ptr [esi]
  4968fd: d8 0d 0c c5 4d 00            	fmul	dword ptr [5096716]
  496903: d8 46 04                     	fadd	dword ptr [esi + 4]
  496906: d9 5c 24 10                  	fstp	dword ptr [esp + 16]
  49690a: 8b 54 24 10                  	mov	edx, dword ptr [esp + 16]
  49690e: d9 c1                        	fld	st(1)
  496910: d8 4e 08                     	fmul	dword ptr [esi + 8]
  496913: d9 44 24 24                  	fld	dword ptr [esp + 36]
  496917: d8 0e                        	fmul	dword ptr [esi]
  496919: de c1                        	faddp	st(1), st
  49691b: d9 46 04                     	fld	dword ptr [esi + 4]
  49691e: d8 0d 0c c5 4d 00            	fmul	dword ptr [5096716]
  496924: de c1                        	faddp	st(1), st
  496926: d9 5c 24 14                  	fstp	dword ptr [esp + 20]
  49692a: d8 4f 08                     	fmul	dword ptr [edi + 8]
  49692d: d9 c1                        	fld	st(1)
  49692f: d8 0f                        	fmul	dword ptr [edi]
  496931: de c1                        	faddp	st(1), st
  496933: d9 47 04                     	fld	dword ptr [edi + 4]
  496936: d8 0d 0c c5 4d 00            	fmul	dword ptr [5096716]
  49693c: de c1                        	faddp	st(1), st
  49693e: d9 5c 24 18                  	fstp	dword ptr [esp + 24]
  496942: d9 47 08                     	fld	dword ptr [edi + 8]
  496945: d8 07                        	fadd	dword ptr [edi]
  496947: d8 0d 0c c5 4d 00            	fmul	dword ptr [5096716]
  49694d: d8 47 04                     	fadd	dword ptr [edi + 4]
  496950: d9 5c 24 1c                  	fstp	dword ptr [esp + 28]
  496954: d8 4f 08                     	fmul	dword ptr [edi + 8]
  496957: d9 44 24 24                  	fld	dword ptr [esp + 36]
  49695b: d8 0f                        	fmul	dword ptr [edi]
  49695d: de c1                        	faddp	st(1), st
  49695f: d9 47 04                     	fld	dword ptr [edi + 4]
  496962: 89 08                        	mov	dword ptr [eax], ecx
  496964: d8 0d 0c c5 4d 00            	fmul	dword ptr [5096716]
  49696a: 8b 4c 24 14                  	mov	ecx, dword ptr [esp + 20]
  49696e: 89 50 04                     	mov	dword ptr [eax + 4], edx
  496971: 8d 54 24 24                  	lea	edx, [esp + 36]
  496975: de c1                        	faddp	st(1), st
  496977: 89 48 08                     	mov	dword ptr [eax + 8], ecx
  49697a: 52                           	push	edx
  49697b: 8d 4c 24 1c                  	lea	ecx, [esp + 28]
  49697f: d9 5c 24 24                  	fstp	dword ptr [esp + 36]
  496983: e8 a8 38 fc ff               	call	0x45a230 <.text+0x59230>
  496988: 8b 10                        	mov	edx, dword ptr [eax]
  49698a: 8b cf                        	mov	ecx, edi
  49698c: 89 11                        	mov	dword ptr [ecx], edx
  49698e: 8b 50 04                     	mov	edx, dword ptr [eax + 4]
  496991: 89 51 04                     	mov	dword ptr [ecx + 4], edx
  496994: 8b 40 08                     	mov	eax, dword ptr [eax + 8]
  496997: 89 41 08                     	mov	dword ptr [ecx + 8], eax
  49699a: 8d 4c 24 24                  	lea	ecx, [esp + 36]
  49699e: 51                           	push	ecx
  49699f: 57                           	push	edi
  4969a0: 56                           	push	esi
  4969a1: 8d 54 24 24                  	lea	edx, [esp + 36]
  4969a5: 52                           	push	edx
  4969a6: e8 05 0d fb ff               	call	0x4476b0 <.text+0x466b0>
  4969ab: 83 c4 0c                     	add	esp, 12
  4969ae: 8b c8                        	mov	ecx, eax
  4969b0: e8 7b 38 fc ff               	call	0x45a230 <.text+0x59230>
  4969b5: 8d 4b 1c                     	lea	ecx, [ebx + 28]
  4969b8: 8b 18                        	mov	ebx, dword ptr [eax]
  4969ba: 8b d1                        	mov	edx, ecx
  4969bc: 89 1a                        	mov	dword ptr [edx], ebx
  4969be: 8b 58 04                     	mov	ebx, dword ptr [eax + 4]
  4969c1: 89 5a 04                     	mov	dword ptr [edx + 4], ebx
  4969c4: 8b 40 08                     	mov	eax, dword ptr [eax + 8]
  4969c7: 89 42 08                     	mov	dword ptr [edx + 8], eax
  4969ca: 8d 54 24 24                  	lea	edx, [esp + 36]
  4969ce: 52                           	push	edx
  4969cf: 51                           	push	ecx
  4969d0: 8d 44 24 20                  	lea	eax, [esp + 32]
  4969d4: 57                           	push	edi
  4969d5: 50                           	push	eax
  4969d6: e8 d5 0c fb ff               	call	0x4476b0 <.text+0x466b0>
  4969db: 83 c4 0c                     	add	esp, 12
  4969de: 8b c8                        	mov	ecx, eax
  4969e0: e8 4b 38 fc ff               	call	0x45a230 <.text+0x59230>
  4969e5: 8b 08                        	mov	ecx, dword ptr [eax]
  4969e7: 89 0e                        	mov	dword ptr [esi], ecx
  4969e9: 8b 50 04                     	mov	edx, dword ptr [eax + 4]
  4969ec: 89 56 04                     	mov	dword ptr [esi + 4], edx
  4969ef: 8b 40 08                     	mov	eax, dword ptr [eax + 8]
  4969f2: 5f                           	pop	edi
  4969f3: 89 46 08                     	mov	dword ptr [esi + 8], eax
  4969f6: 5e                           	pop	esi
  4969f7: 5b                           	pop	ebx
  4969f8: 83 c4 24                     	add	esp, 36
  4969fb: c2 04 00                     	ret	4

; Basis multiply 00495EB0
  495eb0: 83 ec 24                     	sub	esp, 36
  495eb3: 8b 44 24 2c                  	mov	eax, dword ptr [esp + 44]
  495eb7: d9 40 34                     	fld	dword ptr [eax + 52]
  495eba: 8b 4c 24 30                  	mov	ecx, dword ptr [esp + 48]
  495ebe: d8 09                        	fmul	dword ptr [ecx]
  495ec0: 8b 50 0c                     	mov	edx, dword ptr [eax + 12]
  495ec3: d9 40 38                     	fld	dword ptr [eax + 56]
  495ec6: 89 54 24 0c                  	mov	dword ptr [esp + 12], edx
  495eca: d8 49 04                     	fmul	dword ptr [ecx + 4]
  495ecd: d9 40 3c                     	fld	dword ptr [eax + 60]
  495ed0: d8 49 08                     	fmul	dword ptr [ecx + 8]
  495ed3: 8b 48 08                     	mov	ecx, dword ptr [eax + 8]
  495ed6: d9 40 04                     	fld	dword ptr [eax + 4]
  495ed9: 89 4c 24 00                  	mov	dword ptr [esp], ecx
  495edd: d9 40 10                     	fld	dword ptr [eax + 16]
  495ee0: d9 40 1c                     	fld	dword ptr [eax + 28]
  495ee3: d9 40 14                     	fld	dword ptr [eax + 20]
  495ee6: d9 5c 24 04                  	fstp	dword ptr [esp + 4]
  495eea: d9 40 20                     	fld	dword ptr [eax + 32]
  495eed: d9 5c 24 08                  	fstp	dword ptr [esp + 8]
  495ef1: d9 40 18                     	fld	dword ptr [eax + 24]
  495ef4: d9 5c 24 10                  	fstp	dword ptr [esp + 16]
  495ef8: d9 40 24                     	fld	dword ptr [eax + 36]
  495efb: 8b 44 24 28                  	mov	eax, dword ptr [esp + 40]
  495eff: d9 5c 24 14                  	fstp	dword ptr [esp + 20]
  495f03: 8b c8                        	mov	ecx, eax
  495f05: d8 cb                        	fmul	st, st(3)
  495f07: d9 ca                        	fxch	st(2)
  495f09: d8 cd                        	fmul	st, st(5)
  495f0b: de c2                        	faddp	st(2), st
  495f0d: d8 cb                        	fmul	st, st(3)
  495f0f: de c1                        	faddp	st(1), st
  495f11: d9 5c 24 18                  	fstp	dword ptr [esp + 24]
  495f15: 8b 54 24 18                  	mov	edx, dword ptr [esp + 24]
  495f19: d9 44 24 08                  	fld	dword ptr [esp + 8]
  495f1d: 89 11                        	mov	dword ptr [ecx], edx
  495f1f: d8 c9                        	fmul	st, st(1)
  495f21: d9 44 24 04                  	fld	dword ptr [esp + 4]
  495f25: d8 cb                        	fmul	st, st(3)
  495f27: de c1                        	faddp	st(1), st
  495f29: d9 44 24 00                  	fld	dword ptr [esp]
  495f2d: d8 cc                        	fmul	st, st(4)
  495f2f: de c1                        	faddp	st(1), st
  495f31: d9 5c 24 1c                  	fstp	dword ptr [esp + 28]
  495f35: 8b 54 24 1c                  	mov	edx, dword ptr [esp + 28]
  495f39: d9 44 24 14                  	fld	dword ptr [esp + 20]
  495f3d: 89 51 04                     	mov	dword ptr [ecx + 4], edx
  495f40: d8 c9                        	fmul	st, st(1)
  495f42: d9 44 24 10                  	fld	dword ptr [esp + 16]
  495f46: d8 cb                        	fmul	st, st(3)
  495f48: de c1                        	faddp	st(1), st
  495f4a: d9 44 24 0c                  	fld	dword ptr [esp + 12]
  495f4e: d8 cc                        	fmul	st, st(4)
  495f50: de c1                        	faddp	st(1), st
  495f52: d9 5c 24 20                  	fstp	dword ptr [esp + 32]
  495f56: 8b 54 24 20                  	mov	edx, dword ptr [esp + 32]
  495f5a: 89 51 08                     	mov	dword ptr [ecx + 8], edx
  495f5d: dd d8                        	fstp	st(0)
  495f5f: dd d8                        	fstp	st(0)
  495f61: dd d8                        	fstp	st(0)
  495f63: 83 c4 24                     	add	esp, 36
  495f66: c3                           	ret

; Normal update yaw setup and state branch
  452d1f: 8d 8c 24 9c 00 00 00         	lea	ecx, [esp + 156]
  452d26: 8d be e4 fe ff ff            	lea	edi, [esi - 284]
  452d2c: 51                           	push	ecx
  452d2d: 8b cf                        	mov	ecx, edi
  452d2f: 89 86 0c 33 00 00            	mov	dword ptr [esi + 13068], eax
  452d35: e8 16 a6 fd ff               	call	0x42d350 <.text+0x2c350>
  452d3a: 8b 96 f4 32 00 00            	mov	edx, dword ptr [esi + 13044]
  452d40: 52                           	push	edx
  452d41: 8b cf                        	mov	ecx, edi
  452d43: e8 68 3b 04 00               	call	0x4968b0 <.text+0x958b0>
  452d48: 83 7e 4c 07                  	cmp	dword ptr [esi + 76], 7

; Movement-vector initialization
  452e3c: c7 44 24 18 00 00 00 00      	mov	dword ptr [esp + 24], 0
  452e44: c7 44 24 1c 00 00 00 00      	mov	dword ptr [esp + 28], 0
  452e4c: c7 44 24 20 00 00 00 00      	mov	dword ptr [esp + 32], 0

; MOVE_FORWARD contribution
  452eb5: 84 db                        	test	bl, bl
  452eb7: 74 1e                        	je	0x452ed7 <.text+0x51ed7>
  452eb9: d9 44 24 20                  	fld	dword ptr [esp + 32]
  452ebd: 8b 4c 24 18                  	mov	ecx, dword ptr [esp + 24]
  452ec1: d8 05 24 fc 4d 00            	fadd	dword ptr [5110820]
  452ec7: 8b 54 24 1c                  	mov	edx, dword ptr [esp + 28]
  452ecb: 89 4c 24 18                  	mov	dword ptr [esp + 24], ecx
  452ecf: 89 54 24 1c                  	mov	dword ptr [esp + 28], edx
  452ed3: d9 5c 24 20                  	fstp	dword ptr [esp + 32]

; MOVE_BACKWARD contribution
  452f2f: 84 db                        	test	bl, bl
  452f31: 74 1e                        	je	0x452f51 <.text+0x51f51>
  452f33: d9 44 24 20                  	fld	dword ptr [esp + 32]
  452f37: 8b 54 24 18                  	mov	edx, dword ptr [esp + 24]
  452f3b: d8 25 24 fc 4d 00            	fsub	dword ptr [5110820]
  452f41: 8b 44 24 1c                  	mov	eax, dword ptr [esp + 28]
  452f45: 89 54 24 18                  	mov	dword ptr [esp + 24], edx
  452f49: 89 44 24 1c                  	mov	dword ptr [esp + 28], eax
  452f4d: d9 5c 24 20                  	fstp	dword ptr [esp + 32]

; MOVE_LEFT contribution
  452fa9: 84 db                        	test	bl, bl
  452fab: 74 1e                        	je	0x452fcb <.text+0x51fcb>
  452fad: d9 44 24 18                  	fld	dword ptr [esp + 24]
  452fb1: 8b 44 24 1c                  	mov	eax, dword ptr [esp + 28]
  452fb5: d8 05 18 1c 4e 00            	fadd	dword ptr [5119000]
  452fbb: 8b 4c 24 20                  	mov	ecx, dword ptr [esp + 32]
  452fbf: 89 44 24 1c                  	mov	dword ptr [esp + 28], eax
  452fc3: 89 4c 24 20                  	mov	dword ptr [esp + 32], ecx
  452fc7: d9 5c 24 18                  	fstp	dword ptr [esp + 24]

; MOVE_RIGHT contribution
  453023: 84 db                        	test	bl, bl
  453025: 74 1e                        	je	0x453045 <.text+0x52045>
  453027: d9 44 24 18                  	fld	dword ptr [esp + 24]
  45302b: 8b 4c 24 1c                  	mov	ecx, dword ptr [esp + 28]
  45302f: d8 25 18 1c 4e 00            	fsub	dword ptr [5119000]
  453035: 8b 54 24 20                  	mov	edx, dword ptr [esp + 32]
  453039: 89 4c 24 1c                  	mov	dword ptr [esp + 28], ecx
  45303d: 89 54 24 20                  	mov	dword ptr [esp + 32], edx
  453041: d9 5c 24 18                  	fstp	dword ptr [esp + 24]

; Normal versus state-5 basis transform
  4531c8: 8d 86 c0 fe ff ff            	lea	eax, [esi - 320]
  4531ce: f7 d8                        	neg	eax
  4531d0: 1b c0                        	sbb	eax, eax
  4531d2: 8d 8e e4 fe ff ff            	lea	ecx, [esi - 284]
  4531d8: 8d 54 24 18                  	lea	edx, [esp + 24]
  4531dc: 23 c1                        	and	eax, ecx
  4531de: 52                           	push	edx
  4531df: 50                           	push	eax
  4531e0: 8d 44 24 30                  	lea	eax, [esp + 48]
  4531e4: 50                           	push	eax
  4531e5: e8 c6 2c 04 00               	call	0x495eb0 <.text+0x94eb0>
  4531ea: 8b 08                        	mov	ecx, dword ptr [eax]
  4531ec: 89 4c 24 24                  	mov	dword ptr [esp + 36], ecx
  4531f0: 8b 50 04                     	mov	edx, dword ptr [eax + 4]
  4531f3: 89 54 24 28                  	mov	dword ptr [esp + 40], edx
  4531f7: 8b 40 08                     	mov	eax, dword ptr [eax + 8]
  4531fa: 89 44 24 2c                  	mov	dword ptr [esp + 44], eax
  4531fe: eb 3b                        	jmp	0x45323b <.text+0x5223b>
  453200: 8d 4c 24 18                  	lea	ecx, [esp + 24]
  453204: 51                           	push	ecx
  453205: 8d 96 20 33 00 00            	lea	edx, [esi + 13088]
  45320b: 52                           	push	edx
  45320c: 8d 84 24 a4 00 00 00         	lea	eax, [esp + 164]
  453213: 50                           	push	eax
  453214: e8 97 45 04 00               	call	0x4977b0 <.text+0x967b0>
  453219: 83 c4 08                     	add	esp, 8
  45321c: 50                           	push	eax
  45321d: 8d 4c 24 30                  	lea	ecx, [esp + 48]
  453221: 51                           	push	ecx
  453222: e8 89 2c 04 00               	call	0x495eb0 <.text+0x94eb0>
  453227: 8b 10                        	mov	edx, dword ptr [eax]
  453229: 89 54 24 24                  	mov	dword ptr [esp + 36], edx

; Data VA 0x004DC50C: 00 00 00 00 binary32=0
; Data VA 0x004E22BC: 35 fa 8e 3c binary32=0.01745329238474369
