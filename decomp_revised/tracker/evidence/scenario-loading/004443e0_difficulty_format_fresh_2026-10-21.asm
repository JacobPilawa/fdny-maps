; FDNYFirefighter.exe, SHA-256:
; 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Read-only Ghidra 12.0.4 decompilation from the disposable headless
; FDNYPort project. Primary saved project was not modified.
;
; 00471030 obtains the integer passed to 004443E0 from the selected object
; at EverGame +0x6E4, field +0x2C. It stores the formatted result under
; script variable DIFFICULTY. The selected-object writer for +0x6E4 is
; unresolved; these instructions establish the data source, not its value.

00471048  MOV EAX,[0x00502AAC]
0047104D  MOV EAX,[EAX + 0x6E4]
00471058  MOV ECX,[EAX + 0x2C]
0047105B  ADD EAX,0x2C
0047105E  PUSH ECX
0047105F  LEA ECX,[ESP + 0x14]
00471063  CALL 0x004443E0

; 004443E0 builds the two-character format string "%i":

00444438  PUSH 0x25                 ; '%'
00444442  CALL 0x0040D6D0           ; initialize string from '%'
00444455  MOV byte ptr [ESP + 0xC],0x69 ; 'i'
0044445E  CALL 0x00401D00           ; append "i"
00444463  MOV ECX,[ESP + 0x24]      ; integer passed by 00471030
00444467  MOV EDX,[ESP + 0xC]       ; format string object
0044446C  ADD EDX,0xC               ; string bytes "%i"
0044446F  PUSH ECX
00444470  PUSH EDX
00444471  PUSH ESI
00444476  CALL 0x00402770           ; format into output string

; 00471030 then stores the result using key "DIFFICULTY". It separately
; stores the active level record's first string member using key "LEVEL".
