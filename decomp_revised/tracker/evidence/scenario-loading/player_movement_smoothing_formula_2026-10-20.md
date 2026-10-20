# Player movement smoothing formula (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Read-only Ghidra 12.0.4 exports from the saved `FDNYPort` project were
rechecked with `-readOnly -noanalysis`. Relevant entries are `00482AD0`,
`00482FC0`, and the Player movement callsite `004532B3..004532DD`.

## Retail calculation

The movement block passes base `0.95` or `0.2` and multiplier `27` to
`00482AD0`. For bases in the normal range, that helper returns

```text
halfLife = -ln(0.5) / (27 * ln(base))
```

The x87 sequence at `00482B00..00482B12` loads `ln(2)`, evaluates
`ln(base)` with `FYL2X`, multiplies by the second argument, then reverse-divides
the binary64 constant at `004E36C0` (`-ln(2)`). Its strict boundary fallbacks
are `1e-5` when `base < 1e-9` and `1e20` when `base > 0.999999999`; neither
branch is taken for the movement bases 0.2 and 0.95.

At `004532B3`, the returned half-life is passed to `00482FC0` with the shared
frame delta at `00502B04`. That helper evaluates
`pow(0.5, frameDelta / halfLife)`, stores the weight as a float, and blends the
current vector with the target vector. For the two fixed movement bases,

```text
pow(0.5, dt / halfLife) = pow(base, 27 * dt)
```

algebraically. Thus the host's `std::pow(base, 27*dt)` uses the same exponent
relationship when its selected base and delta match. This does not establish
bitwise equivalence: retail uses x87 and stores the power result as a float,
the host currently keeps a double weight, and its base condition uses host
`ground`/`climbing` flags. Retail selects the base from state 5, byte `+0x380`,
or contact latch `+0x12C`; callback cadence also remains unresolved. The
existing expression is therefore mathematically source-shaped, but the host
motion remains provisional.

## Key instructions and constants

```text
00453285  MOV AL,byte ptr [ESI + 0x240]   ; Player root +0x380
0045328D  MOV dword ptr [ESP + 0x24],0x3f733333 ; base 0.95
00453297  MOV dword ptr [ESP + 0x24],0x3e4ccccd ; base 0.2
004532A6  PUSH 0x41d80000                 ; multiplier 27.0
004532B3  CALL 0x00482ad0
004532DD  CALL 0x00482fc0
00482FC3  FLD double ptr [0x004de6f8]     ; 0.5
00482FCA  FLD float ptr [0x00502b04]      ; shared frame delta
00482FD0  FDIV float ptr [ESP + 0x2c]     ; divide by half-life
00482FD4  CALL 0x004b9a20                 ; x87 power helper
00482FE7  FSUB float ptr [ESP + 0x2c]     ; 1.0 - weight
```

The double constant at `004DE6F8` is verified as 0.5. The helper constants at
`004E36C0`, `004E36C8`, `004E36D0`, `004E36D8`, and `004E36E0` were read from
the pinned binary with `ReadFloatBits.java`.

