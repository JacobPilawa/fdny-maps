# Executable compatibility and porting

The installer accepts these SHA-256 hashes of `FDNYFirefighter.exe`:

| Executable | SHA-256 |
| --- | --- |
| Preserved researched build | `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` |
| Same build with window-style patches | `bf82c9c0abfa6e3e951875e8e266d63b2e8918bafc3277d09903dcea5517e5f7` |

These files differ by four bytes in window-style constants. Callback/data layouts are unchanged. The runtime independently checks original vtable pointers and callback prologues before installing its hooks. Player signature failure installs no Player hooks. Victim signature failure leaves Player capture enabled and logs that victim capture is disabled.

The tested setup is the patched executable running in Wine. The preserved build shares the inspected callback signatures/layout; listing it is not a claim of a separate native Windows playthrough. An ASI loader and compatible game runtime are required.

## Address map

All addresses below are virtual addresses at preferred image base `0x00400000`. Source converts these to module-relative addresses at runtime. Do not apply the offsets to unrelated executables.

| Role | Address / local offset |
| --- | --- |
| Player initialization vtable entry → original callback | `0x004E1C68` → `0x0044D550` |
| Player root-update vtable entry → original callback | `0x004E1C5C` → `0x0044FE20` |
| Player movement child vtable entry (validation) | `0x004E1C40` → `0x00452570` |
| Victim spawn entry → original callback | `0x004E25DC` → `0x0045D5B0` |
| Victim root-update entry → original callback | `0x004E25CC` → `0x00459990` |
| Victim scalar deleting destructor entry → callback | `0x004E25B8` → `0x0045DF60` |
| Session pointer → level info → string characters | `*(0x00502AAC)` → `+0x688` pointer → string pointer → `+0x0C` |
| Game frame-delta field | `0x00502B04` |
| Body position, forward vector | root `+0x4C`, root `+0x40` |
| Local AABB minimum, maximum | root `+0x78`, root `+0x84` |
| Player velocity, ground-contact byte | root `+0xFC`, root `+0x12C` |
| Player state, pitch, selected slot | root `+0x18C`, `+0x3430`, `+0x357C` |
| Victim state, definition pointer/name field | root `+0x120`, root `+0x150` → `+0x10` |

The Player movement method receives an embedded subobject (`root+0x140`), while the recorder wraps the **root** update. Mixing those pointer bases produces plausible-looking but incorrect fields. Bounds are local offsets around body position, not an eye location. The victim destructor has a stack argument; see the wrapper and synthetic test for its calling convention.

## Adapting another build

Identify the matching lifecycle/update functions and pointer bases in that build, verify calling conventions and field layouts, then update signatures/RVAs and the installer hash list. Keep the fail-closed runtime checks. Rebuild and run the synthetic test, then verify spawn coordinates, stationary movement, turns, jumps, crouches, same-stage restarts and a stage change in actual gameplay. Check victim capture separately, including spawn/destruction and carry/release. Do not infer an accurate camera replay or complete simulation from a successful hook installation alone.
