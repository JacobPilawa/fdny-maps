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

; Entity init 0044D550 calls 0044CD10, then rebuilds root+0x3460 from a stack
; source at ESP+0x54, then initializes the follower at root+0x34A4 from it.
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

; 0042D0A0 copies source frame fields to the destination, including parent +0x40.
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

; State 5/debug movement composes the stored frame before applying the vector.
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
