; FDNYFirefighter.exe SHA-256:
; 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Ghidra 12.0.4, read-only, noanalysis. Extracted mode-2 paths only.

; RCS_MiscEmitter C_Type 5 dispatch in constructor 00442900:
00442a2d  CMP EAX,0x5
00442a32  PUSH 0xa8
00442a3c  MOV dword ptr [ESP + 0x78],EAX
00442a4c  PUSH 0x2
00442a50  CALL 0x00445600
00442a8c  MOV dword ptr [ESI + 0x78],EAX

; Internal emitter constructor stores its mode in +0x64:
0044566a  MOV EAX,dword ptr [ESP + 0x18]
00445672  MOV dword ptr [ESI + 0x64],EAX

; Internal mode 2 child setup in 00445720:
0044573c  MOV EAX,dword ptr [EBP + 0x64]
0044573f  CMP EAX,0x2
00445748  JNZ 0x004457f3
0044574e  ADD EBP,0x4c
00445751  MOV EBX,0x4
00445756  PUSH 0x168
0044575b  CALL 0x004a5a84
00445771  PUSH EDI
00445772  PUSH 0x502d40
00445777  MOV ECX,EAX
00445779  CALL 0x0047f850
00445784  MOV dword ptr [ESI + 0xa4],0xa0
0044578e  MOV byte ptr [ESI + 0xad],0x1
00445795  MOV dword ptr [ESI + 0xb0],0x4
004457d0  JNZ 0x00445756
004457d9  MOV dword ptr [EDX + 0x80],0x0

; Mode-2 activation/deactivation handlers reached via internal vtable:
00444c90  MOV EDX,dword ptr [ECX + 0x64]
00444c9a  MOV byte ptr [ECX + 0x45],AL
00444caa  RET
00444cb0  CMP dword ptr [ECX + 0x64],0x1
00444cb4  MOV byte ptr [ECX + 0x45],0x0
00444cc5  RET

; Update callback begins with the active-byte gate, then selects mode 2:
00444d00  SUB ESP,0xdc
00444d09  MOV AL,byte ptr [EBX + 0x45]
00444d0c  TEST AL,AL
00444d0e  JZ 0x00445482
00444d23  MOV EAX,dword ptr [EBX + 0x64]
00444d26  CMP EAX,0x2
00444d29  JNZ 0x0044510d
