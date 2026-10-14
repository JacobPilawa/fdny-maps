; FDNYFirefighter.exe SHA-256:
; 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
; Verified from the copied retail executable with objdump -D -Mintel.
; The bounds and reciprocal are read from the executable's .rdata.

00462782: 8b 8d 50 02 00 00    mov    ecx, dword ptr [ebp+0x250] ; scan time
00462788: 2b ca                sub    ecx, edx                 ; current time
0046278a: 81 c1 34 21 00 00    add    ecx, 8500                ; 0x2134
0046279c: db 44 24 18          fild   dword ptr [esp+0x18]
004627ac: 7d 06                jge    0x004627b4
004627ae: d8 05 30 fa 4d 00    fadd   dword ptr [0x004dfa30]   ; 2^32
004627b4: dc 0d 30 29 4e 00    fmul   qword ptr [0x004e2930]   ; 1/8500
004627ba: d8 15 0c c5 4d 00    fcom   dword ptr [0x004dc50c]   ; 0.0f
004627c5: 0f 8b 7e 01 00 00    jnp    0x00462949
004627cb: d8 1d 24 fc 4d 00    fcomp  dword ptr [0x004dfc24]   ; 1.0f

; The accepted normalized interval is [0,1], equivalent to scan age [0,8500]
; milliseconds for the host's wrapping 32-bit tick counter.
