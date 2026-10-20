# PlayerHose mode-scaled hotspot timer (2026-10-20)

Retail `FDNYFirefighter.exe` SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
This check reads the current pinned executable's x86 bytes at `004677D0`,
cross-checking the existing Ghidra export
[`playerhose_hotspot_state_machine_fresh_2026-10-20.c`](playerhose_hotspot_state_machine_fresh_2026-10-20.c).
No fresh Ghidra function review was added.

At hotspot entry, retail reads the authored float at `RCS_HosePath+0x28` and
loads the session mode index from the object at global `00502AAC + 0x6E4`,
field `+0x2C`. It selects one of three binary64 constants, multiplies the
authored float by that value, then stores the rounded binary32 result to
PlayerHose `+0x1A84`:

```asm
00467BB1  fld   dword ptr [eax+0x28]       ; authored M_SpotTime
00467BC2  mov   ecx, dword ptr [0x502AAC]
00467BC8  mov   edx, dword ptr [ecx+0x6E4]
00467BCE  mov   eax, dword ptr [edx+0x2C] ; active mode index
00467BD1  cmp   eax, 0
00467BDB  mov   dword ptr [esp+0x10], 0xCCCCCCCD
00467BE3  mov   dword ptr [esp+0x14], 0x3FF4CCCC ; double 1.3
00467BEB  mov   dword ptr [esp+0x18], 0xCCCCCCCD
00467BF3  mov   dword ptr [esp+0x1C], 0x3FFCCCCC ; double 1.8
00467BFD  ; select [esp+0x18] for index 0
00467C03  cmp   eax, 1
00467C06  ; select [esp+0x10] for index 1, [esp+0x34] (1.0) otherwise
00467C10  fmul  qword ptr [eax]             ; selected scale
00467C16  fstp  dword ptr [esi+0x1A84]     ; rounded timer budget
```

The actual selection branches choose `[esp+0x18]` for index 0, `[esp+0x10]`
for index 1, and `[esp+0x34]` otherwise. The `[esp+0x34]` double is initialized
to 1.0 immediately before the shown constant writes.

| Session mode index | Binary64 timer scale | Host interpretation |
| ---: | ---: | --- |
| `0` | `0x3FFCCCCCCCCCCCCD` (`1.8`) | Authored spot time × 1.8 |
| `1` | `0x3FF4CCCCCCCCCCCD` (`1.3`) | Authored spot time × 1.3 |
| other | `0x3FF0000000000000` (`1.0`) | Authored spot time unchanged |

The selected float is copied to the countdown at PlayerHose `+0x1A7C` when the
hotspot state starts. The timer update decrements that countdown, and timeout
is checked before the next contact update. These instructions establish the
numeric mode-index rule. They do not identify UI labels, prove that the field
is specifically a difficulty setting, or show how the selected index is
initialized.

PlayerRescue initializer `0044D550` also reads this session field and applies a
separate three-way multiplier to its rescue deadline. That supports a
difficulty-related interpretation of the shared index, but the two multiplier
tables differ and neither trace supplies menu labels or the retail initial
selection.

The shared host now applies the same numeric branch before each hotspot. Its
direct scene launch accepts `--hose-mode-index 0|1|2`; index 2 is the host
default, matching the source's `other`/1.0 branch. This default is an explicit
host choice because the retail menu selector and its initial value are not
ported. No Easy/Normal/Hard labels are assigned to these indices.

The rescue-all macOS bundle rebuilt after this change, validated its plist,
and contains eight rescue catalog entries plus all four dedicated hose maps.
The packaged executable SHA-256 is
`61578f31be495ce98c1e951f7afeb09cc7f3c80f097f17d69286a27636332f5e`.
This build confirms compilation and packaging only; no interactive or retail
parity check was made. The existing Homebrew json-c deployment-target warning
remains (library built for macOS 27.0, target linked for 13.3).
