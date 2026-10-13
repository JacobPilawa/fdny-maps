# Hash-pinned Ghidra listing excerpts for FDNYFirefighter.exe
# SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
# Constants independently read from the active program's mapped bytes.

# Eligibility: current X size >= 0.01; now >= fire+0x124; failed chance retries at now+500.
00447ad9: FLD float ptr [EAX]
00447adb: FCOMP double ptr [0x004dfc78]     # 0.01
00447aec: MOV EDX,dword ptr [0x00502afc]    # game clock
00447af2: CMP EDX,dword ptr [EDI + 0x124]
00447b04: CALL 0x00498b30                   # uniform(0,1)
00447b09: FCOMP float ptr [EDI + 0x128]     # intensity gate
00447b19: MOV EAX,[0x00502afc]
00447b1e: ADD EAX,0x1f4                     # rejected roll: +500
00447b23: MOV dword ptr [EDI + 0x124],EAX

# Radius = min(sizeX,sizeZ)*0.5*(1+uniform(0,1)*intensity); angle in [0,2*pi).
00447b7a: FMUL double ptr [0x004de6f8]     # 0.5
00447b90: CALL 0x00498b30                   # uniform(0,1)
00447b95: FMUL float ptr [EDI + 0x128]
00447bad: FMUL float ptr [ESP + 0x2c]
00447bb1: FADD float ptr [ESP + 0x28]
00447bb9: CALL 0x00498b30                   # uniform(0,6.2831855)

# Candidate starts 20 units above origin; swept box bounds are +/-1 and vector is (0,-200,0).
00447c2d: MOV dword ptr [ESP + 0x68],0xc3480000  # -200.0f
00447c3d: MOV dword ptr [ESP + 0x3c],0x3f800000  # +1.0f
00447c55: MOV dword ptr [ESP + 0x2c],0xbf800000  # -1.0f
00447ca4: FADD float ptr [0x004e1604]            # +20.0f to Y
00447cca: CALL 0x00492ce0                        # collision sweep

# Child shape keys: first growth interval [750,3000]; add tail interval [1000,6000].
00447ef3: PUSH 0x453b8000                        # 3000.0f
00447ef8: MOV dword ptr [EDX + 0x8],EAX
00447efb: CALL 0x00498b30                        # lower bound 750.0f
00447f00: CALL 0x004b9400                        # rounded integer timestamp
00447f35: MOV EDI,dword ptr [ESI + 0x28]
00447f38: PUSH 0x45bb8000                        # 6000.0f
00447f3d: PUSH 0x447a0000                        # 1000.0f
00447f42: LEA EDI,[ESP + EDI*0x4 + 0x4c]
00447f46: CALL 0x00498b30                        # uniform(1000,6000)
00447f4b: MOV EDX,dword ptr [EDI + -0x4]
00447f4e: FIADD dword ptr [EDX]                 # add previous key time
00447f53: CALL 0x004b9400

# Retry timing uses global proximity count and the last child key's time.
00447f58: FILD dword ptr [0x00502b90]
00447f62: FDIVR float ptr [0x004e1684]           # numerator 12.0f
00447f68: FCOM float ptr [0x004dfc24]            # compare against 1.0f
00447f7d: MOV EDX,dword ptr [ESI + 0x28]
00447f84: FILD dword ptr [EAX]                   # final child key time
00447f8e: FDIV ST0,ST1
00447f96: JGE 0x00447f9e
00447f9e: FADDP                                   # add current clock
00447fa0: CALL 0x004b9400
00447fab: MOV dword ptr [EDX + 0x124],EAX
