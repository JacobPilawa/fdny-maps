# FireRescue moveto trailing-argument timing trace
#
# Binary: FDNYFirefighter.exe
# SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
# Evidence: Ghidra disassemble_function at the exact addresses below; verified
# against objdump -d -Mintel of the same hash-pinned PE. Ghidra's current C-like
# decompilation omits these implicit x87 stack operations, so use this listing.
#
# SetFireRescuePathPointMove: this = ESI. The trailing command values arrive as
# float bit patterns and are stored at entity +0xB4 (param_4) and +0xB0
# (param_3):
00448CF2: MOV dword ptr [ESI + 0xb4],ECX
00448D06: MOV dword ptr [ESI + 0xb0],EAX
#
# AdvanceFireRescuePathMotion: the function entry copies ECX (the root entity)
# to EDI. It loads each saved float, multiplies by 1000.0f at 0x004E0ED4,
# adds it to an existing integer key timestamp in the x87 stack, converts the
# result through FUN_004B9400, then stores the integer timestamp into the next
# generated key record. The helper's old same-hash export identifies
# FUN_004B9400 as float-to-integer rounding; AddFireRescueMotionKey at
# 0x0044B620 accepts an integer timestamp and compares it with DAT_00502AFC.
00449362: MOV EDI,ECX
004497D9: MOV ECX,dword ptr [ESP + EAX*0x4 + 0x50]
004497DD: FILD dword ptr [ECX]
004497E3: FLD float ptr [EDI + 0xb0]
004497ED: FMUL float ptr [0x004e0ed4]  # 1000.0f
004497F3: FADDP
004497F5: CALL 0x004b9400
00449800: MOV dword ptr [ECX],EAX
00449828: MOV ECX,dword ptr [ESP + EAX*0x4 + 0x50]
0044982C: FILD dword ptr [ECX]
00449832: FLD float ptr [EDI + 0xb4]
0044983C: FMUL float ptr [0x004e0ed4]  # 1000.0f
00449842: FADDP
00449844: CALL 0x004b9400
0044984F: MOV dword ptr [ECX],EAX
#
# Proven: the stored values contribute separate, scaled offsets to generated
# key timestamps. Inference: multiplying by 1000.0f is consistent with authored
# seconds converted to the game's millisecond-like integer clock. The precise
# author-facing terms (delay, duration, or phase) and their runtime effect remain
# unconfirmed.
