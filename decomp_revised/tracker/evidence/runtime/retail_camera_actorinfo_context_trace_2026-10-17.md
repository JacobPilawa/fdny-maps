# Retail camera and ActorInfo transform consumers (2026-10-17)

Read-only Ghidra 12.0.4 headless trace against the retail binaries:

- `FDNYFirefighter.exe` SHA-256:
  `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`
- `GENESIS.DLL` SHA-256:
  `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`

## Trace

- Camera virtual method `00491C60` calls
  `geCamera_SetWorldSpaceXForm(*(this + 0x5C), this + 0x10)`. Constructor
  `00491D70` initializes the camera pointer at `+0x5C` to null; `00491BE0`
  creates the Genesis camera and stores it there.
- `00491D00` publishes that camera pointer through its owner's `+0x170`
  field. If its context pointer at `+0x60` is present, it calls `00495450`.
- `00495450` builds a transform from that context via `0042D0A0` and
  `004977B0`, then passes the matrix to `00492110` through the ActorInfo pointer
  at context `+0x58`.
- `00492110` stores the 3×4 pose beginning at ActorInfo `+0x10` and dispatches
  the pose object's vtable slot `+0x18`. For camera objects this reaches
  `00491C60`; for held actors the equivalent slot reaches `00492450`, which
  calls `geActor_SetPose` with that matrix and scale fields `+0x40..+0x48`.
- Equipment setup `0045F660` parents its component follower to Player
  `+0x34A4`; `00497F70` resolves that component hierarchy and sends its matrix
  to `00492110`.

## Impact and remaining gap

The retail camera and held-tool consumers are now traced through their matrix
writers. This does **not** prove that the camera context hierarchy is derived
from Player view follower `+0x34A4`; the camera and held-tool hierarchies are
separate at the traced consumer boundary. A global basis flip or a per-tool
turn would therefore remain unsupported and could change map viewing,
extinguisher contacts, goggles, and every held ACT. The Axe swing-facing report
and extinguisher contact direction remain open until the camera-context parent
source is joined to the Player view frame or matched retail runtime matrices
are captured.

This is static transform-path evidence only. It contains no live matrix values,
interactive gameplay result, or claim of retail visual parity. The existing
port root `(35, -8, 140)` and camera-to-tool contact frame remain provisional.

## Fresh bounded recheck (2026-10-17)

I re-imported the pinned executable into a temporary Ghidra 12.0.4 headless
project and checked the camera consumer again. Automatic function discovery
did not create a function at `00491C60` because it is reached through a virtual
table entry; the table slot at `004E3B50` points to it. Manually disassembling
that entry confirms the camera consumer passes its stored camera at `+0x5C`
and transform at `+0x10` to `geCamera_SetWorldSpaceXForm`. `00491D70` still
stores transform parent `+0x58` and context `+0x60`; `00491BE0` creates the
camera, and `00491D00` publishes it and invokes the context update. The
hash-pinned pseudocode is in
[`retail_camera_context_trace_2026-10-17.c`](retail_camera_context_trace_2026-10-17.c).

This recheck confirms the camera consumer boundary, but it does not join the
camera node's runtime parent chain to Player follower `+0x34A4` or provide the
numeric camera/ActorInfo matrices at Axe swing time. The view-frame and Axe
direction reports therefore remain open; no basis flip or per-Axe rotation is
supported by the additional trace.
