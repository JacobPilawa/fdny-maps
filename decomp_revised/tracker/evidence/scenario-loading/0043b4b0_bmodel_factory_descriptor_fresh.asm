; Fresh Ghidra instruction listing captured 2026-10-13.
; Binary: FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Address: 0043B4B0 (InitializeBModelFactoryDescriptor)
; Analysis output, not recovered original source.
0043b4b0: PUSH -0x1 
0043b4b2: PUSH 0x4ce9b8 
0043b4b7: MOV EAX,FS:[0x0] 
0043b4bd: PUSH EAX 
0043b4be: MOV dword ptr FS:[0x0],ESP 
0043b4c5: PUSH ECX 
0043b4c6: PUSH ESI 
0043b4c7: MOV ESI,ECX 
0043b4c9: PUSH EDI 
0043b4ca: LEA EDI,[ESI + 0x4] 
0043b4cd: MOV ECX,EDI 
0043b4cf: MOV dword ptr [ESP + 0x8],ESI 
0043b4d3: MOV dword ptr [ESI],0x4e0b88 
0043b4d9: CALL 0x0043aee0 
0043b4de: PUSH 0x64 
0043b4e0: PUSH 0x4e0ba8 
0043b4e5: MOV ECX,EDI 
0043b4e7: MOV dword ptr [ESP + 0x1c],0x0 
0043b4ef: MOV dword ptr [ESI],0x4e0b98 
0043b4f5: CALL 0x0049b690 
0043b4fa: MOV ECX,dword ptr [ESP + 0xc] 
0043b4fe: POP EDI 
0043b4ff: MOV EAX,ESI 
0043b501: POP ESI 
0043b502: MOV dword ptr FS:[0x0],ECX 
0043b509: ADD ESP,0x10 
0043b50c: RET 

