# Axe strike candidate iterator module

`src/bmodel_candidate_iterator.h` implements the bounded intrusive-list cursor and traversal used by Axe strike `00460B90`. `00481BA0` derives the list/group address as `DAT_00502AAC + 0x54`, stores it as cursor word 0, registers the cursor through `00481B00`, and initializes cursor word 1 from list/group `+0x10`. The strike caller reads each node's object pointer from `node+0`, advances to `node+4` before its type/box/hit filters, and terminates when either cursor word 0 or the candidate object is null. The native helper preserves source ordering and pointer identity and does not filter or create candidates.

The original P-code fixture `bmodel-candidate-iterator-pcode-oracle.json` runs `00481BA0` against an explicit manager and empty, one-entry, and four-entry linked list layouts. Only list cursor registration `00481B00` is intercepted; there is no prior cursor, so cleanup helper `00429FE0` is not exercised. Original P-code comparison covers only `00481BA0`'s cursor list address and first-node initialization. The `controlled_input_objects` and `controlled_expected_order` fields describe controlled/source-derived links; that expected order is not emitted by original P-code. The standalone checker separately checks original cursor fields and native traversal against those controlled links/source logic. Focused CTest `bmodel_candidate_iterator_pcode` passes all three rows; it does not establish runtime candidate membership or live Axe list order. Build and run it directly with:

```sh
c++ -std=c++17 -Wall -Wextra -pedantic $(pkg-config --cflags json-c) tools/bmodel_candidate_iterator_pcode_check.cpp $(pkg-config --libs json-c) -o /tmp/bmodel-candidate-iterator-check
/tmp/bmodel-candidate-iterator-check evidence/hose-spray/bmodel-candidate-iterator-pcode-oracle.json
```

## Direct dependencies inspected

- `evidence/ghidra-original-c/00460B90.c`: strike caller's cursor fields, node dereference/advance, type-4 filter, radius-8 box overlap, and `0043E2A0` call.
- `evidence/item-pose/live-extinguisher/contact-dispatch/active-contact-call-path-assembly.txt`: instruction-level `00481BA0`/`00481C00` behavior, including global-to-group address calculation and registration call.
- `evidence/item-pose/live-extinguisher/contact-dispatch/contact-list-trace.md`: group `+0x54` initialization/mask and append behavior traced through `0049C4E0`, `00429490`, and `0042B1A0`; append order is FIFO and follows factory/BSP order. That evidence is for the shared category list path, with FireRescue type 6 examples.
- `evidence/hose-spray/axe-strike-and-bmodel-hit-audit.md` and `src/bmodel_axe_hit.h`: consumer context and completed per-candidate hit leaf; neither supplies Axe's live candidate snapshot.

The direct list-cursor/layout routine is now bounded and source-comparable. Full strike integration remains blocked on runtime population: the exact candidate membership/order and live BModel pointers for Axe's manager group have not been captured. The FireRescue trace documents the shared append mechanism but does not prove the Axe candidate snapshot. Recommended next step: capture the initialized `DAT_00502AAC+0x54` list after world/category registration in a controlled Axe strike fixture, including each node's object and next pointers, then feed that captured list to this iterator and the existing hit leaf. The manager/group lifecycle is supplied by the game runtime; this module deliberately does not synthesize it.
