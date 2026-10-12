# Ash particle byte-channel scale store audit

Snapshot: 2026-10-12. Read-only Ghidra 12.0.4 `FieldStoreAudit.java 0x20`
query against the hash-pinned original `FDNYFirefighter.exe`
(`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`) with
`-noanalysis -readOnly`. The helper reports any direct `MOV` destination whose
text contains offset `0x20`, so most rows are stack-local noise and must be
classified by receiver and call path.

## Findings

- Known particle scale stores remain the constructor `0047D330` at `0047D36B`
  and descriptor binder `0047D370` at `0047D3CD`. Both assign binary32 `1.0`
  to particle `+0x20` (`native-pool-component.md` and
  `ash-presentation-component.md`).
- A register-based candidate at `0047E217` belongs to draw callback
  `0047DF10`, but decompilation shows it copies `[param_1+0xD4]` to that
  callback object's `[param_1+0x20]`. `0047DF10` separately reads its ash
  particle pointer from `param_1+0x104` and passes that pointer to the
  descriptor helpers. The matching offset in the draw packet is therefore not
  a write to particle scale.
- The whole-program textual scan found additional register-based `+0x20`
  candidates in other function ranges. This helper does not infer receiver
  types, aliases, or indirect stores, so those matches alone do not establish
  another ash particle write.

For the mapped Training constructor/bind/update path, the source-backed scale
input remains `1.0`; the non-unit presentation-helper cases are generic branch
coverage, not observed Training values. This narrows the earlier open item but
does not prove that no untraced alias, indirect callback, or bulk copy can
mutate particle `+0x20`. Keep the native field explicit until its ownership and
full update closure are captured. See `presentation-callback-audit.md` for
remaining indirect calls and `ash-resource-registration-audit.md` for the
separate loader object with its own `+0x78` field.
