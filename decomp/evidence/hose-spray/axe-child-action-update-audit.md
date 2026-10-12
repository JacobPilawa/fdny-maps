# Training Axe child held-action callback audit

Recorded 2026-10-12 against hash-pinned `FDNYFirefighter.exe` (`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). This is the separate child vtable `+0x18` body `00460920`, called from the Axe root held-ACTION route `0045FAD0`. The vtable slot is confirmed at table `004E27F0`; the callback component executes only one explicit invocation and does not establish recurring caller cadence.

## Source path

Original instructions in `00460920` show:

- When child state `+0x368` is zero, a false byte global `DAT_00501918` triggers stat-record initialization. The code stores DWORDs `2`, `0`, `1` at `00501908`, `0050190C`, `00501910`, then stores a single zero BYTE at `00501914`, and sets `00501918=1`. The native model preserves the upper three bytes of that last DWORD.
- The body calls temporary string constructor `00401D80`, Axe Hits statistic service `00486950`, then compares the returned string-record pointer with `00501908`. Non-alias records have their reference count decremented; `004011C0` is called only when the resulting count is below one. A global-record alias skips decrement/release. The native/P-code comparison injects both constructor return forms at this explicit service boundary and lets the original caller-side branch execute.
- It writes child state `+0x368=1`, calls child hide service `004A11A0` on `child+0x3B8` with zero, and returns.
- When state is 2, it adds the unsigned DWORDs at `+0x36C` and `+0x370`; only if the 32-bit sum is strictly earlier than `DAT_00502AFC` does it call recovery `0045FA60(child,1.0f)`. State 1, state 3, and other nonzero states are no-ops in this callback.

The four stat words/byte and one-time flag are represented in process-global `Globals`, separate from child state, so their identity can be shared when the two child objects are composed. `DAT_00502E04` is a separate shared global; the original body does not write it in these controlled executions. There is no sound call in this body.

## Original-P-code comparison

`tools/ghidra/HoseAxeChildActionUpdatePcodeOracle.java` executes original x86 PE32 P-code for 11 cases. It compares idle initialization (including the byte-width store with a sentinel upper-byte check), already-initialized idle, temporary refcount release/retain and controlled record-alias branches, state 1/no-op states, and due/equal/future/unsigned-wrap state-2 timers. The string constructor, statistic manager, string destructor, child hide, and recovery callees are explicit intercepted boundaries; the code in `00460920` that orders and conditions them, writes globals/state, and evaluates the timer runs in the emulator. The fixture captures `DAT_00502E04` unchanged.

Fixture: `axe-child-action-update-pcode-oracle.json`. Native component: `src/axe_child_action_update.h`. Comparison: `tools/axe_child_action_update_pcode_check.cpp`; focused CTest `axe_child_action_update_pcode` passes all 11 cases with zero mismatches.

## Boundaries

This is a connectable callback chunk only. It does not implement Axe strike geometry/handler `00460B90`, the separate child `+0x20` Think body `004610E0`, recovery internals `0045FA60`, stat storage, string implementation, audio, player/model/world services, frame order, caller cadence, or live Training values. Neither this callback nor the companion Think comparison claims a hit or whole-game Axe behavior. See [`axe-child-think-audit.md`](axe-child-think-audit.md) for the `+0x20` state dispatcher/model-timer slice.
