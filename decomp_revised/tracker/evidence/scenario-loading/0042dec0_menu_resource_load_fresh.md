# Menu constructor resource-loader callsite

Program: `FDNYFirefighter.exe` (SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`),
Ghidra address `0042DEC0`.

The constructor stores its input at `this+0x154`, initializes the shared
renderer/resource manager, sets the manager flag at `+0x160`, initializes the
menu renderer subobject, constructs a resource parser, and calls
`LoadResourceDefinitionsFile(parser, "data\\resdefs\\menu.rrf")` at
`0042DF45`. It later constructs the menu UI objects and clears the manager flag
at the end. This confirms the menu RRF is loaded from the menu-construction path;
it does not establish the full menu bitmap/UI resource semantics.

The caller-site bytes are preserved in
[`catalog_loader_callsite_context_fresh.json`](catalog_loader_callsite_context_fresh.json).
The full decompilation was reviewed in Ghidra; this focused note keeps the menu
RRF path separate from the rescue/hose request paths.
