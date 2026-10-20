; Read-only Ghidra 12.0.4 inspection of the pinned retail executable.
; Retail input: inputs/original/binaries/FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Saved project: .local/mission-lifecycle-ghidra/FDNYPort

; Data at 004E0A38 is the NUL-terminated string "JUMP". Ghidra lists a code
; reference from 0045370E. In Player update code, root state +0x18C is compared
; with 4 before the action lookup. ESI is Player movement subobject (root+0x140).
00453708  CMP dword ptr [ESI + 0x4c],0x4
0045370c  JNZ 0x0045377e
0045370e  PUSH 0x004e0a38
00453732  CALL 0x00499cb0
00453739  CALL 0x00438550
0045373e  MOV BL,AL
00453766  TEST BL,BL
00453768  JZ 0x00453a50
0045376e  LEA ECX,[ESI - 0x140]
00453774  CALL 0x0044bce0
00453779  JMP 0x00453a50

; The state-not-4 branch skips the JUMP action lookup and calls 0044E990.
; That separate routine contains the 500 ms gate, +0x12C latch check,
; +0x100 velocity write, Jumps statistic increment, and Jump sound request.
0045377e  LEA ECX,[ESI - 0x140]
00453784  CALL 0x0044e990

; ScriptActor token references resolve the meaning of Player state 4.
; 004E1C00 is "crouch" (XREF 004518FB); its handler checks state != 4 and
; an adjacent Player byte before setting root +0x88 and state +0x18C = 4.
; 004E1BF4 is "uncrouch" (XREF 00451964); its handler calls 0044BCE0.
; Ghidra data bytes: 004E1C00 = 63 72 6F 75 63 68 00; 004E1BF4 =
; 75 6E 63 72 6F 75 63 68 00.
004518fb  PUSH 0x004e1c00
00451927  CMP dword ptr [ECX + 0x4c],0x4
00451933  MOV EDX,dword ptr [0x004e1820]
00451939  MOV dword ptr [ECX - 0xb8],EDX
0045193f  MOV dword ptr [ECX + 0x4c],0x4
00451964  PUSH 0x004e1bf4
00451992  CALL 0x0044bce0

; 0044BCE0 only runs its body when root state +0x18C is 4. It checks a
; clearance/query using Player root +0x4c, +0x78, +0x84, and +0x8c. A true
; result requests the sound instance at root +0x508 and returns with state 4.
; A false result writes the constant at root +0x88 and sets root state to 3.
; Ghidra data values: DAT_004E181C is binary32 85.0 and DAT_004E1820 is
; binary32 25.0. The crouch command stores 25.0 at root +0x88; successful
; uncrouch stores 85.0 there.
; This handler is not the 0044E990 jump-stat/velocity routine.
0044bce6  CMP dword ptr [ESI + 0x18c],0x4
0044bcef  MOV EAX,dword ptr [ESI + 0x84]
0044bcf5  FLD float ptr [ESI + 0x8c]
0044bd23  CALL 0x00492df0
0044bd28  TEST AL,AL
0044bd2a  JZ 0x0044bd40
0044bd2c  PUSH 0x0
0044bd2e  LEA ECX,[ESI + 0x508]
0044bd34  CALL 0x004a11a0
0044bd40  MOV ECX,dword ptr [0x004e181c]
0044bd46  MOV dword ptr [ESI + 0x88],ECX
0044bd4c  MOV dword ptr [ESI + 0x18c],0x3

; 00492DF0 submits root +0x4c, +0x78, and the tuple {root +0x84, 85.0,
; root +0x8c} to geWorld_GetContents with mode 2. It returns true only when
; a contents result exists and its bits intersect 0x43. In 0044BCE0 this is
; the blocked branch: sound plays and crouch state/height remain. A clear
; query returns false and restores the 85.0 standing value and state 3.

; Together, the named commands show that state 4 is the crouch state and that
; the JUMP action in that state routes to the same uncrouch helper as the
; ScriptActor command. The Player origin/bounds values supplied to the query
; are recovered, but their exact coordinate relationship and contact with the
; host collider remain open, as do outer update scheduling and normal-state
; jump/contact behavior. Do not
; infer that the JUMP action directly triggers 0044E990.
