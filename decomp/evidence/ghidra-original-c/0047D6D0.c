/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0047d6d0
 * Match: function entry
 * Entry: 0047d6d0 FUN_0047d6d0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __thiscall FUN_0047d6d0(void *this,undefined1 *param_1,undefined1 *param_2)

{
  ulonglong uVar1;
  
  FUN_0048d490(*(void **)((int)this + 0xc),*(int *)((int)this + 4));
  uVar1 = FUN_004b9400();
  *param_1 = (char)uVar1;
  FUN_0048d490(*(void **)((int)this + 0xc),*(int *)((int)this + 8));
  uVar1 = FUN_004b9400();
  *param_2 = (char)uVar1;
  if ((*(byte *)((int)this + 0x18) & 6) != 0) {
    FUN_0048d490(*(void **)((int)this + 0xc),*(int *)((int)this + 4));
    if ((*(uint *)((int)this + 0x18) & 2) == 0) {
      if ((*(uint *)((int)this + 0x18) & 4) != 0) {
        uVar1 = FUN_004b9400();
        *param_1 = (char)uVar1;
      }
    }
    else {
      uVar1 = FUN_004b9400();
      *param_1 = (char)uVar1;
      uVar1 = FUN_004b9400();
      *param_2 = (char)uVar1;
    }
    uVar1 = FUN_004b9400();
    *param_1 = (char)uVar1;
    uVar1 = FUN_004b9400();
    *param_2 = (char)uVar1;
  }
  return;
}


