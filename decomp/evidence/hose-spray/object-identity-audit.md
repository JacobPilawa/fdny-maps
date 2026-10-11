# Hose Spray stream field identity audit

`00463EF0` reads Hose Spray `+0x620` as the contact range and `+0x624` as the gate byte. The saved direct-offset scan contains apparent writes, but they do not establish either field for a Hose Spray instance.

- The `00456090` `this+0x624` access is a HUD widget. Caller `00458400` passes `param_1+0x10bc` as `this`; it is not the separately allocated Hose Spray object.
- The `00461690`/`00461F80` hits belong to the extinguisher class. Factory `0044D080` allocates `0x8d0` bytes and constructs its `004E28C0` vtable path. Hose pickup `0044CAB0` allocates `0xD54` bytes and calls `004634F0`, which installs different vtables.
- The Hose Spray base constructor `0045F7E0`, initializer `00463700`, helper paths `0046DC80`/`0046D6B0`, and valve attachment `0044C840` were checked. Their preserved code does not establish item offsets `+0x620/+0x624`.

Raw/decompiled support is in `tool-field-accesses.tsv`, `field-writer-followup/constructor-field-assembly.txt`, `../item-pose/live-extinguisher/message-routing/00456090_FUN_00456090.c`, caller `../item-pose/live-extinguisher/callback-refs/00458400_FUN_00458400.c`, and extinguisher factory `../item-pose/live-extinguisher/callback-refs/extinguisher-init-callsite-exports/0044d080_FUN_0044d080.c`.

The Hose Spray range/gate writer or runtime initialization is still unknown. This scan cannot establish that allocated memory starts at zero or that the gate is enabled. Leave live Hose Spray contact/suppression disconnected until those values are sourced.
