; Read-only current-hash x86 disassembly of Player movement block 00452570.
; Retail input: inputs/original/binaries/FDNYFirefighter.exe
; SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Captured with /usr/bin/objdump -D -Mintel at image addresses
; 00453270..00453330 and 004536e0..00453820.
; ESI = Player+0x140; state is [ESI+0x4c] (root +0x18c).

; The movement half-life and velocity update have already run before the
; state-4 branch. This excerpt records the 0.95/0.2 selection, call to
; 00482AD0, blend through 00482FC0, and stored velocity at ESI-0x44.
0045327c  84 c0                      test   al,al
0045327e  75 17                      jne    0x00453297
00453280  39 6e 4c                   cmp    dword ptr [esi+0x4c],ebp
00453283  74 12                      je     0x00453297
00453285  8a 86 40 02 00 00          mov    al,byte ptr [esi+0x240]
0045328b  84 c0                      test   al,al
0045328d  c7 44 24 24 33 33 73 3f    mov    dword ptr [esp+0x24],0x3f733333
00453295  74 08                      je     0x0045329f
00453297  c7 44 24 24 cd cc 4c 3e    mov    dword ptr [esp+0x24],0x3e4ccccd
0045329f  8b 4c 24 24                mov    ecx,dword ptr [esp+0x24]
004532a3  8b 46 c0                   mov    eax,dword ptr [esi-0x40]
004532a6  68 00 00 d8 41             push   0x41d80000
004532ab  51                         push   ecx
004532ac  89 44 24 3c                mov    dword ptr [esp+0x3c],eax
004532b0  8d 7e bc                   lea    edi,[esi-0x44]
004532b3  e8 18 f8 02 00             call   0x00482ad0
004532b8  d9 5c 24 04                fstp   dword ptr [esp+0x4]
004532bc  8b 54 24 18                mov    edx,dword ptr [esp+0x18]
004532c0  83 c4 04                   add    esp,0x4
004532c3  52                         push   edx
004532c4  8d 44 24 20                lea    eax,[esp+0x20]
004532c8  50                         push   eax
004532c9  8d 4c 24 34                lea    ecx,[esp+0x34]
004532cd  51                         push   ecx
004532ce  e8 bd 30 ff ff             call   0x00446390
004532d3  83 c4 0c                   add    esp,0xc
004532d6  50                         push   eax
004532d7  8d 54 24 44                lea    edx,[esp+0x44]
004532db  57                         push   edi
004532dc  52                         push   edx
004532dd  e8 de fc 02 00             call   0x00482fc0
004532e2  8b 08                      mov    ecx,dword ptr [eax]
004532e4  89 0f                      mov    dword ptr [edi],ecx
004532e6  8b 50 04                   mov    edx,dword ptr [eax+0x4]
004532e9  89 57 04                   mov    dword ptr [edi+0x4],edx
004532ec  8b 40 08                   mov    eax,dword ptr [eax+0x8]
004532ef  89 47 08                   mov    dword ptr [edi+0x8],eax

; State 4 (crouch) branches around the non-crouch 0044E990 path into its
; dedicated JUMP query. False and true action results both reach the epilogue;
; the true result first calls the uncrouch clearance helper 0044BCE0.
00453708  83 7e 4c 04                cmp    dword ptr [esi+0x4c],0x4
0045370c  75 70                      jne    0x0045377e
0045370e  68 38 0a 4e 00             push   0x004e0a38
00453713  8d 4c 24 14                lea    ecx,[esp+0x14]
00453717  e8 74 f0 fa ff             call   0x00402790
0045371c  8d 4c 24 10                lea    ecx,[esp+0x10]
00453720  51                         push   ecx
00453721  8b 0d 78 3a 50 00          mov    ecx,dword ptr [0x00503a78]
00453727  c7 84 24 ec 00 00 00 0e 00 00 00 mov dword ptr [esp+0xec],0xe
00453732  e8 79 65 04 00             call   0x00499cb0
00453737  8b c8                      mov    ecx,eax
00453739  e8 12 4e fe ff             call   0x00438550
0045373e  8a d8                      mov    bl,al
00453740  8b 44 24 10                mov    eax,dword ptr [esp+0x10]
00453744  3d 08 19 50 00             cmp    eax,0x00501908
0045374b  89 bc 24 e8 00 00 00       mov    dword ptr [esp+0xe8],edi
00453752  74 14                      je     0x00453768
00453754  ff 08                      dec    dword ptr [eax]
00453756  8b 54 24 10                mov    edx,dword ptr [esp+0x10]
0045375a  83 3a 00                   cmp    dword ptr [edx],0x0
0045375d  7f 09                      jg     0x00453768
0045375f  8d 4c 24 10                lea    ecx,[esp+0x10]
00453763  e8 58 da fa ff             call   0x004011c0
00453768  84 db                      test   bl,bl
0045376a  0f 84 e2 02 00 00          je     0x00453a50
00453770  8d 8e c0 fe ff ff          lea    ecx,[esi-0x140]
00453776  e8 67 85 ff ff             call   0x0044bce0
0045377b  e9 d2 02 00 00             jmp    0x00453a50

; Non-crouch state falls through 0044E990, then prepares the normal movement
; integration at 00453789. Root vtable +0x64 invokes the velocity integrator;
; collision loop 0043F6D0 follows when +0x380 is clear.
0045377e  8d 8e c0 fe ff ff          lea    ecx,[esi-0x140]
00453784  e8 07 b2 ff ff             call   0x0044e990
00453789  8b 0d 04 2b 50 00          mov    ecx,dword ptr [0x00502b04]
0045378f  8d 86 78 04 00 00          lea    eax,[esi+0x478]
004537ca  8b 07                      mov    eax,dword ptr [edi]
004537cc  ff 50 64                   call   dword ptr [eax+0x64]
004537cf  8a 86 40 02 00 00          mov    al,byte ptr [esi+0x240]
004537d5  84 c0                      test   al,al
004537d7  75 07                      jne    0x004537e0
004537d9  8b cf                      mov    ecx,edi
004537db  e8 f0 be fe ff             call   0x0043f6d0

; The movement-vector cap and smoothing instructions precede 00453708 in the
; full block. State 4 therefore can update its capped velocity, then exits
; before the normal integrator/collision calls shown above.
