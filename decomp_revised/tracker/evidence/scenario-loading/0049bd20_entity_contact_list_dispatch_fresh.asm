; Fresh Ghidra instruction listing captured 2026-10-13.
; Binary: FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Address: 0049BD20 (DispatchEntityContactLists)
; Analysis output, not recovered original source.
0049bd20: SUB ESP,0x8 
0049bd23: MOV EAX,dword ptr [ECX + 0x14] 
0049bd26: PUSH EBX 
0049bd27: PUSH EBP 
0049bd28: PUSH ESI 
0049bd29: PUSH EDI 
0049bd2a: LEA EDI,[ECX + 0x4] 
0049bd2d: XOR EBX,EBX 
0049bd2f: MOV dword ptr [ESP + 0x10],ECX 
0049bd33: MOV dword ptr [EDI + 0x4],EAX 
0049bd36: MOV dword ptr [EDI + 0x8],EBX 
0049bd39: MOV byte ptr [EDI + 0xc],BL 
0049bd3c: MOV byte ptr [EDI + 0xd],0x1 
0049bd40: MOV EAX,dword ptr [EDI + 0x4] 
0049bd43: CMP EAX,EBX 
0049bd45: JZ 0x0049bdf5 
0049bd4b: CMP byte ptr [EDI + 0xd],BL 
0049bd4e: JZ 0x0049bd59 
0049bd50: MOV ECX,EAX 
0049bd52: MOV byte ptr [EDI + 0xd],BL 
0049bd55: MOV EBP,dword ptr [ECX] 
0049bd57: JMP 0x0049bd7a 
0049bd59: CMP byte ptr [EDI + 0xc],BL 
0049bd5c: JZ 0x0049bd68 
0049bd5e: MOV EDX,dword ptr [EDI + 0x4] 
0049bd61: MOV byte ptr [EDI + 0xc],BL 
0049bd64: MOV EBP,dword ptr [EDX] 
0049bd66: JMP 0x0049bd7a 
0049bd68: MOV dword ptr [EDI + 0x8],EAX 
0049bd6b: MOV EAX,dword ptr [EDI + 0x4] 
0049bd6e: MOV EAX,dword ptr [EAX + 0x4] 
0049bd71: CMP EAX,EBX 
0049bd73: MOV dword ptr [EDI + 0x4],EAX 
0049bd76: JZ 0x0049bdf5 
0049bd78: MOV EBP,dword ptr [EAX] 
0049bd7a: CMP EBP,EBX 
0049bd7c: JZ 0x0049bdf5 
0049bd7e: MOV EDX,dword ptr [EBP] 
0049bd81: MOV ECX,EBP 
0049bd83: CALL dword ptr [EDX + 0x4] 
0049bd86: TEST AL,AL 
0049bd88: JZ 0x0049bd40 
0049bd8a: MOV ESI,dword ptr [EDI + 0x4] 
0049bd8d: MOV ECX,dword ptr [EDI + 0x8] 
0049bd90: CMP ECX,EBX 
0049bd92: MOV EAX,dword ptr [ESI + 0x4] 
0049bd95: MOV dword ptr [EDI + 0x4],EAX 
0049bd98: JZ 0x0049bd9d 
0049bd9a: MOV dword ptr [ECX + 0x4],EAX 
0049bd9d: CMP dword ptr [EDI + 0x10],ESI 
0049bda0: JNZ 0x0049bda5 
0049bda2: MOV dword ptr [EDI + 0x10],EAX 
0049bda5: CMP dword ptr [EDI + 0x14],ESI 
0049bda8: JNZ 0x0049bdb0 
0049bdaa: MOV EAX,dword ptr [EDI + 0x8] 
0049bdad: MOV dword ptr [EDI + 0x14],EAX 
0049bdb0: CMP dword ptr [EDI + 0x14],EBX 
0049bdb3: JNZ 0x0049bdbf 
0049bdb5: MOV EAX,dword ptr [EDI + 0x10] 
0049bdb8: CMP EAX,EBX 
0049bdba: JZ 0x0049bdbf 
0049bdbc: MOV dword ptr [EDI + 0x14],EAX 
0049bdbf: PUSH ESI 
0049bdc0: MOV ECX,EDI 
0049bdc2: CALL 0x00429d20 
0049bdc7: PUSH ESI 
0049bdc8: CALL 0x004b7b63 
0049bdcd: MOV ESI,dword ptr [EDI + 0x18] 
0049bdd0: MOV EDX,dword ptr [ESP + 0x14] 
0049bdd4: ADD ESP,0x4 
0049bdd7: DEC ESI 
0049bdd8: MOV dword ptr [EDI + 0x18],ESI 
0049bddb: MOV byte ptr [EDI + 0xc],0x1 
0049bddf: MOV EAX,dword ptr [EBP + 0x18] 
0049bde2: LEA ECX,[EAX + EAX*0x8] 
0049bde5: MOV EAX,dword ptr [EDX + ECX*0x8 + 0x4c] 
0049bde9: LEA ECX,[EDX + ECX*0x8 + 0x4c] 
0049bded: PUSH EBP 
0049bdee: CALL dword ptr [EAX] 
0049bdf0: JMP 0x0049bd40 
0049bdf5: MOV ECX,dword ptr [EDI + 0x10] 
0049bdf8: MOV dword ptr [EDI + 0x4],ECX 
0049bdfb: MOV dword ptr [EDI + 0x8],EBX 
0049bdfe: MOV byte ptr [EDI + 0xc],BL 
0049be01: MOV byte ptr [EDI + 0xd],0x1 
0049be05: MOV EAX,dword ptr [EDI + 0x4] 
0049be08: CMP EAX,EBX 
0049be0a: JZ 0x0049becd 
0049be10: CMP byte ptr [EDI + 0xd],BL 
0049be13: JZ 0x0049be1e 
0049be15: MOV EDX,EAX 
0049be17: MOV byte ptr [EDI + 0xd],BL 
0049be1a: MOV EBP,dword ptr [EDX] 
0049be1c: JMP 0x0049be41 
0049be1e: CMP byte ptr [EDI + 0xc],BL 
0049be21: JZ 0x0049be2b 
0049be23: MOV EAX,dword ptr [EDI + 0x4] 
0049be26: MOV byte ptr [EDI + 0xc],BL 
0049be29: JMP 0x0049be3f 
0049be2b: MOV ECX,dword ptr [EDI + 0x4] 
0049be2e: MOV dword ptr [EDI + 0x8],EAX 
0049be31: MOV EAX,dword ptr [ECX + 0x4] 
0049be34: CMP EAX,EBX 
0049be36: MOV dword ptr [EDI + 0x4],EAX 
0049be39: JZ 0x0049becd 
0049be3f: MOV EBP,dword ptr [EAX] 
0049be41: CMP EBP,EBX 
0049be43: JZ 0x0049becd 
0049be49: MOV ESI,dword ptr [ESP + 0x10] 
0049be4d: XOR ECX,ECX 
0049be4f: MOV dword ptr [ESP + 0x14],ECX 
0049be53: ADD ESI,0x54 
0049be56: MOV EAX,dword ptr [EBP + 0x20] 
0049be59: MOV EDX,0x1 
0049be5e: SHL EDX,CL 
0049be60: TEST EAX,EDX 
0049be62: JZ 0x0049bebb 
0049be64: MOV EAX,dword ptr [ESI + 0x8] 
0049be67: MOV dword ptr [ESI + -0x4],EAX 
0049be6a: MOV dword ptr [ESI],EBX 
0049be6c: MOV byte ptr [ESI + 0x4],BL 
0049be6f: MOV byte ptr [ESI + 0x5],0x1 
0049be73: MOV EAX,dword ptr [ESI + -0x4] 
0049be76: CMP EAX,EBX 
0049be78: JZ 0x0049beb7 
0049be7a: CMP byte ptr [ESI + 0x5],BL 
0049be7d: JZ 0x0049be88 
0049be7f: MOV ECX,EAX 
0049be81: MOV byte ptr [ESI + 0x5],BL 
0049be84: MOV EAX,dword ptr [ECX] 
0049be86: JMP 0x0049bea8 
0049be88: CMP byte ptr [ESI + 0x4],BL 
0049be8b: JZ 0x0049be97 
0049be8d: MOV EDX,dword ptr [ESI + -0x4] 
0049be90: MOV byte ptr [ESI + 0x4],BL 
0049be93: MOV EAX,dword ptr [EDX] 
0049be95: JMP 0x0049bea8 
0049be97: MOV dword ptr [ESI],EAX 
0049be99: MOV EAX,dword ptr [ESI + -0x4] 
0049be9c: MOV EAX,dword ptr [EAX + 0x4] 
0049be9f: CMP EAX,EBX 
0049bea1: MOV dword ptr [ESI + -0x4],EAX 
0049bea4: JZ 0x0049beb7 
0049bea6: MOV EAX,dword ptr [EAX] 
0049bea8: CMP EAX,EBX 
0049beaa: JZ 0x0049beb7 
0049beac: MOV EDX,dword ptr [EBP] 
0049beaf: PUSH EAX 
0049beb0: MOV ECX,EBP 
0049beb2: CALL dword ptr [EDX + 0x48] 
0049beb5: JMP 0x0049be73 
0049beb7: MOV ECX,dword ptr [ESP + 0x14] 
0049bebb: INC ECX 
0049bebc: ADD ESI,0x48 
0049bebf: CMP ECX,0x9 
0049bec2: MOV dword ptr [ESP + 0x14],ECX 
0049bec6: JL 0x0049be56 
0049bec8: JMP 0x0049be05 
0049becd: POP EDI 
0049bece: POP ESI 
0049becf: POP EBP 
0049bed0: POP EBX 
0049bed1: ADD ESP,0x8 
0049bed4: RET 0x8 

