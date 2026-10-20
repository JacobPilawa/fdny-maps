# FireRescue moveto visual-subobject position trace
#
# Binary: inputs/original/binaries/FDNYFirefighter.exe
# SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
# Captured from the current local hash-pinned binary with GNU objdump.
#
# FUN_0044A6A0 receives ECX, returns that same pointer in EAX, and sets its
# dirty byte +0x50. In the 00449340 route helper, ECX is root+0x12C (visual).
0044A6A0: 8B C1                   mov eax,ecx
0044A6A2: C6 40 50 01             mov byte ptr [eax+0x50],1
0044A6A6: C3                      ret

# 00449340_fresh.c shows the route helper first copies visual +0x28..+0x30 to
# the generated FireRescue request +0x7C..+0x84 (ZZ_location), then later writes
# its route candidate local_50/local_4c/local_48 back to visual +0x28..+0x30.
# The helper body has no store to the FireRescue root transform +0x4C..+0x54.
#
# In 0044A840, ESI is the visual subobject. This loop transforms its six
# bounds/vertex inputs, then adds visual base X/Y/Z to the transformed result
# and stores six world-space records at the fourth sprite child's +0xD8 onward.
0044AC16: 8D 6E 28                lea ebp,[esi+0x28]
0044AC20: 55                      push ebp
0044AC2A: E8 A1 06 FF FF          call 0043B2D0
0044AC39: E8 72 B2 04 00          call 00495EB0
0044AC5E: D8 45 00                fadd dword ptr [ebp]       # transformed X + visual X
0044AC61: 8B 46 6C                mov eax,dword ptr [esi+0x6C] # fourth child
0044AC67: D9 9C 38 D8 00 00 00   fstp dword ptr [eax+edi+0xD8]
0044AC6E: 8B 4E 6C                mov ecx,dword ptr [esi+0x6C]
0044AC71: D9 05 34 2C 50 00       fld dword ptr [0x00502C34]
0044AC77: D8 46 2C                fadd dword ptr [esi+0x2C]   # transformed Y + visual Y
0044AC7A: D9 9C 39 DC 00 00 00   fstp dword ptr [ecx+edi+0xDC]
0044AC81: 8B 56 6C                mov edx,dword ptr [esi+0x6C]
0044AC84: D9 05 38 2C 50 00       fld dword ptr [0x00502C38]
0044AC8A: D8 46 30                fadd dword ptr [esi+0x30]   # transformed Z + visual Z
0044AC8D: D9 9C 3A E0 00 00 00   fstp dword ptr [edx+edi+0xE0]

# 0044B060 calls 0044A840 after its visual bounds setup. Existing same-hash
# visual-terminal-state.md traces the four child records through 0047F960 to
# geWorld_AddPolyOnce. This excerpt directly demonstrates the base vector's
# effect on at least the fourth child's rendered polygon vertices; it does not
# claim that all three flame quad matrix chains are fully traced here.
