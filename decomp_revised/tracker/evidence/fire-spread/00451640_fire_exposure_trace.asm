# Hash-pinned Ghidra listing excerpts for FDNYFirefighter.exe
# SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e

# Player message vtable handler.
00453a76: MOV EAX,dword ptr [ESI + 0x3438]
00453a7c: TEST EAX,EAX
00453a7e: JNZ 0x00453ad7                        # ignore when gate is nonzero
00453a84: SUB EAX,0x7530
00453a8c: JZ 0x00453aa2                        # message 0x7531
00453a8f: JNZ 0x00453ad7
00453ab1: ADD ECX,0x12c                         # FireRescue visual subobject
00453ab7: CALL 0x00447870                       # read fire source position
00453abf: CALL 0x00451640                       # ApplyPlayerFireExposure
00453acb: MOV EAX,dword ptr [ESP + 0x18]
00453ad2: CALL 0x0044bc00                       # message 30000 branch
00453adb: RET 0x8                               # message id + payload

# ApplyPlayerFireExposure: pain deadline, statistic, burn accumulation.
00451742: MOV ECX,dword ptr [0x00502afc]         # unsigned game clock
00451748: CMP ECX,dword ptr [ESI + 0x390]
0045174e: JBE 0x00451820                        # pain/stat gate not due
0045176d: CALL 0x004897c0                       # RC_Pain lookup/play path
00451778: PUSH 0x451c4000                       # 2500.0f
0045177d: PUSH 0x447a0000                       # 1000.0f
00451782: CALL 0x00498b30                       # shared RNG range
00451790: FILD dword ptr [ESP + 0x8]             # current clock
0045179c: FADDP                                   # random interval + now
0045179e: CALL 0x004b9400                       # convert to integer deadline
004517af: MOV dword ptr [ESI + 0x390],EAX        # next pain deadline
004517c6: PUSH 0x1
004517de: CALL 0x00486980                       # increment "Serious Burns"

# The FST stores a binary32 copy but leaves the extended sum on the x87 stack.
# FCOMP therefore applies a strict > 1.0 comparison to the extended sum.
00451820: FLD float ptr [0x004e1818]            # 1.2
00451826: FMUL float ptr [0x00502b04]           # frame delta
0045182c: FMUL float ptr [ESP + 0x88]            # exposure influence
00451833: FADD float ptr [ESI + 0x3538]          # existing burn accumulator
00451839: FST float ptr [ESI + 0x3538]           # store rounded float; no pop
0045183f: FCOMP double ptr [0x004dd808]         # 1.0
0045184a: JNZ 0x00451855                        # <= threshold returns
0045184c: PUSH 0x1
00451850: CALL 0x00450fb0                       # passout
