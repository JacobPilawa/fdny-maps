# Retail camera context ancestry follow-up (2026-10-20)

Read-only Ghidra 12.0.4 headless follow-up against the pinned retail binaries:

- `FDNYFirefighter.exe` SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`
- `GENESIS.DLL` SHA-256: `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`

## Confirmed static path

- Camera method `00491C60` passes the Genesis camera at object `+0x5C` and the transform at `+0x10` to `geCamera_SetWorldSpaceXForm`. Constructor `00491D70` stores a transform parent at `+0x58` and a context at `+0x60`; `00491BE0` creates the camera, and `00491D00` publishes it through owner `+0x170` before updating a non-null context.
- `00495450` obtains a context frame through `0042D0A0` and parent-aware `004977B0`, extracts its basis, then sends a matrix through the context ActorInfo at `+0x58` and `00492110` to the camera pose consumer.
- Player update `0044F340` updates its follower from view frame `+0x3460` into `+0x34A4`, then calls `00495450` for linked Type-5 contexts. Equipment components have a separate path parented to Player follower `+0x34A4` before their matrices reach `00492110`.

## Boundary and open work

These static call paths do not establish that any Type-5 camera context is parented to Player view frame `+0x3460` or follower `+0x34A4`. No runtime pointer identity, parent chain, callback schedule, or numeric camera/ActorInfo matrix was captured. The camera-to-follower join and Axe swing-time render pose therefore remain unresolved; this trace supports no global basis flip or per-Axe rotation.

This is static binary evidence, not an interactive retail check or visual-parity claim. The fuller consumer trace remains in [`retail_camera_actorinfo_context_trace_2026-10-17.md`](retail_camera_actorinfo_context_trace_2026-10-17.md).
