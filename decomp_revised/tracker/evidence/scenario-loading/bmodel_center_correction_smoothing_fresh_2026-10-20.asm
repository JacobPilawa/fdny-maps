; Hash-pinned raw-instruction evidence
; Program: FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Read-only Ghidra 12.0.4, -process FDNYFirefighter.exe -readOnly -noanalysis
; 0043BD90 has a function boundary. 00482FC0's old-weight math is confirmed
; by this instruction window plus the saved pseudocode in
; bmodel_liftable_pose_fresh_2026-10-20.c.

; 0043BD90 reads the first child's current center, compares it to BModel +0x224,
; and only blends when the mode-2 vector comparison finds a difference.
0043BD90  SUB ESP,0x18
0043BD93  PUSH ESI
0043BD94  PUSH EDI
0043BD95  LEA EAX,[ESP + 0x8]
0043BD99  PUSH EAX
0043BD9A  MOV EDI,ECX
0043BD9C  CALL 0x0043bc80                 ; current first-child center -> local
0043BDA1  LEA ECX,[ESP + 0x8]
0043BDA5  PUSH ECX                          ; current center
0043BDA6  LEA ESI,[EDI + 0x224]            ; desired held target
0043BDAC  PUSH ESI
0043BDAD  CALL 0x0043b340                 ; component comparison, mode 2
0043BDB2  ADD ESP,0x8
0043BDB5  TEST AL,AL
0043BDB7  JZ 0x0043bddb
0043BDB9  PUSH 0x0                          ; later 0043BAA0 flag: don't overwrite target
0043BDBB  PUSH 0x3dcccccd                 ; 0.1f smoothing duration
0043BDC0  PUSH ESI                          ; target vector
0043BDC1  LEA EDX,[ESP + 0x14]
0043BDC5  PUSH EDX                          ; current center
0043BDC6  LEA EAX,[ESP + 0x24]
0043BDCA  PUSH EAX                          ; blended output vector
0043BDCB  CALL 0x00482fc0
0043BDD0  ADD ESP,0x10
0043BDD3  PUSH EAX
0043BDD4  MOV ECX,EDI
0043BDD6  CALL 0x0043baa0                 ; apply blended point to children
0043BDDB  ; copy target +0x224 into cached current-center field +0x230

; 00482FC0 computes an exponential old-center weight from shared frame delta.
00482FC0  SUB ESP,0x18
00482FC3  FLD double ptr [0x004de6f8]      ; 0.5
00482FC9  PUSH ESI
00482FCA  FLD float ptr [0x00502b04]      ; shared frame delta
00482FD0  FDIV float ptr [ESP + 0x2c]     ; caller's 0.1f duration
00482FD4  CALL 0x004b9a20                 ; CRT pow adapter
00482FD9  FSTP float ptr [ESP + 0x2c]     ; round weight to binary32
00482FDD  FLD float ptr [0x004dfc24]      ; 1.0f
00482FE7  FSUB float ptr [ESP + 0x2c]     ; target weight = 1 - old weight
00482FF5  CALL 0x00446390                 ; target * (1 - old weight)
0048300D  CALL 0x00446390                 ; current * old weight
0048301B  CALL 0x0042d050                 ; add weighted vectors

; Formula established by this caller and helper:
;   oldWeight = binary32(pow(0.5, sharedFrameDelta / 0.1f))
;   blendedCenter = oldCenter * oldWeight + target * (1 - oldWeight)
; This establishes a 0.1-second half-life in the source's frame-delta formula.
; It does not establish how often this Player update is called in retail.
