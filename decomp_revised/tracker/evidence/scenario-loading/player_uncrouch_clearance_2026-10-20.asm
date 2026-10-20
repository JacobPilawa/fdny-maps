; Read-only current-hash x86 disassembly of Player uncrouch clearance helper.
; Retail input: inputs/original/binaries/FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Captured with /usr/bin/objdump -D -Mintel at image addresses
; helper 0044BCE0..0044BD5C; wrapper is 00492DF0.
;
; ESI is Player root. The query uses root+0x4C as origin, root+0x78 as mins,
; and a stack max vector (root+0x84, DAT_004E181C, root+0x8C).

0044bce0  83 ec 0c                      sub    esp,0xc
0044bce3  56                            push   esi
0044bce4  8b f1                         mov    esi,ecx
0044bce6  83 be 8c 01 00 00 04          cmp    dword ptr [esi+0x18c],0x4
0044bced  75 67                         jne    0x0044bd56
0044bcef  8b 86 84 00 00 00             mov    eax,dword ptr [esi+0x84]
0044bcf5  d9 86 8c 00 00 00             fld    dword ptr [esi+0x8c]
0044bcfb  8b 0d 1c 18 4e 00             mov    ecx,dword ptr [0x004e181c]
0044bd01  8b 15 78 3a 50 00             mov    edx,dword ptr [0x00503a78]
0044bd07  89 44 24 04                   mov    dword ptr [esp+0x4],eax
0044bd0b  89 4c 24 08                   mov    dword ptr [esp+0x8],ecx
0044bd0f  d9 5c 24 0c                   fstp   dword ptr [esp+0xc]
0044bd13  8b 4a 04                      mov    ecx,dword ptr [edx+0x4]
0044bd16  8d 44 24 04                   lea    eax,[esp+0x4]
0044bd1a  50                            push   eax
0044bd1b  8d 56 78                      lea    edx,[esi+0x78]
0044bd1e  52                            push   edx
0044bd1f  8d 46 4c                      lea    eax,[esi+0x4c]
0044bd22  50                            push   eax
0044bd23  e8 c8 70 04 00                call   0x00492df0
0044bd28  84 c0                         test   al,al
0044bd2a  74 14                         je     0x0044bd40
0044bd2c  6a 00                         push   0
0044bd2e  8d 8e 08 05 00 00             lea    ecx,[esi+0x508]
0044bd34  e8 67 54 05 00                call   0x004a11a0
0044bd39  b0 01                         mov    al,0x1
0044bd3b  5e                            pop    esi
0044bd3c  83 c4 0c                      add    esp,0xc
0044bd3f  c3                            ret
0044bd40  8b 0d 1c 18 4e 00             mov    ecx,dword ptr [0x004e181c]
0044bd46  89 8e 88 00 00 00             mov    dword ptr [esi+0x88],ecx
0044bd4c  c7 86 8c 01 00 00 03 00 00 00 mov    dword ptr [esi+0x18c],0x3
0044bd56  b0 01                         mov    al,0x1
0044bd58  5e                            pop    esi
0044bd59  83 c4 0c                      add    esp,0xc
0044bd5c  c3                            ret
