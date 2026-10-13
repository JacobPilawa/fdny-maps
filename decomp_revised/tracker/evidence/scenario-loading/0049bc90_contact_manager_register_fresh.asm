; Fresh Ghidra instruction listing captured 2026-10-13.
; Binary: FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Address: 0049BC90 (RegisterContactManagerEntity)
; Analysis output, not recovered original source.
0049bc90: PUSH ESI 
0049bc91: PUSH EDI 
0049bc92: MOV EDI,dword ptr [ESP + 0xc] 
0049bc96: MOV EAX,dword ptr [EDI + 0x18] 
0049bc99: INC EAX 
0049bc9a: MOV ESI,ECX 
0049bc9c: LEA EAX,[EAX + EAX*0x8] 
0049bc9f: MOV EDX,dword ptr [ESI + EAX*0x8] 
0049bca2: LEA ECX,[ESI + EAX*0x8] 
0049bca5: PUSH 0x1 
0049bca7: PUSH EDI 
0049bca8: CALL dword ptr [EDX + 0x4] 
0049bcab: MOV EAX,dword ptr [ESI + 0x18] 
0049bcae: INC EAX 
0049bcaf: PUSH 0xc 
0049bcb1: MOV dword ptr [ESI + 0x18],EAX 
0049bcb4: CALL 0x004a5a84 
0049bcb9: MOV dword ptr [EAX + 0x4],0x0 
0049bcc0: MOV dword ptr [EAX],EDI 
0049bcc2: MOV byte ptr [EAX + 0x8],0x1 
0049bcc6: MOV ECX,dword ptr [ESI + 0x14] 
0049bcc9: ADD ESP,0x4 
0049bccc: TEST ECX,ECX 
0049bcce: JNZ 0x0049bcdb 
0049bcd0: POP EDI 
0049bcd1: MOV dword ptr [ESI + 0x10],EAX 
0049bcd4: MOV dword ptr [ESI + 0x14],EAX 
0049bcd7: POP ESI 
0049bcd8: RET 0x8 
0049bcdb: MOV dword ptr [ECX + 0x4],EAX 
0049bcde: POP EDI 
0049bcdf: MOV dword ptr [ESI + 0x14],EAX 
0049bce2: POP ESI 
0049bce3: RET 0x8 

