; Ghidra 12.0.4 read-only instruction capture from saved FDNYPort project.
; Retail input: FDNYFirefighter.exe, SHA-256:
; 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Address 0049C820 is not a defined function in this project snapshot.
; Vtable 004DF8E8 slot +0x04 points here; owner +0x3C4 reaches that slot.
; This body drains one queued item per iteration and calls item slot +0 or +0x2C.

0049c820: PUSH EBP
0049c821: PUSH ESI
0049c822: PUSH EDI
0049c823: LEA EDI,[ECX + -0x40]
0049c826: MOV EAX,dword ptr [EDI + 0x10]
0049c829: TEST EAX,EAX
0049c82b: JZ 0x0049c87b
0049c82d: CMP dword ptr [EAX],0x0
0049c830: JZ 0x0049c87b
0049c832: MOV ESI,EAX
0049c834: TEST ESI,ESI
0049c836: JNZ 0x0049c83c
0049c838: XOR ECX,ECX
0049c83a: JMP 0x0049c865
0049c83c: MOV EAX,dword ptr [ESI + 0x4]
0049c83f: TEST EAX,EAX
0049c841: MOV EBP,dword ptr [ESI]
0049c843: MOV dword ptr [EDI + 0x10],EAX
0049c846: JNZ 0x0049c84b
0049c848: MOV dword ptr [EDI + 0x14],EAX
0049c84b: PUSH ESI
0049c84c: MOV ECX,EDI
0049c84e: CALL 0x00429d20
0049c853: PUSH ESI
0049c854: CALL 0x004b7b63
0049c859: MOV EAX,dword ptr [EDI + 0x18]
0049c85c: ADD ESP,0x4
0049c85f: DEC EAX
0049c860: MOV dword ptr [EDI + 0x18],EAX
0049c863: MOV ECX,EBP
0049c865: MOV AL,byte ptr [ECX + 0x1d]
0049c868: TEST AL,AL
0049c86a: JNZ 0x0049c874
0049c86c: MOV EAX,dword ptr [ECX]
0049c86e: PUSH 0x1
0049c870: CALL dword ptr [EAX]
0049c872: JMP 0x0049c826
0049c874: MOV EDX,dword ptr [ECX]
0049c876: CALL dword ptr [EDX + 0x2c]
0049c879: JMP 0x0049c826
0049c87b: POP EDI
0049c87c: POP ESI
0049c87d: POP EBP
0049c87e: RET 0x8
