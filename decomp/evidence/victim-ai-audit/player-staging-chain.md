# Player factory to staged initialization

The normal successful level-loading path statically establishes Player participation in staged initialization. This closes the conditional registration gap in the earlier deadline-input audit; it does not establish recurring Player/root update timing or uniqueness of initializer invocation.

## Verified chain

1. Owner construction `0042B4E0` creates `0042B350` at owner+0x3C0 and publishes it as `*(00503A78)+0x24`. Its primary table is `004E4158` (+4 -> append `0042B1A0`), secondary table at +0x40 is `004DF8F8` (+4 -> drain `0049C670`).
2. Factory registration `004297E0` constructs `0044BFF0`, naming `RCS_PlayerRescue`, and appends it with `0049F160`. The factory primary table `004E1860` has byte +8 -> `00454080`. That allocator calls `00453D50`, registers the result through `0049D450`, and returns the primary Player pointer.
3. Level loader `0042AB00` restores app+0x688 to the supplied current level before BSP loading. It creates a World request with factory at request+0x0C and appends it to the spawn queue (call `0042ADDB`).
4. World factory `0049E300` returns `0049E240` with primary table `004E4458`; byte +0x20 (`004E4478`) is `0049E4D0`. This first initialization pass visits registered factories in list order, then each matching BSP entity. For each entity it creates a request with the factory pointer at +0x0C and appends it through queue primary +4. Training has concrete `RCS_PlayerRescue` entity 115, authored spawn message 10000.
5. World-generated requests are appended during a first-pass callback, after that drain has completed its construction loop. They remain for the next drain; they do not join that first callback batch. Owner service `00429520` calls staging secondary +4 at `0042956E`. The exact rendered-frame latency is not assigned here.
6. At `0049C6E7`, drain `0049C670` reads request+0x0C into ECX. This is a **factory pointer**, not a subobject of the request. It calls factory primary +8 with the request as the stack argument at `0049C6ED`. For Player this returns the primary Player pointer. `0049C6F6` retains it, registration `00429490` follows, then it is appended to the local batch.
7. After all construction requests, the first ordered batch pass invokes primary +0x20 (`0049C77A`); the second complete pass invokes primary +0x24 (`0049C7CF`). Player's installed primary table `004E1C48` maps these to `0044D550` and no-op `0043F660`. Thus the first pass performs the recovered deadline input initialization using the restored current level.

The existing `SpawnQueue` and factory-ordered Training startup representation already implement the ordered batch abstraction. This finding supplies the missing source registration proof, not a new recurring scheduler. Continuous carry, root +0x14 and visual Think cadence remain unresolved.

## Evidence

Fresh read-only Ghidra logs are retained under ignored `player-staging-native/`: entry/factory/queue-table/staging-chain/world-loader/world-init-target queries. They verify raw table words, exact indirect instruction operands and allocator return flow. Existing exports under `entity-bootstrap/`, `entity-passes/`, `entity-queues/`, `entity-dispatch/`, `startup-world/` corroborate the path. Training entity 115 is in `assets/training/entities.json`. Program SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. Allocation failure, alternate loaders and other computed call paths are outside this positive successful-path proof.
