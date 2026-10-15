# FireRescue route visual-update threshold in the no-C_Model initializer path
# Binary: inputs/original/binaries/FDNYFirefighter.exe
# SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
# Captured with GNU objdump -d -Mintel from the hash-pinned local executable.

# `eax` is the RCS_FireRescue UserData pointer. UserData +0x08 is C_Model.
# When it is null, the conditional branch reaches this H_Radius/H_Height path.
00448eca: 0f 84 c4 00 00 00          je 0x00448f94
00448f94: 8b 48 10                   mov ecx,dword ptr [eax + 0x10] # H_Radius
00448f97: d9 40 14                   fld dword ptr [eax + 0x14]    # H_Height
00448f9a: 8b 50 7c                   mov edx,dword ptr [eax + 0x7c]
00448f9d: 89 4c 24 10                mov dword ptr [esp + 0x10],ecx
00448fa1: 8b 88 80 00 00 00          mov ecx,dword ptr [eax + 0x80]
00448fa7: 89 4c 24 24                mov dword ptr [esp + 0x24],ecx
00448fab: 8b 4c 24 10                mov ecx,dword ptr [esp + 0x10]
00448faf: 51                         push ecx
00448fb0: 51                         push ecx
00448fb1: 89 54 24 28                mov dword ptr [esp + 0x28],edx
00448fb5: 8b 90 84 00 00 00          mov edx,dword ptr [eax + 0x84]
00448fbb: d9 1c 24                   fstp dword ptr [esp]
00448fbe: 51                         push ecx
00448fbf: 8d 44 24 44                lea eax,[esp + 0x44]
00448fc3: 8d ae 08 01 00 00          lea ebp,[esi + 0x108]        # FireRescue root +0x108
00448fc9: 50                         push eax                     # output vector
00448fca: 8b cd                      mov ecx,ebp
00448fcc: 89 54 24 38                mov dword ptr [esp + 0x38],edx
00448fd0: e8 ab 42 fe ff             call 0x0042d280
00448fd5: 8b 7e 78                   mov edi,dword ptr [esi + 0x78]

# In the no-model branch, 0042D280 writes its three float arguments to the
# receiver vector, then copies that vector to the output buffer. The call
# arguments above produce root[+0x108..+0x110] =
# (UserData.H_Radius, UserData.H_Height, UserData.H_Radius).

# AdvanceFireRescuePathMotion (`00449340_fresh.c`) reads that vector's first
# component and returns until the route progress has advanced past half of it:
#   fVar1 = *(float *)(root + 0x108);
#   fVar3 = DAT_004de6f8;             # 0.5, also shown in fire-spread/0044A7A0_falloff_assembly_excerpt.asm
#   if (newProgress - priorVisualProgress <= fVar1 * fVar3) return;

# Authored route examples have no C_Model:
#   MBase entity 149 `mortarfire1`: H_Radius=100, so threshold=50.
#   Gas R entity 355 `Fire41`: H_Radius=45, so threshold=22.5.
# The non-null C_Model branch derives the vector by a separate model path and
# is not covered by this field mapping. The deferred child-spawn request and
# its queued object's world initialization/lifetime are separate open work.
