/* Fresh Ghidra decompilation, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 0046E3D0 (HasScriptActorEvent)
 * The pseudocode and listing are analysis output, not recovered original source.
 */
/* Summary: Tests whether the ScriptActor event table contains the requested event key. BModel
   initialization uses it to test for `use` before assigning generic use mode 2. Evidence: fresh
   decompilation and raw x86 at 0x0046E3D0, cross-checked against BModel initializer 0x0043C670 in
   the hash-pinned executable. */

bool __thiscall HasScriptActorEvent(void *this,int *param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  piVar2 = param_1;
  iVar3 = FUN_0046e210((void *)((int)this + 8),(byte *)(param_1 + 3));
  if (iVar3 == 0) {
    iVar3 = 0;
  }
  else {
    iVar3 = iVar3 + 0xc;
  }
  if ((piVar2 != &DAT_00501908) && (iVar1 = *piVar2, *piVar2 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&param_1);
  }
  return iVar3 != 0;
}

