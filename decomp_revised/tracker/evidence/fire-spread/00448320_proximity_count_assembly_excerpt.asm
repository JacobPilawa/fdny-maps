# Retail input: inputs/original/binaries/FDNYFirefighter.exe
# SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
# Captured with:
#   objdump -d -Mintel --start-address=0x448320 --stop-address=0x448385
#
# The float compare at +0x120 sets x87 status flags. The masked tests below
# separate prior distance <=750 from prior distance >750.

00448320: 8b 41 48                   mov  eax,dword ptr [ecx+0x48]
00448323: 8b 50 04                   mov  edx,dword ptr [eax+0x04]
00448326: 85 d2                      test edx,edx
00448328: 74 4d                      je   0x448377
0044832a: 8b 54 24 04                mov  edx,dword ptr [esp+0x04]
0044832e: 81 fa ee 02 00 00          cmp  edx,750
00448334: 7f 1b                      jg   0x448351
00448336: d9 81 20 01 00 00          fld  dword ptr [ecx+0x120]
0044833c: d8 1d b0 16 4e 00          fcomp dword ptr [0x4e16b0]
00448342: df e0                      fnstsw ax
00448344: f6 c4 41                   test ah,0x41
00448347: 75 08                      jne  0x448351
00448349: a1 90 2b 50 00             mov  eax,dword ptr [0x502b90]
0044834e: 40                         inc  eax
0044834f: eb 21                      jmp  0x448372
00448351: d9 81 20 01 00 00          fld  dword ptr [ecx+0x120]
00448357: d8 1d b0 16 4e 00          fcomp dword ptr [0x4e16b0]
0044835d: df e0                      fnstsw ax
0044835f: f6 c4 41                   test ah,0x41
00448362: 7a 13                      jp   0x448377
00448364: 81 fa ee 02 00 00          cmp  edx,750
0044836a: 7e 0b                      jle  0x448377
0044836c: a1 90 2b 50 00             mov  eax,dword ptr [0x502b90]
00448371: 48                         dec  eax
00448372: a3 90 2b 50 00             mov  dword ptr [0x502b90],eax
00448377: db 44 24 04                fild dword ptr [esp+0x04]
0044837b: d9 91 20 01 00 00          fst  dword ptr [ecx+0x120]
00448381: d8 1d b0 16 4e 00          fcomp dword ptr [0x4e16b0]
