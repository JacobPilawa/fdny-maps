; Read-only Ghidra 12.0.4 instruction capture from the pinned retail EXE.
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Program: .local/mission-lifecycle-ghidra/FDNYPort/FDNYFirefighter.exe
; The Player movement block receives ESI = Player+0x140 and sets EDI = root.

; State-not-4 branch evaluates the jump transaction first.
0045377e  LEA ECX,[ESI - 0x140]
00453784  CALL 0x0044e990

; The root vtable slot +0x64 calls wrapper 0043F680 (root vtable address point
; 004E1C48, slot 004E1CAC). The wrapper calls 0049DBC0 and clears root +0x130.
004537ca  MOV EAX,dword ptr [EDI]
004537cc  CALL dword ptr [EAX + 0x64]

; When movement-subobject byte +0x240 is clear, collision handling follows
; the integrator callback; accepted responses can call 0049E0A0 and set root +0x12C.
004537cf  MOV AL,byte ptr [ESI + 0x240]
004537d5  TEST AL,AL
004537d7  JNZ 0x004537e0
004537d9  MOV ECX,EDI
004537db  CALL 0x0043f6d0

; Within this Player movement pass the order is jump check -> latch consume /
; motion integration -> collision response. A collision-set +0x12C can be
; observed by the next normal-state pass before its integrator clears it.
; This proves local order, not the outer recurring cadence.
