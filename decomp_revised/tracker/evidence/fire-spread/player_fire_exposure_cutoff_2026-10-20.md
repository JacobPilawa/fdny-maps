# Player fire exposure influence cutoff (2026-10-20)

## Pinned source and instructions

The source is `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
`objdump -d -Mintel` confirms the influence gate in `004483A0`:

```text
00448494: CALL 0044A7A0                   ; spatial falloff, x87 ST(0)
00448499: FMUL float ptr [ESP + 0x20]    ; intensity weight
0044849D: FST float ptr [004FF8E0]      ; copy to float; keep ST(0)
004484A3: FCOMP qword ptr [004E16B8]    ; compare retained result to f64 cutoff
004484A9: FNSTSW AX
004484AB: TEST AH, 0x05
004484AE: JNP 00448539                    ; reject below threshold / unordered
004484C8: CALL 0048FEA0                   ; visibility query
004484D0: TEST AL, AL
004484D2: JNE 004484E6
004484D4: FLD float ptr [004FF8E0]
004484DA: FMUL qword ptr [004DF7B8]     ; obstruction attenuation
004484E0: FSTP float ptr [004FF8E0]
004484E6: ...                             ; store record and send 0x7531
```

At data address `004E16B8`, the executable bytes are
`2d 43 1c eb e2 36 1a 3f`, little-endian binary64
`0x3f1a36e2eb1c432d`, value `0.0001`. The comparator is inclusive: values
equal to the cutoff continue to the visibility query. `004DF7B8` is binary64
`0.1`. After obstruction attenuation, the code stores the resulting float
influence and sends message `0x7531` without reapplying the cutoff.

## Port change and limits

`player_fire_contact.h` now names the verified cutoff as a `double`, and the
shared rescue host applies it before the visibility query. The host no longer
drops an obstructed candidate when its attenuated influence falls below that
cutoff; retail still queues the message in that case. This is a narrow
source-backed correction to contact eligibility.

The host retains a binary64 approximation of the pre-storage influence for its
cutoff check, while retail compares the live x87 extended result against the
binary64 cutoff. The host narrows the intensity weight to binary32 before
multiplication and narrows the message value before later visibility
attenuation, matching the traced storage points. Its spatial arithmetic still
uses binary64 rather than x87 extended precision. The host now supplies the
first flame sprite's strength-derived alpha override as the emitter multiplier;
the setter and falloff read are documented in
[`fire_emitter_alpha_exposure_2026-10-20.md`](fire_emitter_alpha_exposure_2026-10-20.md).
The alpha value before the first actor update and callback cadence remain
unresolved. Near-cutoff precision and retail damage feel also remain
unverified. This is not a retail-parity claim.
