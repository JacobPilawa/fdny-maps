
inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

Disassembly of section .text:

00401000 <.text>:
  475310: 64 a1 00 00 00 00            	mov	eax, dword ptr fs:[0]
  475316: 6a ff                        	push	-1
  475318: 68 d8 2e 4d 00               	push	5058264
  47531d: 50                           	push	eax
  47531e: 64 89 25 00 00 00 00         	mov	dword ptr fs:[0], esp
  475325: 56                           	push	esi
  475326: 8b 44 24 18                  	mov	eax, dword ptr [esp + 24]
  47532a: 89 41 78                     	mov	dword ptr [ecx + 120], eax
  47532d: 8a 15 18 19 50 00            	mov	dl, byte ptr [5249304]
  475333: 84 d2                        	test	dl, dl
  475335: 6a 00                        	push	0
  475337: 51                           	push	ecx
  475338: c7 44 24 14 00 00 00 00      	mov	dword ptr [esp + 20], 0
  475340: 89 64 24 20                  	mov	dword ptr [esp + 32], esp
  475344: 8b c4                        	mov	eax, esp
  475346: 75 2c                        	jne	0x475374 <.text+0x74374>
  475348: ba 01 00 00 00               	mov	edx, 1
  47534d: 88 15 18 19 50 00            	mov	byte ptr [5249304], dl
  475353: c7 05 08 19 50 00 02 00 00 00	mov	dword ptr [5249288], 2
  47535d: c7 05 0c 19 50 00 00 00 00 00	mov	dword ptr [5249292], 0
  475367: 89 15 10 19 50 00            	mov	dword ptr [5249296], edx
  47536d: c6 05 14 19 50 00 00         	mov	byte ptr [5249300], 0
  475374: 8b 74 24 1c                  	mov	esi, dword ptr [esp + 28]
  475378: 81 fe 08 19 50 00            	cmp	esi, 5249288
  47537e: c7 00 08 19 50 00            	mov	dword ptr [eax], 5249288
  475384: 74 04                        	je	0x47538a <.text+0x7438a>
  475386: 89 30                        	mov	dword ptr [eax], esi
  475388: ff 06                        	inc	dword ptr [esi]
  47538a: e8 c1 ad ff ff               	call	0x470150 <.text+0x6f150>
  47538f: 81 fe 08 19 50 00            	cmp	esi, 5249288
  475395: c7 44 24 0c ff ff ff ff      	mov	dword ptr [esp + 12], 4294967295
  47539d: 74 14                        	je	0x4753b3 <.text+0x743b3>
  47539f: 8b 0e                        	mov	ecx, dword ptr [esi]
  4753a1: 49                           	dec	ecx
  4753a2: 8b c1                        	mov	eax, ecx
  4753a4: 85 c0                        	test	eax, eax
  4753a6: 89 0e                        	mov	dword ptr [esi], ecx
  4753a8: 7f 09                        	jg	0x4753b3 <.text+0x743b3>
  4753aa: 8d 4c 24 14                  	lea	ecx, [esp + 20]
  4753ae: e8 0d be f8 ff               	call	0x4011c0 <.text+0x1c0>
  4753b3: 8b 4c 24 04                  	mov	ecx, dword ptr [esp + 4]
  4753b7: 64 89 0d 00 00 00 00         	mov	dword ptr fs:[0], ecx
  4753be: 5e                           	pop	esi
  4753bf: 83 c4 0c                     	add	esp, 12
  4753c2: c2 08 00                     	ret	8
