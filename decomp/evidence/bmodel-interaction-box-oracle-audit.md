# BModel interaction-box oracle audit

Audited 2026-10-11 against `FDNYFirefighter.exe` in the read-only Ghidra 12.0.4 project copy at `/private/tmp/fdny-ghidra-ro-20261011`. The assembly below was exported with `ExportFunctionAssembly.java`; the signed-zero trace used a fresh `EmulatorHelper` with x87 control word `0x037f`, status word `0`, and reset registers for each call. No live game process was attached.

## Nine boundary mismatches are p-code false negatives

Oracle samples 16, 18, 28, 38, 40, 50, 60, 62, and 72 start exactly on a box minimum face and travel in the negative direction. The x87 path computes an exit parameter of negative zero and compares it with the initial near value, positive zero. In the sample-16 trace, `ST0` is `-0` after `FMUL ST1` at `00482C1F`; the following `FCOM`/`FNSTSW AX` should set the x87 equal condition. Instead the emulator leaves `FPUStatusWord=0x0100` (stale C0 from an earlier comparison). `TEST AH,0x5` at `00482C27` therefore sets PF=0, and `JNP 00482E8F` at `00482C2A` incorrectly takes the miss return. A hardware x87 equal result sets C3 but not C0/C2; the test is zero, so JNP is not taken. The fixture's untouched NaN sentinels for these samples are not valid expected output values. Treat these nine mismatches as an EmulatorHelper signed-zero condition-code limitation; do not change the ray algorithm on their basis.

## Three point-only ULP cases were an extra native rounding

Samples 11, 33, and 55 use one nonzero direction component with bits `0x90FD87B6` (−1.0000000031710769e−28) and start coordinate 2. Ghidra's `00489D80` calls `0045A230` to normalize direction, then `00447710` to multiply it by the float near distance, and `0042D050` to add the start point. `0045A230` copies the input vector to scratch and invokes `00459B80`. That routine leaves `1.0 / length` in x87 extended precision (`FDIVR [1.0f]` at `00459BBA`, `FLD ST0` at `00459BC0`), multiplies each original float component, and rounds only each component result at `FSTP float` (`00459BC4`, `00459BCB`, `00459BD4`). `00447710` and `0042D050` likewise store each multiply and add result as float.

The former native helper rounded the reciprocal to float before multiplying the direction, adding a rounding point absent from those instructions. `InteractionSphere::normalizedForPoint` now retains the `Extended` reciprocal through the component multiplies, matching the original operation points. The three point outputs now match the fixture; the existing `--interaction-box-check` reports only the nine p-code boundary mismatches among 268 samples. The p-code point fixture has not been independently captured from a running original game, so keep that provenance distinction explicit.

## Follow-up bit to verify

When the boundary path is accepted, the original stores the exit parameter to the far local with `FSTP float` and later copies the output bits with integer `MOV`s (`00482EA8`, `00482EEC`–`00482EF0`). The traced exit is `-0`, so the original path appears to preserve far bits `0x80000000`. The native `Extended::pack` canonicalizes a zero significand to positive zero. The current oracle cannot establish the expected far output because its signed-zero branch artifact returns before writing outputs; verify this bit after replacing or working around that p-code condition-code limitation.
