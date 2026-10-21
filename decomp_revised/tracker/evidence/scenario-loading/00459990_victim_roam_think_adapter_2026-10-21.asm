
inputs/original/binaries/FDNYFirefighter.exe:	file format coff-i386

; Retail SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Ghidra vtable entry: VictimRescue primary vtable 004E25B8 + 0x14 = 00459990.

Disassembly of section .text:

00401000 <.text>:
  459990: 8a 41 10                     	mov	al, byte ptr [ecx + 16]
  459993: 84 c0                        	test	al, al
  459995: 75 0b                        	jne	0x4599a2 <.text+0x589a2>
  459997: 81 c1 d4 00 00 00            	add	ecx, 212
  45999d: e9 9e 6d 01 00               	jmp	0x470740 <.text+0x6f740>
  4599a2: c3                           	ret
  4599a3: 90                           	nop
