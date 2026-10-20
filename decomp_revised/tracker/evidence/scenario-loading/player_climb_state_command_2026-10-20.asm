; Read-only Ghidra 12.0.4 instruction/data inspection.
; Retail input: inputs/original/binaries/FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Saved FDNYPort project, -readOnly -noanalysis, JDK 21.

; Player ScriptActor handler compares the parsed token with the string at
; 004E1BE4, then loops over Player command recipients and calls 0044C0B0.
00451b54  CALL 0x0040c330
00451b59  MOV EAX,dword ptr [EAX]
00451b5b  MOV EAX,dword ptr [EAX]
00451b5d  ADD EAX,0xc
00451b60  PUSH 0x004e1be4
00451b65  PUSH EAX
00451b66  CALL 0x004ca140
00451b78  JZ  0x00451ba6
00451b83  LEA EBX,[EBP + 0xfffffec0]
00451b90  MOV ECX,EBX
00451b92  CALL 0x0044c0b0

; Ghidra data lookup: 004E1BE4 is "climb" (ASCII 63 6C 69 6D 62 00),
; and the reference at 00451B60 points to it.

; The adjacent branch compares "unclimb" at 004E1BDC. If Player root state
; +0x18C is 5, it changes it to 3; other state values are left alone.
00451ba6  PUSH 0x004e1bdc
00451bb2  CALL 0x0040c330
00451bb9  CALL 0x0040c360
00451bbe  TEST AL,AL
00451bc0  JZ  0x00451bec
00451bcb  MOV EDX,0x5
00451bd0  MOV ECX,0x3
00451bd5  CMP dword ptr [EBP + 0x4c],EDX
00451bd8  JNZ 0x00451bdd
00451bda  MOV dword ptr [EBP + 0x4c],ECX

; Ghidra data lookup: 004E1BDC is "unclimb" (ASCII 75 6E 63 6C 69 6D 62 00).
; This resolves the state-5 command token. The separate Player update JUMP
; action query at 00453739 is state-4-only and calls 0044BCE0, not the
; 0044E990 jump-stat/velocity routine at 00453784. See
; player_jump_action_state4_2026-10-20.asm.
