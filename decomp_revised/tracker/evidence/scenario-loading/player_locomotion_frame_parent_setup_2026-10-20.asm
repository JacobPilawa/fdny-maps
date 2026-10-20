; Read-only x86 check of the Player locomotion-frame initialization and use.
; Retail input: inputs/original/binaries/FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Disassembly: /usr/bin/objdump -D -Mintel, current pinned binary.

; Constructor 00453D50 initially calls 0042D2D0(root+0x3460, 0).
00453f89  PUSH EBX
00453f8a  LEA ECX,[ESI + 0x3460]
00453f90  MOV BYTE PTR [ESP + 0x20],0x15
00453f95  MOV DWORD PTR [ESI + 0x3430],EBX
00453f9b  MOV DWORD PTR [ESI + 0x3434],EBX
00453fa1  MOV DWORD PTR [ESI + 0x3438],EBX
00453fa7  CALL 0x0042D2D0

; Entity init 0044D550 calls 0044CD10, then resets the receiver basis/scale at
; root+0x3460 and writes a frame copy to the stack output at ESP+0x54.
0044df1e  CALL 0x0044CD10
0044df23  LEA EDX,[ESP + 0x54]
0044df27  LEA EDI,[ESI + 0x3460]
0044df2d  PUSH EDX
0044df2e  MOV ECX,EDI
0044df30  CALL 0x0042D350
0044df35  PUSH 0x1
0044df37  PUSH EDI
0044df38  LEA ECX,[ESI + 0x34A4]
0044df3e  CALL 0x00483A20
; The 00483A20 source argument is EDI (root+0x3460); the local output copy
; returned by 0042D350 is not forwarded to the follower initializer.

; 0042D350 call semantics. ECX is the receiver, and the stack argument is the
; output frame. It sets the receiver's axes and scale, leaving position +0x28
; and parent +0x40 intact, then copies receiver to the output via 0042D0A0.
0042d350  SUB ESP,0xC
0042d353  PUSH ESI
0042d354  MOV ESI,ECX                    ; receiver
0042d356  MOV ECX,0x3F800000
0042d35b  XOR EAX,EAX
0042d35d  PUSH ECX
0042d35e  PUSH ECX
0042d35f  MOV DWORD PTR [ESI + 0x08],EAX ; reset receiver basis
0042d362  MOV DWORD PTR [ESI + 0x0C],EAX
0042d365  MOV DWORD PTR [ESI + 0x10],EAX
0042d368  MOV DWORD PTR [ESI + 0x18],EAX
0042d36b  MOV DWORD PTR [ESI + 0x1C],EAX
0042d36e  MOV DWORD PTR [ESI + 0x20],EAX
0042d371  PUSH ECX
0042d372  LEA EAX,[ESP + 0x10]
0042d376  MOV DWORD PTR [ESI + 0x04],ECX
0042d379  MOV DWORD PTR [ESI + 0x14],ECX
0042d37c  MOV DWORD PTR [ESI + 0x24],ECX
0042d37f  PUSH EAX                      ; output-copy destination
0042d380  LEA ECX,[ESI + 0x34]          ; unit scale
0042d383  CALL 0x0042D280
0042d388  PUSH ESI                      ; source receiver
0042d389  MOV ESI,DWORD PTR [ESP + 0x18] ; output-copy destination
0042d38d  MOV ECX,ESI
0042d38f  CALL 0x0042D0A0

; 0042D0A0's ECX is the copy destination; its first stack argument is source.
; It copies all frame fields +0x04..+0x40 from source into destination.
0042d0f0  LEA EDX,[ECX + 0x28]
0042d0f3  MOV EDI,DWORD PTR [EDX]
0042d0f5  LEA ESI,[EAX + 0x28]
0042d0f8  MOV DWORD PTR [ESI],EDI
0042d0fa  MOV EDI,DWORD PTR [EDX + 0x4]
0042d0fd  MOV DWORD PTR [ESI + 0x4],EDI
0042d100  MOV EDX,DWORD PTR [EDX + 0x8]
0042d103  MOV DWORD PTR [ESI + 0x8],EDX
0042d106  LEA EDX,[ECX + 0x34]
0042d109  MOV EDI,DWORD PTR [EDX]
0042d10b  LEA ESI,[EAX + 0x34]
0042d10e  MOV DWORD PTR [ESI],EDI
0042d110  MOV EDI,DWORD PTR [EDX + 0x4]
0042d113  MOV DWORD PTR [ESI + 0x4],EDI
0042d116  MOV EDX,DWORD PTR [EDX + 0x8]
0042d119  MOV DWORD PTR [ESI + 0x8],EDX
0042d11c  MOV ECX,DWORD PTR [ECX + 0x40]
0042d11f  POP EDI
0042d120  MOV DWORD PTR [EAX + 0x40],ECX

; Movement setup resets axes/scale on root+0x24, copies it to ESP+0x9C, then
; rotates the receiver by the view yaw. The stack argument is an output copy.
00452d1f  LEA ECX,[ESP + 0x9C]
00452d26  LEA EDI,[ESI - 0x11C]          ; Player root +0x24 receiver
00452d2c  PUSH ECX
00452d2d  MOV ECX,EDI
00452d35  CALL 0x0042D350
00452d3a  MOV EDX,DWORD PTR [ESI + 0x32F4]
00452d40  PUSH EDX
00452d41  MOV ECX,EDI
00452d43  CALL 0x004968B0

; State 5/debug movement overwrites the temporary with a parent-aware copy of
; the stored root+0x3460 frame before applying the same vector transform.
00453205  LEA EDX,[ESI + 0x3320]       ; Player root +0x3460
0045320b  PUSH EDX
0045320c  LEA EAX,[ESP + 0xA4]         ; temporary yaw frame
00453213  PUSH EAX
00453214  CALL 0x004977B0
00453219  ADD ESP,0x8
0045321c  PUSH EAX
0045321d  LEA ECX,[ESP + 0x30]
00453221  PUSH ECX
00453222  CALL 0x00495EB0

; The normal-state path reaches 0044F340 later at 00453860. This establishes
; intra-update order only, not the recurring Player callback cadence.
