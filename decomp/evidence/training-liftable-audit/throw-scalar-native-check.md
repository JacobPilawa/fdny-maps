# Native BModel throw scalar check

`decomp/src/throw_scalar.h` is an isolated scalar transcription of BModel setup `0043B580` and the scalar prefix of update `0043D330`. `--throw-scalar-check` compares it with [`throw-scalar-oracle.json`](throw-scalar-oracle.json). It covers setup scalar fields and update completion/point outputs; it does not call the BModel mutator, update model transforms, dispatch events, or claim a runtime schedule.

## Source mapping

Read-only Ghidra 12.0.4 inspection of the matching executable (`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`) verified:

- `0043B2D0` subtracts start minus end per component and stores three binary32 locals. `0043B580` loads and squares those float results, adds x²+y² then z², applies `FSQRT`, and multiplies by binary32 `0x3A888889` at `004E0BB8`.
- Setup stores the global tick at `+0x208`, `-384.0f` at `+0x210`, the starting middle coordinate at `+0x218`, and mode 2 at `+0x25C`. The duration is stored as float at `+0x20C` while its extended x87 value remains live. The following velocity expression uses that extended duration, then stores its result to `+0x214` as float.
- Update subtracts ticks in 32-bit integer arithmetic, sign-tests the result, adds binary64 `2^32` at `004DFC90` when negative, and multiplies by binary64 `0.001` at `004DFA28`. It compares that extended elapsed value to stored duration. Active interpolation stores the ratio to a float local before using it; x/z are endpoint interpolation and y is `(t*acceleration+initialVelocity)*t+initialMiddle`. The completion branch selects the endpoint based on the reverse byte.

The instruction sequence establishing the x87 sum order and live duration was checked in the Ghidra listing: `0043B5BA..0043B5F2`; update timing and interpolation are at `0043D400..0043D520`. Ghidra decompilation and those instruction listings agree with the scalar equations above. The binary64 constants were read from the executable bytes: `004DFC90 = 0x41F0000000000000` and `004DFA28 = 0x3F50624DD2F1A9FC`.

## Reproduction and limits

From the repository root, the listing/decompilation queries used the preserved project without analysis or writes:

```sh
mkdir -p /private/tmp/fdny-throw-native-ghidra-user
JAVA_HOME=/Library/Java/JavaVirtualMachines/temurin-25.jdk/Contents/Home \
JAVA_TOOL_OPTIONS='-Duser.home=/private/tmp/fdny-throw-native-ghidra-user' \
/Applications/Ghidra/support/analyzeHeadless decomp/evidence/ghidra fdny \
  -process FDNYFirefighter.exe -noanalysis -readOnly \
  -scriptPath decomp/tools/ghidra \
  -postScript ExportFunctionAssembly.java 0043B2D0 0043B580 0043D330 \
  -postScript DecompileFunctions.java 0043B2D0 0043B580 0043D330
```

Then build and run:

```sh
cmake --build decomp/build -j 2
./decomp/build/fdny-training --throw-scalar-check
```

Result: 4 setup rows / 24 fields and 16 update rows / 64 branch-and-point fields compared, zero mismatches. The fixture itself is Ghidra `EmulatorHelper` x86 p-code with x87 control word `0x037f`; this pass establishes native parity with that fixture, not live-game or hardware parity. It does not establish `0043DC60` dispatch timing or connect the scalar model to gameplay scheduling.
