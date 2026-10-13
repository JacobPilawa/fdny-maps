; Fresh Ghidra 12.0.4 listing from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e; captured 2026-10-13.
0042b1a0: PUSH ESI
0042b1a1: PUSH EDI
0042b1a2: MOV EDI,dword ptr [ESP + 0xc]
0042b1a6: TEST EDI,EDI
0042b1a8: MOV ESI,ECX
0042b1aa: JZ 0x0042b1e5
0042b1ac: MOV EAX,dword ptr [ESI + 0x18]
0042b1af: INC EAX
0042b1b0: PUSH 0xc
0042b1b2: MOV dword ptr [ESI + 0x18],EAX
0042b1b5: CALL 0x004a5a84
0042b1ba: MOV CL,byte ptr [ESP + 0x14]
0042b1be: MOV dword ptr [EAX + 0x4],0x0
0042b1c5: MOV dword ptr [EAX],EDI
0042b1c7: MOV byte ptr [EAX + 0x8],CL
0042b1ca: MOV ECX,dword ptr [ESI + 0x14]
0042b1cd: ADD ESP,0x4
0042b1d0: TEST ECX,ECX
0042b1d2: JNZ 0x0042b1df
0042b1d4: POP EDI
0042b1d5: MOV dword ptr [ESI + 0x10],EAX
0042b1d8: MOV dword ptr [ESI + 0x14],EAX
0042b1db: POP ESI
0042b1dc: RET 0x8
0042b1df: MOV dword ptr [ECX + 0x4],EAX
0042b1e2: MOV dword ptr [ESI + 0x14],EAX
0042b1e5: POP EDI
0042b1e6: POP ESI
0042b1e7: RET 0x8
