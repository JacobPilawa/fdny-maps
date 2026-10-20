; Static instruction inspection against FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Address: 0049C3D0; this is stored as slot +4 in owner +0x50 manager vtable 004E4164.
; The function is data-bound in the vtable; the earlier direct code-xref query
; did not find callers because the manager reaches it through its vtable.
; A fresh headless analysis copy initially had no function at or containing
; this address. After confirming its 12-byte body and RET 4 boundary, an
; explicit function was defined in the disposable project and exported to the
; current function inventory. The primary saved Ghidra project was not changed.

0049c3d0  MOV ECX, [ESP+0x4]
0049c3d4  MOV EAX, [ECX]
0049c3d6  CALL DWORD PTR [EAX+0x14]
0049c3d9  RET 0x4
