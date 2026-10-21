# Profile selection pointer writer

## Provenance

Read-only Ghidra 12.0.4 headless decompilation and x86 disassembly from the
disposable `FDNYPort` project copy. The primary saved project was not modified.
Input `FDNYFirefighter.exe` SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
No retail runtime or `.rb` profile was captured.

## Menu route

`0042B4E0` constructs the menu scene at `EverGame+0x3C8`, passing the
EverGame object as its constructor argument. `ConstructMenuAndLoadMenuResources`
(`0042DEC0`) stores that argument at scene `+0x154` and constructs scene child
1 with `004300C0`; child 2 is the level selector `0042F670`. The `004300C0`
vtable begins at `004DFD48`; slot 1 at `004DFD4C` points to callback
`00430060`. Its constructor builds six slot entries, six reset entries, and a
back action. This maps the callback's event groups to the profile menu.

For events 0 through 5, `00430060` obtains the EverGame through its parent
scene's `+0x154`, adds `+0x6CC` for the embedded profile manager, calls
`004239C0(manager, eventIndex)`, then calls `0042D3E0(scene, 2)` to switch to
the already mapped level-selector child. Events 6 through 11 call
`0042FF50(eventIndex-6)`, which builds the prompt `Reset <profile>'s career?`
and displays it through `0042D6E0`; event 12 calls the scene's back handler.

## Pointer write

The manager constructed at `EverGame+0x6CC` has its selected-record pointer at
manager `+0x18`, exactly `EverGame+0x6E4`. `004239C0` checks a nonnegative slot
against the manager count, verifies that the indexed record is non-null, then
stores that record pointer to manager `+0x18` at `004239ED`. An out-of-range
or null slot stores zero at `004239F4`. Its only direct code caller is
`00430060` at `00430093`, which passes the embedded manager base and a slot
event from the six-entry profile menu.

This resolves the static writer path. `00471030` therefore reads the currently
selected profile's `+0x2C` field when rebuilding `DIFFICULTY`; the profile
constructor default of 1 only describes a new record, and reset callback
`00425690` also sets `+0x2C` to 1 while clearing the level ordinal and saving
the reset profile. Existing `.rb` difficulty values may differ. No claim is
made about which slot a player selected in a live run, exact profile-file
contents, or save execution. The host's `DIFFICULTY="1"` seed matches a fresh
or reset profile only; existing-profile parity still needs profile loading and
runtime coverage.

## Raw instruction anchors

```text
00430083  MOV EAX,[EDI + 0x28]
00430086  MOV ECX,[EAX + 0x154]
0043008C  PUSH ESI
0043008D  ADD ECX,0x6CC
00430093  CALL 0x004239C0
00430098  MOV ECX,[EDI + 0x28]
0043009B  PUSH 0x2
0043009D  CALL 0x0042D3E0

004239D1  PUSH EDI
004239D2  CALL 0x0048D490
004239D7  MOV EAX,[ESI]
004239D9  CMP dword ptr [EAX + EDI*4],0
004239E7  MOV ECX,[ESI]
004239E9  MOV EDX,[ECX + EDI*4]
004239ED  MOV [ESI + 0x18],EDX
004239F4  MOV dword ptr [ESI + 0x18],0
```

Related evidence: [fresh-profile difficulty default](profile_difficulty_constructor_default_2026-10-21.md),
[profile manager initialization](../runtime-loop/00425090_profile_manager_initializer_fresh_2026-10-21.c),
[selector initial ordinal resolution](selector_initial_ordinal_resolution.md),
and [profile-backed score path](mission_outcome_score_profile_fresh_2026-10-20.md).
