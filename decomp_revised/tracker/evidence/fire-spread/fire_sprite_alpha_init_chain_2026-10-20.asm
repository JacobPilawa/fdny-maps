# Raw disassembly excerpts for FireRescue sprite alpha initialization
# Input: FDNYFirefighter.exe
# SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
# Generated from the pinned local retail image with objdump -d -Mintel.

# FireRescue visual child slots (0x0044b790..0x0044b800)
inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  44b790: 53                           	push	ebx
  44b791: 56                           	push	esi
  44b792: 33 db                        	xor	ebx, ebx
  44b794: 53                           	push	ebx
  44b795: 8b f1                        	mov	esi, ecx
  44b797: e8 34 1b fe ff               	call	0x42d2d0 <.text+0x2c2d0>
  44b79c: c7 06 30 fc 4d 00            	mov	dword ptr [esi], 5110832
  44b7a2: 89 5e 44                     	mov	dword ptr [esi + 68], ebx
  44b7a5: 89 5e 48                     	mov	dword ptr [esi + 72], ebx
  44b7a8: 89 5e 4c                     	mov	dword ptr [esi + 76], ebx
  44b7ab: 89 5e 70                     	mov	dword ptr [esi + 112], ebx
  44b7ae: 89 5e 74                     	mov	dword ptr [esi + 116], ebx
  44b7b1: 89 5e 78                     	mov	dword ptr [esi + 120], ebx
  44b7b4: 89 5e 7c                     	mov	dword ptr [esi + 124], ebx
  44b7b7: c7 86 84 00 00 00 10 00 00 00	mov	dword ptr [esi + 132], 16
  44b7c1: c7 86 80 00 00 00 40 66 48 00	mov	dword ptr [esi + 128], 4744768
  44b7cb: 89 9e 90 00 00 00            	mov	dword ptr [esi + 144], ebx
  44b7d1: 89 9e 94 00 00 00            	mov	dword ptr [esi + 148], ebx
  44b7d7: 89 9e 98 00 00 00            	mov	dword ptr [esi + 152], ebx
  44b7dd: 88 5e 50                     	mov	byte ptr [esi + 80], bl
  44b7e0: 33 c0                        	xor	eax, eax
  44b7e2: 89 46 60                     	mov	dword ptr [esi + 96], eax
  44b7e5: 89 46 64                     	mov	dword ptr [esi + 100], eax
  44b7e8: 89 46 68                     	mov	dword ptr [esi + 104], eax
  44b7eb: 89 46 6c                     	mov	dword ptr [esi + 108], eax
  44b7ee: 89 5e 5c                     	mov	dword ptr [esi + 92], ebx
  44b7f1: 89 5e 58                     	mov	dword ptr [esi + 88], ebx
  44b7f4: 8b c6                        	mov	eax, esi
  44b7f6: 5e                           	pop	esi
  44b7f7: 5b                           	pop	ebx
  44b7f8: c3                           	ret
  44b7f9: 90                           	nop
  44b7fa: 90                           	nop
  44b7fb: 90                           	nop
  44b7fc: 90                           	nop
  44b7fd: 90                           	nop
  44b7fe: 90                           	nop
  44b7ff: 90                           	nop

