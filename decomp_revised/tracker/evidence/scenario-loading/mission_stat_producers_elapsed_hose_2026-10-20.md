# Mission stat elapsed-time and HoseSpray producers (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The read-only follow-up used Ghidra 12.0.4 and the saved
`.local/mission-lifecycle-ghidra/FDNYPort` project with
`-process FDNYFirefighter.exe -readOnly -noanalysis`. Raw instruction windows
were inspected for the registered stat strings and arithmetic constants.

## Retail producer evidence

`Completion Time` is referenced from code without a function object in the
saved project. At `0045277A` the code constructs the key, loads the shared
frame delta from `00502B04`, multiplies by the binary32 constant at
`004E0ED4` (`1000.0`), converts through `004B9400`, then adds the integer with
`00486980`:

```asm
0045277A  PUSH 0x4DF500             ; "Completion Time"
00452790  FLD float ptr [0x00502B04]
00452796  FMUL float ptr [0x004E0ED4] ; 1000.0f
004527A3  CALL 0x004B9400           ; x87 float to integer, toward zero
004527AE  MOV ECX,[global+0x6E4]
004527B4  PUSH EAX
004527BA  ADD ECX,0x30
004527BD  CALL 0x00486980           ; add named result statistic
```

The frame-delta source and its seconds conversion are recorded in
[`0042bf70_frame_clock_delta_fresh.c`](../runtime/0042bf70_frame_clock_delta_fresh.c).
The `1000` scale therefore produces a per-callback integer consistent with
milliseconds, though the Records display format and exact retail callback
cadence are not established here.

`Gallons Sprayed` is inside the current-hash function at `0044F340`. Its
`ACTION` predicate must be true and the item at selected slot `+0x357C` is
updated through vtable `+0x18`; only after that callback does the producer
check for slot 4 and add the converted frame delta. The x87 multiplier at
`004DFA18` is binary64 `1000.0`:

```asm
0044F6E0  TEST BL,BL                ; BL is the ACTION predicate result
0044F6E2  JZ 0x0044F790
0044F6E8  MOV EDI,[ESI+0x357C]     ; selected equipment slot
0044F703  CALL dword ptr [EDX+0x18] ; selected item update
0044F706  CMP dword ptr [ESI+0x357C],0x4
0044F70D  JNZ 0x0044F790
0044F713  PUSH 0x4DF4A0             ; "Gallons Sprayed"
0044F724  FLD float ptr [0x00502B04]
0044F72A  FMUL double ptr [0x004DFA18] ; 1000.0
0044F73B  CALL 0x004B9400           ; truncate toward zero
0044F740  MOV ECX,[global+0x6E4]
0044F74C  PUSH EAX
0044F755  ADD ECX,0x30
0044F758  CALL 0x00486980           ; add named result statistic
```

This establishes the numeric contribution and slot/action gate, not the
presentation unit implied by the name “Gallons.” The source does not gate this
stat on a fire contact. The host's held-action sampling cadence remains
provisional.

`Total Steps` is incremented by one in `0044EA90` when its 100 ms timer is
due; the function repeatedly samples until it chooses a sound variant
different from the previous one, then stores the current clock, adds one, and
requests sound playback. This is a source-backed increment trigger, but it does
not prove that each increment represents a literal physical step. The host has
no matching sound-selection scheduler, so this producer is not ported yet.

`Jumps` at `0044E990` is gated by a 500 ms clock comparison. State `+0x18C`
equal to 5 changes to 3 and sets `+0x100` to 400; otherwise byte `+0x12C` must
be nonzero, after which `+0x100` increases by 400. It then adds one to `Jumps`,
requests sound, and stores the current clock. The port's collision latch is not
yet proven equivalent to this retail byte, so no jump counter is wired.

`Air Time` at `004528B6` has no function owner in the saved project. The
instruction window tests byte `[ESI-0x14]`; only its zero branch loads
`00502B04`, multiplies by the same `1000.0f` constant, converts through
`004B9400`, and adds to the named statistic. The owning object/field identity,
transition gate, and update cadence remain unresolved. It is not mapped to the
host's airborne intervals.

The bounded movement and action-stat audits found no source-backed
`Objects Thrown` producer. Its eligible object, trigger, and value remain
unknown.

## Shared host change

The shared rescue loop now adds the source-truncated per-frame delta to
`Completion Time`. While the selected HoseSpray item is active under the
mode-1/mode-3 update gate, it adds the same delta to `Gallons Sprayed` without
requiring a fire hit. Both values are in-memory only. The host uses its paced
frame as the update callback; equivalent retail callback cadence has not been
verified. No profile serialization, Records display, test, or retail parity
claim is included in this change.

## Build checkpoint

The rescue-all macOS app rebuilt successfully after the shared-host change.
Standalone and packaged executables both have SHA-256
`b98f26372c8dc108834b8b748888abe75816d06e96049c6c9ac44efd09580a90`.
The app `Info.plist` validated, and the packaged catalog emitted eight
source-ordered rescue choices. The build emitted the existing warning that
Homebrew `json-c` was built for macOS 27.0 while the app targets macOS 13.3.
This confirms compilation and packaging only; no gameplay tests, interactive
run, or retail comparison were performed.

The associated bounded audit ledger is in `docs/WORK_QUEUE.md` under
“Delegated audit ledger (2026-10-19 to 2026-10-20)”.
