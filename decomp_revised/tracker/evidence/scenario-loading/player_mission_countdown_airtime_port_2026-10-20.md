# Player mission countdown, Air Time, and timeout state — 2026-10-20

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The read-only review used Ghidra 12.0.4 on the saved `FDNYPort` program and
checked the relevant instructions with `objdump -d -Mintel`. The executable
hash was verified immediately before the review. An independent read-only
audit repeated the Player offset and writer analysis.

## Retail trace

Player initializer `0044D550` stores the mode-scaled level time at Player
`+0x343C` (stores at `0044DE1A`, fallback `600.0f` at `0044DE22`, scaled store
at `0044DE90`). The level parser converts the authored `time:` field to
seconds by multiplying it by 60; the Player initializer applies its mode
factor. This field has two confirmed uses: it is read by the escape/RescueBot
deadline helper `004541A0`, and it is decremented as the live Player timer by
the update body beginning at `00452570`.

`00452570` receives the Player movement subobject at root `+0x140`. Its
instructions map as follows:

```asm
00452737  FLD  dword ptr [ESI+0x32FC] ; Player+0x343C
0045273D  FSUB dword ptr [0x00502B04] ; shared frame delta, seconds
00452743  FSTP dword ptr [ESI+0x32FC]
00452790  FLD  dword ptr [0x00502B04]
00452796  FMUL dword ptr [0x004E0ED4] ; 1000.0f
004527A3  CALL 0x004B9400              ; truncate toward zero
004527BD  CALL 0x00486980              ; add Completion Time
00452887  FLD  dword ptr [ESI+0x32FC]
0045288D  FCOMP dword ptr [0x004DC50C] ; binary32 0.0
00452893  FNSTSW AX
00452895  TEST AH,0x05
00452898  JP   0x004528AF              ; nonnegative timer path
0045289A  MOV  dword ptr [ESI+0x32FC],0
004528A4  MOV  dword ptr [ESI+0x32F8],EDI ; Player+0x3438; EDI=2 from entry
004528AA  JMP  0x00453A50              ; shared function epilogue
004528AF  MOV  AL,byte ptr [ESI-0x14]  ; Player+0x12C
004528B2  TEST AL,AL
004528B4  JNZ  0x00452920
004528B6  ...                           ; add Air Time by trunc(delta*1000)
```

The timer is in seconds: each Player update subtracts the shared seconds
delta. Once the value becomes negative, the update clamps it to zero, writes
state `2` to Player `+0x3438`, and exits through a shared epilogue. The value
at `004DC50C` is binary32 zero, verified from the pinned executable bytes.
The status updater `00458400` routes `+0x3438 == 2` to widget selector `0x48`,
`RC_ResLvlLose`, and `RescueLose` (see
[`mission_outcome_status_dispatch_2026-10-19.md`](mission_outcome_status_dispatch_2026-10-19.md)).
This ties countdown expiry to the loss-feedback state. The complete frontend
return/outro and result-widget timing remain unverified.

Air Time adds the same truncated `frameDelta * 1000.0f` value only when the
remaining timer is nonnegative and Player `+0x12C` is zero. The collision
callback `0049E0A0` sets `+0x12C` for a contact normal with Y at least `0.5`;
`0049DBC0` later copies it to `+0x12D` and clears it. This identifies the
tested transient byte, but does not establish an exact host/retail callback
schedule or make “Air Time” a broader physical-state definition than this
source gate.

## Shared port change

The rescue host initializes a live remaining-seconds field from the selected
catalog record and decrements it with each paced gameplay frame. It clamps
negative values to zero, writes `RescueExitState.messageGate = 2` on expiry,
and stops calling the host Player movement step for that result state. Escape
and RescueBot deadline helpers now read the live countdown rather than the
initial catalog value. The in-memory `Air Time` statistic increments when the
remaining field is nonnegative and the host collision latch is clear. This
uses the host's paced frame and previous-step latch; callback order and
cadence remain provisional. The stat is not yet profile-backed, and result
audio, widget timing, frontend return, and full result presentation are not
implemented by this change.

## Verification and limits

The source-level countdown, zero clamp, state-2 write, Air Time gate, and
millisecond statistic contribution are statically supported by the hash-pinned
binary. The independent writer scan found only the initializer's direct
`+0x343C` stores, the per-frame subtraction, and timeout zeroing.

The macOS rescue-all app compiled and packaged. The standalone and bundled
executables match at SHA-256
`c53458c9ce9cca0219475ea270e837ce217d45141e604e5f1e08bc50f6fadd5e`; the
build script validated `Info.plist` and included the eight source-ordered
rescue choices. The existing Homebrew `json-c` warning remains: the library
was built for macOS 27.0 while the app targets macOS 13.3.

No gameplay tests, natural retail expiry, interactive run, result audio,
visual result, or retail-parity check has been performed. The build confirms
compilation and packaging only.
