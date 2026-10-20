; Read-only Ghidra 12.0.4 headless instruction window.
; Retail input: inputs/original/binaries/FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Saved FDNYPort project, -readOnly -noanalysis, JDK 21.
; Capture: /tmp/fdny_view_follower_refresh_2026-10-20.log

; Player movement update calls its view builder immediately before held-object work:
00453860  CALL 0x0044f340
00453865  LEA ECX,[ESI + 0x3114]
0045386b  CALL 0x00459630
00453870  LEA ECX,[ESI + 0x32cc]
00453876  CALL 0x00455580
0045387b  CMP dword ptr [ESI + 0x3300],EBP
00453881  JZ  0x00453a38

; The held-object branch's angle-vector helper reads movement +0x3364.
00453943  LEA EAX,[ESP + 0x28]
00453947  PUSH EAX
00453948  LEA ECX,[ESI + 0x3364]
0045394e  CALL 0x0049e770

; In this update, ESI is Player movement root +0x140. Thus ESI+0x3364 is
; Player root +0x34a4, the view follower. The separate held-target writer
; retrieves Player root +0x24 through the Player vtable slot +0x18.
; Current-hash decompilation of 0044f340 calls 00483720 on Player+0x34a4,
; refreshing that follower before returning to 00453860's held-object branch
; (equipment_view_frame_trace_2026-10-17.c, lines near FUN_00483720 call).
; This establishes same-update ordering only. It does not establish equality
; with Genesis's rendered camera world transform or the outer update cadence.