# FireRescue visual setup and sprite construction (0x0044b270..0x0044b510)
inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  44b270: 64 a1 00 00 00 00            	mov	eax, dword ptr fs:[0]
  44b276: 6a ff                        	push	-1
  44b278: 68 a1 fa 4c 00               	push	5044897
  44b27d: 50                           	push	eax
  44b27e: 64 89 25 00 00 00 00         	mov	dword ptr fs:[0], esp
  44b285: 83 ec 0c                     	sub	esp, 12
  44b288: 53                           	push	ebx
  44b289: 56                           	push	esi
  44b28a: 57                           	push	edi
  44b28b: 8b f9                        	mov	edi, ecx
  44b28d: e8 de fa ff ff               	call	0x44ad70 <.text+0x49d70>
  44b292: 68 00 00 80 3f               	push	1065353216
  44b297: 68 00 00 80 3f               	push	1065353216
  44b29c: 68 00 00 80 3f               	push	1065353216
  44b2a1: 8d 44 24 18                  	lea	eax, [esp + 24]
  44b2a5: 33 db                        	xor	ebx, ebx
  44b2a7: 50                           	push	eax
  44b2a8: 8d 8f 90 00 00 00            	lea	ecx, [edi + 144]
  44b2ae: c6 87 9c 00 00 00 01         	mov	byte ptr [edi + 156], 1
  44b2b5: 89 9f 88 00 00 00            	mov	dword ptr [edi + 136], ebx
  44b2bb: 89 9f 8c 00 00 00            	mov	dword ptr [edi + 140], ebx
  44b2c1: e8 ba 1f fe ff               	call	0x42d280 <.text+0x2c280>
  44b2c6: 8a 44 24 2c                  	mov	al, byte ptr [esp + 44]
  44b2ca: 84 c0                        	test	al, al
  44b2cc: 8b 74 24 28                  	mov	esi, dword ptr [esp + 40]
  44b2d0: 74 50                        	je	0x44b322 <.text+0x4a322>
  44b2d2: 39 5f 58                     	cmp	dword ptr [edi + 88], ebx
  44b2d5: 75 4b                        	jne	0x44b322 <.text+0x4a322>
  44b2d7: 68 cc 00 00 00               	push	204
  44b2dc: e8 a3 a7 05 00               	call	0x4a5a84 <.text+0xa4a84>
  44b2e1: 83 c4 04                     	add	esp, 4
  44b2e4: 89 44 24 2c                  	mov	dword ptr [esp + 44], eax
  44b2e8: 3b c3                        	cmp	eax, ebx
  44b2ea: 89 5c 24 20                  	mov	dword ptr [esp + 32], ebx
  44b2ee: 74 0a                        	je	0x44b2fa <.text+0x4a2fa>
  44b2f0: 56                           	push	esi
  44b2f1: 8b c8                        	mov	ecx, eax
  44b2f3: e8 48 db 00 00               	call	0x458e40 <.text+0x57e40>
  44b2f8: eb 02                        	jmp	0x44b2fc <.text+0x4a2fc>
  44b2fa: 33 c0                        	xor	eax, eax
  44b2fc: 68 9a 99 99 3f               	push	1067030938
  44b301: 68 cd cc 4c 3f               	push	1061997773
  44b306: c7 44 24 28 ff ff ff ff      	mov	dword ptr [esp + 40], 4294967295
  44b30e: 89 47 58                     	mov	dword ptr [edi + 88], eax
  44b311: e8 1a d8 04 00               	call	0x498b30 <.text+0x97b30>
  44b316: d8 0d 10 18 4e 00            	fmul	dword ptr [5117968]
  44b31c: 83 c4 08                     	add	esp, 8
  44b31f: d9 5f 54                     	fstp	dword ptr [edi + 84]
  44b322: 39 5f 5c                     	cmp	dword ptr [edi + 92], ebx
  44b325: 74 30                        	je	0x44b357 <.text+0x4a357>
  44b327: 8b 4f 60                     	mov	ecx, dword ptr [edi + 96]
  44b32a: c6 41 30 01                  	mov	byte ptr [ecx + 48], 1
  44b32e: 8b 57 64                     	mov	edx, dword ptr [edi + 100]
  44b331: c6 42 30 01                  	mov	byte ptr [edx + 48], 1
  44b335: 8b 47 68                     	mov	eax, dword ptr [edi + 104]
  44b338: c6 40 30 01                  	mov	byte ptr [eax + 48], 1
  44b33c: 8b 4f 6c                     	mov	ecx, dword ptr [edi + 108]
  44b33f: 5f                           	pop	edi
  44b340: 5e                           	pop	esi
  44b341: c6 41 30 01                  	mov	byte ptr [ecx + 48], 1
  44b345: 5b                           	pop	ebx
  44b346: 8b 4c 24 0c                  	mov	ecx, dword ptr [esp + 12]
  44b34a: 64 89 0d 00 00 00 00         	mov	dword ptr fs:[0], ecx
  44b351: 83 c4 18                     	add	esp, 24
  44b354: c2 08 00                     	ret	8
  44b357: 89 77 5c                     	mov	dword ptr [edi + 92], esi
  44b35a: 8d 77 60                     	lea	esi, [edi + 96]
  44b35d: bb 03 00 00 00               	mov	ebx, 3
  44b362: 55                           	push	ebp
  44b363: 68 68 01 00 00               	push	360
  44b368: e8 17 a7 05 00               	call	0x4a5a84 <.text+0xa4a84>
  44b36d: 83 c4 04                     	add	esp, 4
  44b370: 89 44 24 30                  	mov	dword ptr [esp + 48], eax
  44b374: 85 c0                        	test	eax, eax
  44b376: c7 44 24 24 01 00 00 00      	mov	dword ptr [esp + 36], 1
  44b37e: 74 10                        	je	0x44b390 <.text+0x4a390>
  44b380: 6a 00                        	push	0
  44b382: 68 08 2d 50 00               	push	5254408
  44b387: 8b c8                        	mov	ecx, eax
  44b389: e8 c2 44 03 00               	call	0x47f850 <.text+0x7e850>
  44b38e: eb 02                        	jmp	0x44b392 <.text+0x4a392>
  44b390: 33 c0                        	xor	eax, eax
  44b392: 89 06                        	mov	dword ptr [esi], eax
  44b394: c6 80 ad 00 00 00 01         	mov	byte ptr [eax + 173], 1
  44b39b: 8b 16                        	mov	edx, dword ptr [esi]
  44b39d: 68 00 00 c0 3f               	push	1069547520
  44b3a2: c7 82 b0 00 00 00 04 00 00 00	mov	dword ptr [edx + 176], 4
  44b3ac: 8b 2e                        	mov	ebp, dword ptr [esi]
  44b3ae: 68 00 00 00 3f               	push	1056964608
  44b3b3: c7 44 24 2c ff ff ff ff      	mov	dword ptr [esp + 44], 4294967295
  44b3bb: e8 70 d7 04 00               	call	0x498b30 <.text+0x97b30>
  44b3c0: d8 4d 1c                     	fmul	dword ptr [ebp + 28]
  44b3c3: 83 c4 04                     	add	esp, 4
  44b3c6: d9 5d 1c                     	fstp	dword ptr [ebp + 28]
  44b3c9: a1 0c 2d 50 00               	mov	eax, dword ptr [5254412]
  44b3ce: 48                           	dec	eax
  44b3cf: 89 44 24 34                  	mov	dword ptr [esp + 52], eax
  44b3d3: db 44 24 34                  	fild	dword ptr [esp + 52]
  44b3d7: d9 1c 24                     	fstp	dword ptr [esp]
  44b3da: 6a 00                        	push	0
  44b3dc: e8 4f d7 04 00               	call	0x498b30 <.text+0x97b30>
  44b3e1: 83 c4 08                     	add	esp, 8
  44b3e4: e8 17 e0 06 00               	call	0x4b9400 <.text+0xb8400>
  44b3e9: 8b 0e                        	mov	ecx, dword ptr [esi]
  44b3eb: 50                           	push	eax
  44b3ec: e8 0f 1f 03 00               	call	0x47d300 <.text+0x7c300>
  44b3f1: 8b 0e                        	mov	ecx, dword ptr [esi]
  44b3f3: c7 81 a4 00 00 00 a0 00 00 00	mov	dword ptr [ecx + 164], 160
  44b3fd: 8b 06                        	mov	eax, dword ptr [esi]
  44b3ff: d9 80 e8 00 00 00            	fld	dword ptr [eax + 232]
  44b405: 83 c6 04                     	add	esi, 4
  44b408: 4b                           	dec	ebx
  44b409: dc 05 78 fc 4d 00            	fadd	qword ptr [5110904]
  44b40f: d9 98 e8 00 00 00            	fstp	dword ptr [eax + 232]
  44b415: 8b 46 fc                     	mov	eax, dword ptr [esi - 4]
  44b418: d9 80 0c 01 00 00            	fld	dword ptr [eax + 268]
  44b41e: dc 05 78 fc 4d 00            	fadd	qword ptr [5110904]
  44b424: d9 98 0c 01 00 00            	fstp	dword ptr [eax + 268]
  44b42a: 0f 85 33 ff ff ff            	jne	0x44b363 <.text+0x4a363>
  44b430: 68 68 01 00 00               	push	360
  44b435: e8 4a a6 05 00               	call	0x4a5a84 <.text+0xa4a84>
  44b43a: 83 c4 04                     	add	esp, 4
  44b43d: 89 44 24 30                  	mov	dword ptr [esp + 48], eax
  44b441: 85 c0                        	test	eax, eax
  44b443: c7 44 24 24 02 00 00 00      	mov	dword ptr [esp + 36], 2
  44b44b: 5d                           	pop	ebp
  44b44c: 74 10                        	je	0x44b45e <.text+0x4a45e>
  44b44e: 6a 00                        	push	0
  44b450: 68 d0 2c 50 00               	push	5254352
  44b455: 8b c8                        	mov	ecx, eax
  44b457: e8 f4 43 03 00               	call	0x47f850 <.text+0x7e850>
  44b45c: eb 02                        	jmp	0x44b460 <.text+0x4a460>
  44b45e: 33 c0                        	xor	eax, eax
  44b460: 89 47 6c                     	mov	dword ptr [edi + 108], eax
  44b463: c6 80 ad 00 00 00 01         	mov	byte ptr [eax + 173], 1
  44b46a: 8b 57 6c                     	mov	edx, dword ptr [edi + 108]
  44b46d: 68 00 00 c0 3f               	push	1069547520
  44b472: c7 82 b0 00 00 00 04 00 00 00	mov	dword ptr [edx + 176], 4
  44b47c: 8b 77 6c                     	mov	esi, dword ptr [edi + 108]
  44b47f: 68 00 00 00 3f               	push	1056964608
  44b484: c7 44 24 28 ff ff ff ff      	mov	dword ptr [esp + 40], 4294967295
  44b48c: e8 9f d6 04 00               	call	0x498b30 <.text+0x97b30>
  44b491: d8 4e 1c                     	fmul	dword ptr [esi + 28]
  44b494: 83 c4 04                     	add	esp, 4
  44b497: d9 5e 1c                     	fstp	dword ptr [esi + 28]
  44b49a: a1 0c 2d 50 00               	mov	eax, dword ptr [5254412]
  44b49f: 48                           	dec	eax
  44b4a0: 89 44 24 30                  	mov	dword ptr [esp + 48], eax
  44b4a4: db 44 24 30                  	fild	dword ptr [esp + 48]
  44b4a8: d9 1c 24                     	fstp	dword ptr [esp]
  44b4ab: 6a 00                        	push	0
  44b4ad: e8 7e d6 04 00               	call	0x498b30 <.text+0x97b30>
  44b4b2: 83 c4 08                     	add	esp, 8
  44b4b5: e8 46 df 06 00               	call	0x4b9400 <.text+0xb8400>
  44b4ba: 8b 4f 6c                     	mov	ecx, dword ptr [edi + 108]
  44b4bd: 50                           	push	eax
  44b4be: e8 3d 1e 03 00               	call	0x47d300 <.text+0x7c300>
  44b4c3: 8b 4f 6c                     	mov	ecx, dword ptr [edi + 108]
  44b4c6: c7 81 a4 00 00 00 a0 00 00 00	mov	dword ptr [ecx + 164], 160
  44b4d0: 8b 47 6c                     	mov	eax, dword ptr [edi + 108]
  44b4d3: d9 80 e8 00 00 00            	fld	dword ptr [eax + 232]
  44b4d9: 8b 4c 24 18                  	mov	ecx, dword ptr [esp + 24]
  44b4dd: dc 05 78 fc 4d 00            	fadd	qword ptr [5110904]
  44b4e3: d9 98 e8 00 00 00            	fstp	dword ptr [eax + 232]
  44b4e9: 8b 7f 6c                     	mov	edi, dword ptr [edi + 108]
  44b4ec: d9 87 0c 01 00 00            	fld	dword ptr [edi + 268]
  44b4f2: dc 05 78 fc 4d 00            	fadd	qword ptr [5110904]
  44b4f8: d9 9f 0c 01 00 00            	fstp	dword ptr [edi + 268]
  44b4fe: 5f                           	pop	edi
  44b4ff: 5e                           	pop	esi
  44b500: 5b                           	pop	ebx
  44b501: 64 89 0d 00 00 00 00         	mov	dword ptr fs:[0], ecx
  44b508: 83 c4 18                     	add	esp, 24
  44b50b: c2 08 00                     	ret	8
  44b50e: 90                           	nop
  44b50f: 90                           	nop

