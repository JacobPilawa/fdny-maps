; Read-only Ghidra 12.0.4 headless capture from saved FDNYPort project.
; Retail input: inputs/original/binaries/FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Query: FindFunctionCallers.java 00470740; InspectInstructionWindow.java 0044FE20

; Player root update wrapper adjusts to the movement subobject before the
; shared status-gated updater. The subobject's slot-zero callback is the
; Player movement update 00452570.
0044fe59  LEA ECX,[ESI + 0x140]
0044fe5f  CALL 0x00470740

; Direct code references returned for shared updater 00470740:
; 00443603 CALL; 00446fe2 CALL; 0044fe5f CALL;
; 0043afe3 JMP; 0043fa76 JMP; 00442bb3 JMP; 0045999d JMP.
; These identify local wrappers/thunks only. This query does not establish
; the recurring manager that invokes Player root vtable slot +0x14 or the
; Player/BModel ordering in a retail frame. The attempted function query for
; wrapper 0044fe20 reports no saved Ghidra function boundary; its instruction
; window is available in /tmp/fdny_player_update_wrapper_2026-10-20.log.

; Player root vtable +0x14 -> 0044fe20 -> 00470740(Player+0x140) ->
; movement-subobject slot zero -> 00452570 is the local route recorded in
; dirty_space_axe_warp_trace_2026-10-14.md. Recurring outer cadence remains
; unresolved; do not convert this route into a host frame-rate claim.
