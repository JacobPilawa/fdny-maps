Retail input: inputs/original/binaries/FDNYFirefighter.exe
SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
Ghidra 12.0.4 saved FDNYPort project; read-only headless exports.

Player movement Total Steps gate. ESI is Player+0x140; the previously
reviewed state branch identifies the excluded state as state 5. EBP is the
state-comparison operand in this slice.

004532f2  MOV EAX,dword ptr [ESI + 0x4c]  ; root +0x18c
004532f8  CMP EAX,EBP
004532fa  JZ 0x0045331f                 ; state 5 skips this producer
004532fc  MOV AL,byte ptr [ESI + 0x240]  ; root +0x380
00453302  TEST AL,AL
00453304  JNZ 0x0045331f
00453306  MOV AL,byte ptr [ESI + 0x58]   ; root +0x198
00453309  TEST AL,AL
00453312  JZ 0x0045331f
0045331a  CALL 0x0044ea90

The same movement pass calls 0044F340 later, at 00453860. Its boolean input
is set from the computed movement magnitude at 00453816..0045382d. The
Player+0x198 byte therefore gates this helper using the preceding update's
head-motion sample state.

0044EA90 Total Steps/footstep helper:

0044eaab  MOV EAX,dword ptr [ESI + 0x384]
0044eab1  MOV ECX,dword ptr [0x00502afc]  ; retail clock
0044eab7  ADD EAX,0x64                    ; 100 ms
0044eaba  CMP ECX,EAX
0044eabc  JC 0x0044ebc6                  ; too soon
0044ead0  MOV ECX,dword ptr [ESI + 0x544] ; Player sound count
0044eadf  FSUB double ptr [0x004dfc78]   ; count - 0.001
0044eaea  CALL 0x00498b30               ; shared pseudo-random range
0044eaf2  CALL 0x004b9400               ; truncate toward zero
0044eaf9  CMP EDI,dword ptr [ESI + 0x558]
0044eaff  JZ 0x0044ead0                  ; retry matching index
0044eb01  MOV EDX,dword ptr [0x00502afc]
0044eb07  MOV dword ptr [ESI + 0x384],EDX
0044eb75  CALL 0x00486980               ; add one to Total Steps
0044ebb4  CALL 0x0048d490               ; get Player sound-list entry
0044ebc0  CALL 0x004a11a0               ; request sound playback

Head-motion writer 00491140, called by 0044F340 with this = Player+0x198:

00491140 shifts the three Y output samples at subobject +0x58/+0x5c/+0x60.
It clears byte this+0, then sets it when the middle sample is strictly less
than both its older and newer neighbors. This is a confirmed sample pattern;
calling it a walking-step/trough signal is an interpretation of its use by the
Total Steps gate.

    *(uint32_t *)(this + 0x60) = *(uint32_t *)(this + 0x5c);
    *(uint32_t *)(this + 0x5c) = *(uint32_t *)(this + 0x58);
    *(uint8_t *)this = 0;
    *(float *)(this + 0x58) = outputY;
    if (*(float *)(this + 0x5c) < outputY &&
        *(float *)(this + 0x5c) < *(float *)(this + 0x60))
        *(uint8_t *)this = 1;

The constructor initializes the Y sample history and mode/phase fields but
the inspected chain has no explicit store to byte this+0. The first +0x198
gate value therefore remains unknown. Player+0x558 starts at zero; no later
static direct writer was found, but computed or external writers are not
excluded. The host uses a deterministic false initial gate and rejection
index zero pending those uncertainties.