# Sprite constructor (0x0047f850..0x0047f960)
inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  47f850: 6a ff                        	push	-1
  47f852: 68 98 34 4d 00               	push	5059736
  47f857: 64 a1 00 00 00 00            	mov	eax, dword ptr fs:[0]
  47f85d: 50                           	push	eax
  47f85e: 64 89 25 00 00 00 00         	mov	dword ptr fs:[0], esp
  47f865: 51                           	push	ecx
  47f866: 8b 44 24 18                  	mov	eax, dword ptr [esp + 24]
  47f86a: 53                           	push	ebx
  47f86b: 56                           	push	esi
  47f86c: 57                           	push	edi
  47f86d: 8b f1                        	mov	esi, ecx
  47f86f: 8b 4c 24 20                  	mov	ecx, dword ptr [esp + 32]
  47f873: 50                           	push	eax
  47f874: 51                           	push	ecx
  47f875: 8b ce                        	mov	ecx, esi
  47f877: 89 74 24 14                  	mov	dword ptr [esp + 20], esi
  47f87b: e8 f0 da ff ff               	call	0x47d370 <.text+0x7c370>
  47f880: 33 db                        	xor	ebx, ebx
  47f882: 8d 7e 34                     	lea	edi, [esi + 52]
  47f885: 53                           	push	ebx
  47f886: 8b cf                        	mov	ecx, edi
  47f888: 89 5c 24 1c                  	mov	dword ptr [esp + 28], ebx
  47f88c: e8 3f da fa ff               	call	0x42d2d0 <.text+0x2c2d0>
  47f891: c7 06 48 35 4e 00            	mov	dword ptr [esi], 5125448
  47f897: c7 07 30 fc 4d 00            	mov	dword ptr [edi], 5110832
  47f89d: 89 5e 7c                     	mov	dword ptr [esi + 124], ebx
  47f8a0: 89 9e 80 00 00 00            	mov	dword ptr [esi + 128], ebx
  47f8a6: 89 9e 84 00 00 00            	mov	dword ptr [esi + 132], ebx
  47f8ac: 89 9e 88 00 00 00            	mov	dword ptr [esi + 136], ebx
  47f8b2: 89 9e 8c 00 00 00            	mov	dword ptr [esi + 140], ebx
  47f8b8: 89 9e 90 00 00 00            	mov	dword ptr [esi + 144], ebx
  47f8be: 89 9e 94 00 00 00            	mov	dword ptr [esi + 148], ebx
  47f8c4: 89 9e 98 00 00 00            	mov	dword ptr [esi + 152], ebx
  47f8ca: 89 9e 9c 00 00 00            	mov	dword ptr [esi + 156], ebx
  47f8d0: 89 9e b4 00 00 00            	mov	dword ptr [esi + 180], ebx
  47f8d6: 89 9e b8 00 00 00            	mov	dword ptr [esi + 184], ebx
  47f8dc: 89 9e bc 00 00 00            	mov	dword ptr [esi + 188], ebx
  47f8e2: 89 9e c0 00 00 00            	mov	dword ptr [esi + 192], ebx
  47f8e8: 89 9e c4 00 00 00            	mov	dword ptr [esi + 196], ebx
  47f8ee: 89 9e c8 00 00 00            	mov	dword ptr [esi + 200], ebx
  47f8f4: 8b ce                        	mov	ecx, esi
  47f8f6: 89 9e b0 00 00 00            	mov	dword ptr [esi + 176], ebx
  47f8fc: e8 ff fb ff ff               	call	0x47f500 <.text+0x7e500>
  47f901: 8b 4c 24 10                  	mov	ecx, dword ptr [esp + 16]
  47f905: b8 a4 70 7d 3f               	mov	eax, 1065185444
  47f90a: 89 86 08 01 00 00            	mov	dword ptr [esi + 264], eax
  47f910: 89 86 2c 01 00 00            	mov	dword ptr [esi + 300], eax
  47f916: 89 86 30 01 00 00            	mov	dword ptr [esi + 304], eax
  47f91c: 89 86 54 01 00 00            	mov	dword ptr [esi + 340], eax
  47f922: 89 9e e4 00 00 00            	mov	dword ptr [esi + 228], ebx
  47f928: 89 9e e8 00 00 00            	mov	dword ptr [esi + 232], ebx
  47f92e: 89 9e 0c 01 00 00            	mov	dword ptr [esi + 268], ebx
  47f934: 89 9e 50 01 00 00            	mov	dword ptr [esi + 336], ebx
  47f93a: 88 9e ac 00 00 00            	mov	byte ptr [esi + 172], bl
  47f940: 5f                           	pop	edi
  47f941: 8b c6                        	mov	eax, esi
  47f943: 5e                           	pop	esi
  47f944: 5b                           	pop	ebx
  47f945: 64 89 0d 00 00 00 00         	mov	dword ptr fs:[0], ecx
  47f94c: 83 c4 10                     	add	esp, 16
  47f94f: c2 08 00                     	ret	8
  47f952: 90                           	nop
  47f953: 90                           	nop
  47f954: 90                           	nop
  47f955: 90                           	nop
  47f956: 90                           	nop
  47f957: 90                           	nop
  47f958: 90                           	nop
  47f959: 90                           	nop
  47f95a: 90                           	nop
  47f95b: 90                           	nop
  47f95c: 90                           	nop
  47f95d: 90                           	nop
  47f95e: 90                           	nop
  47f95f: 90                           	nop

