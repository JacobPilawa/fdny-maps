# Remaining `+0x14` candidate follow-up

This is a bounded follow-up to [`fixed-offset-call-candidates.md`](fixed-offset-call-candidates.md), not a claim that all 53 candidates were classified. I first read the existing common, Player, and Training-victim scheduler audits. Their established negative boundary remains: the identified app/owner/staged paths do not prove the recurring root `+0x14` updater bridge; known Player and victim root wrappers only show the downstream child update.

I selected five unresolved candidate functions with multiple calls or apparent manager/container context and checked their receiver/caller context in the preserved Ghidra 12.0.4 database (FDNYFirefighter.exe SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). Queries used `-noanalysis -readOnly` against a scratch copy.

| Candidate | Ghidra evidence | Assessment |
|---|---|---|
| `00420bb0` (`00420c34`) | Fresh decompilation: calls this object's `+0x10`, reads bytes using `+0x04`, compares the bytes with a local zero buffer, then invokes `+0x14` only on the non-equal/error path. | Stream/reader operation; receiver is not an actor-root list. The local compare is not an actor iteration. |
| `00425d50` (`00426075`) | Fresh decompilation has source strings `main\\PlayerManager.cpp`, `BeginNamedRBR`, and field names `Player`, `Name`, `LevelIndex`, `DifficultyLevel`, and `Score`. It loops over serialized fields and dispatches typed reader methods; the `+0x14` call handles `LevelIndex`. | Player-manager deserialization, not runtime actor updating. |
| `0042d6e0` (`0042d7a9`) | Fresh decompilation validates a container index against 7, then calls `(**(code **)(**(int **)(*(int *)(this+0x158)+0x18)+0x14))()`. The containing routine initializes/reads a six-entry array/container. | Nested element/container interface callback; no recurring root updater evidence. |
| `00487a90` (six listed callsites) | Previously inspected in this audit: the callsites are in `basic\\PlayerStats.cpp` rich-binary/XML property parsing (`value`, `idealValue`, `updateRule`, etc.); the receiver is the parser/reader interface. | Repeated calls in a parse loop, not actor-list scheduling. |
| `00489a30` (two listed callsites) | Previously inspected in this audit: type-based serializer dispatch calls reader `+0x14` amid `+0x0c/+0x10/+0x18` operations, then routes values to writer serialization callbacks. | Serialization interface dispatch, not a scheduler bridge. |

None of these five supplies evidence for the recurring Training root updater. This is a narrow negative result: the fixed-offset scan is syntactic, and no exhaustive receiver resolution, reachability proof, or computed-dispatch search was attempted. The three new decompilations were obtained with:

```sh
JAVA_HOME=/Library/Java/JavaVirtualMachines/temurin-21.jdk/Contents/Home \
PATH=/Library/Java/JavaVirtualMachines/temurin-21.jdk/Contents/Home/bin:/usr/bin:/bin \
JAVA_TOOL_OPTIONS=-Duser.home=/private/tmp/fdny-ghidra-user-home \
/Applications/Ghidra/support/analyzeHeadless /private/tmp/fdny-victim-release-query fdny \
  -process FDNYFirefighter.exe -noanalysis -readOnly \
  -scriptPath decomp/tools/ghidra -postScript DecompileFunctions.java \
  00420bb0 00425d50 0042d6e0
```

The most useful next step is to trace a concrete root-vtable entry (`0044FE20` Player or `00459990` victim) backward from its installation/registration and enumerate direct callers of the wrapper, then inspect only the actual manager/list loop those callers reveal. Repeating the `+0x14` syntax scan is unlikely to resolve this because it conflates parsers, frame subobjects, and root methods.
