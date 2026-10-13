; Fresh Ghidra instruction listing captured 2026-10-13.
; Binary: FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Address: 00440D60 (InitializeBoundsTriggerFactoryDescriptor)
; Analysis output, not recovered original source.
00440d60: PUSH -0x1 
00440d62: PUSH 0x4ceef8 
00440d67: MOV EAX,FS:[0x0] 
00440d6d: PUSH EAX 
00440d6e: MOV dword ptr FS:[0x0],ESP 
00440d75: PUSH ECX 
00440d76: PUSH ESI 
00440d77: MOV ESI,ECX 
00440d79: PUSH EDI 
00440d7a: LEA EDI,[ESI + 0x4] 
00440d7d: MOV ECX,EDI 
00440d7f: MOV dword ptr [ESP + 0x8],ESI 
00440d83: MOV dword ptr [ESI],0x4e0b88 
00440d89: CALL 0x0043aee0 
00440d8e: PUSH 0x64 
00440d90: PUSH 0x4e0fa8 
00440d95: MOV ECX,EDI 
00440d97: MOV dword ptr [ESP + 0x1c],0x0 
00440d9f: MOV dword ptr [ESI],0x4e0f98 
00440da5: CALL 0x0049b690 
00440daa: MOV ECX,dword ptr [ESP + 0xc] 
00440dae: POP EDI 
00440daf: MOV EAX,ESI 
00440db1: POP ESI 
00440db2: MOV dword ptr FS:[0x0],ECX 
00440db9: ADD ESP,0x10 
00440dbc: RET 