# Sprite base constructor (0x0047d370..0x0047d3e0)
inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  47d370: 8b 44 24 04                  	mov	eax, dword ptr [esp + 4]
  47d374: 8a 54 24 08                  	mov	dl, byte ptr [esp + 8]
  47d378: c7 01 20 35 4e 00            	mov	dword ptr [ecx], 5125408
  47d37e: c7 41 24 00 00 00 00         	mov	dword ptr [ecx + 36], 0
  47d385: c7 41 28 00 00 00 00         	mov	dword ptr [ecx + 40], 0
  47d38c: c7 41 2c 00 00 00 00         	mov	dword ptr [ecx + 44], 0
  47d393: 56                           	push	esi
  47d394: 89 41 0c                     	mov	dword ptr [ecx + 12], eax
  47d397: 88 51 10                     	mov	byte ptr [ecx + 16], dl
  47d39a: 8b 40 18                     	mov	eax, dword ptr [eax + 24]
  47d39d: 8d 71 04                     	lea	esi, [ecx + 4]
  47d3a0: 56                           	push	esi
  47d3a1: 89 41 18                     	mov	dword ptr [ecx + 24], eax
  47d3a4: c7 06 00 00 00 00            	mov	dword ptr [esi], 0
  47d3aa: e8 01 ff ff ff               	call	0x47d2b0 <.text+0x7c2b0>
  47d3af: 8b 16                        	mov	edx, dword ptr [esi]
  47d3b1: 8d 41 08                     	lea	eax, [ecx + 8]
  47d3b4: 42                           	inc	edx
  47d3b5: 50                           	push	eax
  47d3b6: 89 10                        	mov	dword ptr [eax], edx
  47d3b8: e8 f3 fe ff ff               	call	0x47d2b0 <.text+0x7c2b0>
  47d3bd: a1 fc 2a 50 00               	mov	eax, dword ptr [5253884]
  47d3c2: 89 41 14                     	mov	dword ptr [ecx + 20], eax
  47d3c5: b8 00 00 80 3f               	mov	eax, 1065353216
  47d3ca: 89 41 1c                     	mov	dword ptr [ecx + 28], eax
  47d3cd: 89 41 20                     	mov	dword ptr [ecx + 32], eax
  47d3d0: c6 41 30 01                  	mov	byte ptr [ecx + 48], 1
  47d3d4: 8b c1                        	mov	eax, ecx
  47d3d6: 5e                           	pop	esi
  47d3d7: c2 08 00                     	ret	8
  47d3da: 90                           	nop
  47d3db: 90                           	nop
  47d3dc: 90                           	nop
  47d3dd: 90                           	nop
  47d3de: 90                           	nop
  47d3df: 90                           	nop

