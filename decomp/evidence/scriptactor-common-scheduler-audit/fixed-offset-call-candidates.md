# Fixed-offset +0x14 call-site scan

Read-only listing output from Ghidra 12.0.4, FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.
Query: FindIndirectCallSlot.java "+ 0x14" (literal fixed-offset CALL/JMP syntax only).
Each row records address, instruction, and containing function when Ghidra defines one.

The listing scan found these 53 literal fixed-offset candidates. Receiver types and reachability are unresolved unless the audit discusses a call site.

```text
00408414 CALL dword ptr [EAX + 0x14] function=<none>
0040868d JMP dword ptr [EAX + 0x14] function=00408660 FUN_00408660
00408a20 CALL dword ptr [EAX + 0x14] function=004089f0 FUN_004089f0
00409291 CALL dword ptr [EAX + 0x14] function=00409270 FUN_00409270
004094b9 CALL dword ptr [EDX + 0x14] function=00409400 FUN_00409400
0040a5ed CALL dword ptr [ESI + 0x14] function=0040a4d0 FUN_0040a4d0
0040c010 CALL dword ptr [EDX + 0x14] function=0040bd40 FUN_0040bd40
0040c112 CALL dword ptr [EDX + 0x14] function=0040bd40 FUN_0040bd40
00416bed CALL dword ptr [EDX + 0x14] function=004168b0 FUN_004168b0
00416c9e CALL dword ptr [EDX + 0x14] function=004168b0 FUN_004168b0
0041c77d CALL dword ptr [EAX + 0x14] function=0041c410 FUN_0041c410
0041c807 CALL dword ptr [EDX + 0x14] function=0041c410 FUN_0041c410
0041caf0 CALL dword ptr [EDI + 0x14] function=0041cae0 FUN_0041cae0
0041cbc5 JMP dword ptr [EAX + 0x14] function=<none>
00420c34 CALL dword ptr [EDX + 0x14] function=00420bb0 FUN_00420bb0
00426075 CALL dword ptr [EDX + 0x14] function=00425d50 FUN_00425d50
0042aed4 CALL dword ptr [EDX + 0x14] function=0042aea0 FUN_0042aea0
0042bdda CALL dword ptr [EAX + 0x14] function=<none>
0042d7a9 CALL dword ptr [EDX + 0x14] function=0042d6e0 FUN_0042d6e0
00436091 CALL dword ptr [EAX + 0x14] function=<none>
00439339 CALL dword ptr [EAX + 0x14] function=00439330 FUN_00439330
0044f632 CALL dword ptr [EAX + 0x14] function=0044f340 FUN_0044f340
0044fa9c CALL dword ptr [EDX + 0x14] function=0044f340 FUN_0044f340
00459519 CALL dword ptr [EDX + 0x14] function=00459350 FUN_00459350
0046269d CALL dword ptr [EAX + 0x14] function=<none>
004649cc CALL dword ptr [EDX + 0x14] function=<none>
00467e25 CALL dword ptr [EDX + 0x14] function=004677d0 FUN_004677d0
0046cdd2 CALL dword ptr [EDX + 0x14] function=0046c5a0 FUN_0046c5a0
0046d05f CALL dword ptr [EDX + 0x14] function=0046c5a0 FUN_0046c5a0
00487dbd CALL dword ptr [EDX + 0x14] function=00487a90 FUN_00487a90
00487e1b CALL dword ptr [EDX + 0x14] function=00487a90 FUN_00487a90
00487e6c CALL dword ptr [EDX + 0x14] function=00487a90 FUN_00487a90
00487ebd CALL dword ptr [EDX + 0x14] function=00487a90 FUN_00487a90
00487f0e CALL dword ptr [EDX + 0x14] function=00487a90 FUN_00487a90
00487f5c CALL dword ptr [EDX + 0x14] function=00487a90 FUN_00487a90
00489a99 CALL dword ptr [EDX + 0x14] function=00489a30 FUN_00489a30
00489b04 CALL dword ptr [EDX + 0x14] function=00489a30 FUN_00489a30
0049ee0d CALL dword ptr [EDX + 0x14] function=<none>
0049eece CALL dword ptr [EDX + 0x14] function=<none>
004a3935 CALL dword ptr [EAX + 0x14] function=004a38a2 FUN_004a38a2
004a3999 CALL dword ptr [EAX + 0x14] function=004a38a2 FUN_004a38a2
004a4215 CALL dword ptr [EAX + 0x14] function=004a41f6 FUN_004a41f6
004ae755 JMP dword ptr [EAX + 0x14] function=004ae735 FUN_004ae735
004b0cf1 CALL dword ptr [EAX + 0x14] function=004b0bc9 FUN_004b0bc9
004b0e3f CALL dword ptr [EAX + 0x14] function=004b0bc9 FUN_004b0bc9
004b0faf CALL dword ptr [EAX + 0x14] function=004b0bc9 FUN_004b0bc9
004b2d73 CALL dword ptr [EAX + 0x14] function=004b2d66 FUN_004b2d66
004b66df CALL dword ptr [EAX + 0x14] function=004b6600 FUN_004b6600
004b6812 CALL dword ptr [EAX + 0x14] function=004b6600 FUN_004b6600
004b6966 CALL dword ptr [EAX + 0x14] function=004b6600 FUN_004b6600
004b9582 CALL dword ptr [EBP + 0x14] function=004b9564 __ArrayUnwind
004b95ed CALL dword ptr [EBP + 0x14] function=004b95c2 `eh_vector_destructor_iterator'
004b9698 CALL dword ptr [EBP + 0x14] function=004b9674 `eh_vector_constructor_iterator'
MATCH_COUNT 53
```
