; Wait duration parse and absolute deadline calculation inside HandleCommonScriptActorCommand.
; Fresh Ghidra 12.0.4 disassembly from FDNYFirefighter.exe, SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.
; DAT_004E0ED4 is float 1000.0 (raw bytes 00 00 7A 44).

0046fb98: MOV ECX,ESI 
0046fb9a: CALL 0x00473d20 
0046fb9f: LEA ECX,[ESP + 0x14] 
0046fba3: PUSH ECX 
0046fba4: LEA ECX,[ESP + 0x30] 
0046fba8: CALL 0x0040cc50 
0046fbad: TEST AL,AL 
0046fbaf: MOV EAX,dword ptr [ESP + 0x2c] 
0046fbb3: MOV dword ptr [ESP + 0x24],EBX 
0046fbb7: JNZ 0x0046fbd9 
0046fbb9: CMP EAX,EBP 
0046fbbb: JZ 0x0046fc2c 
0046fbbd: DEC dword ptr [EAX] 
0046fbbf: MOV EDX,dword ptr [ESP + 0x2c] 
0046fbc3: CMP dword ptr [EDX],0x0 
0046fbc6: JLE 0x0046fbce 
0046fbc8: MOV dword ptr [ESP + 0x2c],EBP 
0046fbcc: JMP 0x0046fc2c 
0046fbce: LEA ECX,[ESP + 0x2c] 
0046fbd2: CALL 0x004011c0 
0046fbd7: JMP 0x0046fc2c 
0046fbd9: CMP EAX,EBP 
0046fbdb: JZ 0x0046fbf7 
0046fbdd: DEC dword ptr [EAX] 
0046fbdf: MOV EAX,dword ptr [ESP + 0x2c] 
0046fbe3: CMP dword ptr [EAX],0x0 
0046fbe6: JLE 0x0046fbee 
0046fbe8: MOV dword ptr [ESP + 0x2c],EBP 
0046fbec: JMP 0x0046fbf7 
0046fbee: LEA ECX,[ESP + 0x2c] 
0046fbf2: CALL 0x004011c0 
0046fbf7: FLD float ptr [ESP + 0x14] 
0046fbfb: MOV ECX,dword ptr [0x00502afc] 
0046fc01: TEST ECX,ECX 
0046fc03: FMUL float ptr [0x004e0ed4] 
0046fc09: FILD dword ptr [0x00502afc] 
0046fc0f: JGE 0x0046fc17 
0046fc11: FADD float ptr [0x004dfa30] 
0046fc17: FADDP 
0046fc19: CALL 0x004b9400 
0046fc1e: MOV ECX,dword ptr [ESP + 0x10] 
0046fc22: MOV dword ptr [ECX + 0x48],EAX 
0046fc25: MOV dword ptr [ECX + 0x4c],0x1 