# Sprite animation initializer (0x0047f500..0x0047f5a0)
inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  47f500: 83 ec 0c                     	sub	esp, 12
  47f503: 53                           	push	ebx
  47f504: 56                           	push	esi
  47f505: 8b f1                        	mov	esi, ecx
  47f507: 33 db                        	xor	ebx, ebx
  47f509: 89 5e 74                     	mov	dword ptr [esi + 116], ebx
  47f50c: e8 1f de ff ff               	call	0x47d330 <.text+0x7c330>
  47f511: 68 00 00 80 3f               	push	1065353216
  47f516: 68 00 00 80 3f               	push	1065353216
  47f51b: 68 00 00 80 3f               	push	1065353216
  47f520: 8d 44 24 14                  	lea	eax, [esp + 20]
  47f524: 50                           	push	eax
  47f525: 8d 8e b4 00 00 00            	lea	ecx, [esi + 180]
  47f52b: c7 86 a8 00 00 00 00 00 80 3f	mov	dword ptr [esi + 168], 1065353216
  47f535: e8 46 dd fa ff               	call	0x42d280 <.text+0x2c280>
  47f53a: 53                           	push	ebx
  47f53b: 53                           	push	ebx
  47f53c: 53                           	push	ebx
  47f53d: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  47f541: 51                           	push	ecx
  47f542: 8d 4e 7c                     	lea	ecx, [esi + 124]
  47f545: e8 36 dd fa ff               	call	0x42d280 <.text+0x2c280>
  47f54a: 53                           	push	ebx
  47f54b: 53                           	push	ebx
  47f54c: 53                           	push	ebx
  47f54d: 8d 54 24 14                  	lea	edx, [esp + 20]
  47f551: 52                           	push	edx
  47f552: 8d 8e 88 00 00 00            	lea	ecx, [esi + 136]
  47f558: e8 23 dd fa ff               	call	0x42d280 <.text+0x2c280>
  47f55d: 53                           	push	ebx
  47f55e: 53                           	push	ebx
  47f55f: 53                           	push	ebx
  47f560: 8d 44 24 14                  	lea	eax, [esp + 20]
  47f564: 50                           	push	eax
  47f565: 8d 8e 94 00 00 00            	lea	ecx, [esi + 148]
  47f56b: e8 10 dd fa ff               	call	0x42d280 <.text+0x2c280>
  47f570: b0 01                        	mov	al, 1
  47f572: 88 9e a0 00 00 00            	mov	byte ptr [esi + 160], bl
  47f578: 89 9e a4 00 00 00            	mov	dword ptr [esi + 164], ebx
  47f57e: 88 9e cc 00 00 00            	mov	byte ptr [esi + 204], bl
  47f584: 88 9e d4 00 00 00            	mov	byte ptr [esi + 212], bl
  47f58a: 88 46 30                     	mov	byte ptr [esi + 48], al
  47f58d: 88 86 ad 00 00 00            	mov	byte ptr [esi + 173], al
  47f593: 88 46 78                     	mov	byte ptr [esi + 120], al
  47f596: 5e                           	pop	esi
  47f597: 5b                           	pop	ebx
  47f598: 83 c4 0c                     	add	esp, 12
  47f59b: c3                           	ret
  47f59c: 90                           	nop
  47f59d: 90                           	nop
  47f59e: 90                           	nop
  47f59f: 90                           	nop

