; Fresh Ghidra disassembly captured 2026-10-13.
; Binary: FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Address: 0043AFF0 (CheckBModelReachExpansion)
; Listing output, not original source.
0043aff0: MOV EAX,dword ptr [ECX + 0xec] 
0043aff6: MOV EDX,dword ptr [EAX + 0x34] 
0043aff9: TEST EDX,EDX 
0043affb: JZ 0x0043b024 
0043affd: MOV EDX,dword ptr [EAX + 0x38] 
0043b000: TEST EDX,EDX 
0043b002: JNZ 0x0043b024 
0043b004: FLD float ptr [ECX + 0x1ec] 
0043b00a: FCOM double ptr [0x004e0b68] 
0043b010: FNSTSW AX 
0043b012: TEST AH,0x5 
0043b015: JP 0x0043b027 
0043b017: FCOMP double ptr [0x004e0b60] 
0043b01d: FNSTSW AX 
0043b01f: TEST AH,0x41 
0043b022: JNZ 0x0043b029 
0043b024: XOR AL,AL 
0043b026: RET 
0043b027: FSTP ST0 
0043b029: MOV AL,0x1 
0043b02b: RET
