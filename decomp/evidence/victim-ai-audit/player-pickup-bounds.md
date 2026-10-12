# Player bounds and victim pickup overlap

Ghidra source used for this trace: `ghidra-refresh/rescue-flow-20261011/0044d550_FUN_0044d550.c`, `0045a070_FUN_0045a070.c`, `0049d790_FUN_0049d790.c`, and `0049db90_thunk_FUN_0049d790.c`. The original image is `FDNYFirefighter.exe`; relevant vtable and scalar data were read from the same Ghidra project. The current native helper is intentionally isolated: callback order and runtime scheduling are not known.

## Recovered bounds

- Player primary vtable `004E1C48`, entry index 8 (byte offset `+0x20`), enters initializer `0044D550`. It initializes local minimum `(-30,-30,-30)` at player `+0x78..+0x80` and maximum `(30, DAT_004E181C, 30)` at `+0x84..+0x8c`. Ghidra's scalar word at `004E181C` is `0x42AA0000`, or `85.0f`.
- Player primary vtable slot `+0x0c` is thunk `0049DB90` to `0049D790`. That routine transforms the origin of the frame at player `+0x24` into world coordinates and stores it at `+0x68..+0x70`. This is the origin returned to victim callback `0045A070`.
- Crouch setter `0044BCB0` changes max Y at `+0x88` to scalar `004E1820` (`0x41C80000`, `25.0f`) and state `+0x18c` to 4, provided the prior state is not 4 and byte `+0x12c` is nonzero. Uncrouch `0044BCE0` tests a proposed max Y of 85 through collision routine `00492DF0`; only on a clear result does it restore max Y to 85 and state to 3. The overlap therefore uses the current box height, not a fixed standing height.

## Callback predicate

Victim callback `0045A070` first rejects victim states 5 and 6, requires the other entity's type at `+0x18` to equal 1, and calls its vtable slot `+0x0c`. A null returned world-box pointer ends the callback. It resolves radius from victim `+0x150+0x1c` through `00459B40(radius, 0)`; when that helper result is less than 1, it substitutes `60.0f` (`0x42700000`). Training victims 191 and 194 both author `J_Radius=60`, so their callback uses that authored radius.

For victim center `(cx,cy,cz)`, resolved radius `r`, player world origin `(ox,oy,oz)`, local minimum `min`, and local maximum `max`, Ghidra's six inclusive tests are evaluated in X, Z, then Y order:

```text
ox + min.x <= r + cx    &&    cx - r <= ox + max.x
oz + min.z <= r + cz    &&    cz - r <= oz + max.z
oy + min.y <= r + cy    &&    cy - r <= oy + max.y
```

Equality is accepted on every face. The machine code uses x87 floating-point additions followed by ordered comparisons. The native helper expresses the same source inequalities using binary64 intermediates; its small boundary harness checks the recovered formula and gates, not a direct original-DLL numerical oracle. Extreme-magnitude rounding and generic invalid-radius behavior remain outside this helper. `00459B40` delegates its validity test to `0043B120`, so that bit-level comparator/fallback is not replaced by a guessed C++ comparison.

When all tests pass, `0045A070` queues message `0x7532` with the victim pointer for the other entity. The owner update path reaches victim `+0x48` callbacks before its message drain, so the message can be delivered in that owner update. The Player movement/update ordering relative to this callback is still unresolved; no scheduler connection is justified yet.

## Native boundary

`src/victim_pickup.h` now contains the source-backed box dimensions and isolated overlap/gate predicate. The `--victim-pickup-geometry-check` command exercises all six player box faces, the crouch/standing height difference, and the state/type/box gates (14 total source-derived cases). This is a component check only. No victim pickup message, carry state, callback schedule, or exit path has been wired into the Training loop.
