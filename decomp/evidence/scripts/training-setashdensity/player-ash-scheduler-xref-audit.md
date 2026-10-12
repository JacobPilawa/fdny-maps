# Player ash recurring-owner xref follow-up

Snapshot: 2026-10-12. Read-only Ghidra 12.0.4 queries against the
hash-pinned `FDNYFirefighter.exe` project; no project analysis changes were
saved. This follow-up checked `0044FE20`, `00452570`, `004E1C5C`, `004E1C40`,
`00470740`, `0049BD20`, `00429520`, `00499DE0`, `00499E60`, and `00503A78`.

## Bounded result

- `0044FE20` has only the incoming data reference `004E1C5C` in the bounded
  xref result. `00452570` has only incoming data reference `004E1C40`; references
  to that table entry are constructors, not a recurring owner call.
- Direct callers of shared updater `00470740` are class wrappers, including the
  known Player wrapper call at `0044FE5F`. No recurring owner call to that
  wrapper was found in the inspected routes.
- The `0049BD20` owner-manager pass and Player root slot `+0x48` implementation
  do not dispatch the Player `+0x14` updater. App registry `00499E60` invokes
  slot `+0x70`; the Player movement table maps that slot to a helper, and the
  inspected Player initializer contains no registration bridge.
- The inspected global singleton references identify the input-owner slot
  `+0x14` at `0042AED4`, not a recurring Player update owner.

This is a bounded xref/callsite result, not proof that the executable has no
other computed or runtime-registered path. The recurring Player/ash schedule,
callback cadence, and resulting shared-clock/RNG order remain unresolved.

## Next evidence

The highest-value follow-up is a bounded live Training trace at `0044FE20`:
record its caller and `00502AFC` alongside entry/exit state at `0042AEA0` and
`00429520`, plus BModel, Fire, and Victim callback hits. If no safe live capture
is available, trace Player construction into its runtime update-list
registration rather than repeating literal slot scans. See also
`descriptor-lifetime-followup.md` for the constructor-to-first-pool-callback
clock question.
