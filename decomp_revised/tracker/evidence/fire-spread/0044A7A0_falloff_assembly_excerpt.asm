# Current hash-pinned Ghidra listing for FDNYFirefighter.exe
# SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e

# r = (volume.sizeA + volume.sizeB) * 0.5; reject when r < 0.01.
0044a7a6: FLD float ptr [ESI + 0x4c]
0044a7a9: FADD float ptr [ESI + 0x44]
0044a7ac: FMUL double ptr [0x004de6f8]      # 0.5
0044a7b2: FCOM double ptr [0x004dfc78]      # 0.01
0044a7bd: JNP 0x0044a810

# Add candidate padding and form squared distance dx^2 + (2*dy)^2 + dz^2.
0044a7c3: FADD float ptr [ESP + 0x18]
0044a7d5: CALL 0x0043b2d0                    # point - volume origin
0044a7da: FLD float ptr [ESP + 0x14]
0044a7de: FADD ST0,ST0                        # double vertical delta
0044a7e3: FLD float ptr [ESP + 0xc]          # dx
0044a7e7: FMUL float ptr [ESP + 0xc]
0044a7eb: FLD float ptr [ESP + 0x4]          # dz
0044a7ef: FMUL float ptr [ESP + 0x4]
0044a7f3: FADDP
0044a7f5: FLD ST1
0044a7f7: FMUL ST2
0044a7f9: FADDP
0044a7fd: FLD float ptr [ESP + 0x18]         # padded radius
0044a801: FMUL float ptr [ESP + 0x18]
0044a807: FCOMPP
0044a80e: JNZ 0x0044a81f                     # inside: compute falloff

# Return zero on radius/containment failure. Success result is returned in x87 ST0.
0044a813: FLD float ptr [0x004dc50c]         # zero
0044a81c: RET 0x8
0044a81f: FSQRT
0044a825: FSUBR float ptr [ESP + 0x14]        # r - sqrt(distance^2)
0044a829: FLD float ptr [EAX + 0xd0]          # emitter scale
0044a82f: FDIV float ptr [ESP + 0x14]         # scale / r
0044a833: FMULP
0044a838: RET 0x8                             # x87 result, no hidden output pointer