# operator new wrapper (0x004a5a84..0x004a5aa8)
inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  4a5a84: 56                           	push	esi
  4a5a85: 8b 74 24 08                  	mov	esi, dword ptr [esp + 8]
  4a5a89: eb 10                        	jmp	0x4a5a9b <.text+0xa4a9b>
  4a5a8b: 56                           	push	esi
  4a5a8c: e8 e1 56 01 00               	call	0x4bb172 <.text+0xba172>
  4a5a91: 85 c0                        	test	eax, eax
  4a5a93: 59                           	pop	ecx
  4a5a94: 75 05                        	jne	0x4a5a9b <.text+0xa4a9b>
  4a5a96: e8 3f 3e 00 00               	call	0x4a98da <.text+0xa88da>
  4a5a9b: 56                           	push	esi
  4a5a9c: e8 30 23 01 00               	call	0x4b7dd1 <.text+0xb6dd1>
  4a5aa1: 85 c0                        	test	eax, eax
  4a5aa3: 59                           	pop	ecx
  4a5aa4: 74 e5                        	je	0x4a5a8b <.text+0xa4a8b>
  4a5aa6: 5e                           	pop	esi
  4a5aa7: c3                           	ret

# malloc wrappers and heap allocation (0x004b7d5f..0x004b7de8)
inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  4b7d5f: 83 3d e4 42 50 00 03         	cmp	dword ptr [5260004], 3
  4b7d66: 56                           	push	esi
  4b7d67: 8b 74 24 08                  	mov	esi, dword ptr [esp + 8]
  4b7d6b: 75 13                        	jne	0x4b7d80 <.text+0xb6d80>
  4b7d6d: 3b 35 d0 42 50 00            	cmp	esi, dword ptr [5259984]
  4b7d73: 77 0b                        	ja	0x4b7d80 <.text+0xb6d80>
  4b7d75: 56                           	push	esi
  4b7d76: e8 2f 76 00 00               	call	0x4bf3aa <.text+0xbe3aa>
  4b7d7b: 85 c0                        	test	eax, eax
  4b7d7d: 59                           	pop	ecx
  4b7d7e: 75 23                        	jne	0x4b7da3 <.text+0xb6da3>
  4b7d80: 85 f6                        	test	esi, esi
  4b7d82: 75 01                        	jne	0x4b7d85 <.text+0xb6d85>
  4b7d84: 46                           	inc	esi
  4b7d85: 83 3d e4 42 50 00 01         	cmp	dword ptr [5260004], 1
  4b7d8c: 74 06                        	je	0x4b7d94 <.text+0xb6d94>
  4b7d8e: 83 c6 0f                     	add	esi, 15
  4b7d91: 83 e6 f0                     	and	esi, -16
  4b7d94: 56                           	push	esi
  4b7d95: 6a 00                        	push	0
  4b7d97: ff 35 e0 42 50 00            	push	dword ptr [5260000]
  4b7d9d: ff 15 4c c3 4d 00            	call	dword ptr [5096268]
  4b7da3: 5e                           	pop	esi
  4b7da4: c3                           	ret
  4b7da5: 83 7c 24 04 e0               	cmp	dword ptr [esp + 4], -32
  4b7daa: 77 22                        	ja	0x4b7dce <.text+0xb6dce>
  4b7dac: ff 74 24 04                  	push	dword ptr [esp + 4]
  4b7db0: e8 aa ff ff ff               	call	0x4b7d5f <.text+0xb6d5f>
  4b7db5: 85 c0                        	test	eax, eax
  4b7db7: 59                           	pop	ecx
  4b7db8: 75 16                        	jne	0x4b7dd0 <.text+0xb6dd0>
  4b7dba: 39 44 24 08                  	cmp	dword ptr [esp + 8], eax
  4b7dbe: 74 10                        	je	0x4b7dd0 <.text+0xb6dd0>
  4b7dc0: ff 74 24 04                  	push	dword ptr [esp + 4]
  4b7dc4: e8 a9 33 00 00               	call	0x4bb172 <.text+0xba172>
  4b7dc9: 85 c0                        	test	eax, eax
  4b7dcb: 59                           	pop	ecx
  4b7dcc: 75 de                        	jne	0x4b7dac <.text+0xb6dac>
  4b7dce: 33 c0                        	xor	eax, eax
  4b7dd0: c3                           	ret
  4b7dd1: ff 35 64 3d 50 00            	push	dword ptr [5258596]
  4b7dd7: ff 74 24 08                  	push	dword ptr [esp + 8]
  4b7ddb: e8 c5 ff ff ff               	call	0x4b7da5 <.text+0xb6da5>
  4b7de0: 59                           	pop	ecx
  4b7de1: 59                           	pop	ecx
  4b7de2: c3                           	ret
  4b7de3: 55                           	push	ebp
  4b7de4: 8b ec                        	mov	ebp, esp
  4b7de6: 83 ec 20                     	sub	esp, 32
