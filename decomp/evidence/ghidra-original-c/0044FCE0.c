/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044fce0
 * Match: function entry
 * Entry: 0044fce0 FUN_0044fce0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void FUN_0044fce0(char param_1)

{
  void *this;
  undefined4 *puVar1;
  int local_14;
  undefined4 *local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cffe8;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  FUN_00481c00(&local_14);
  local_4 = 0;
  puVar1 = local_10;
  while( true ) {
    if ((local_14 == 0) || (puVar1 == (undefined4 *)0x0)) {
      this = (void *)0x0;
    }
    else {
      this = (void *)*puVar1;
    }
    if ((local_14 != 0) && (puVar1 != (undefined4 *)0x0)) {
      puVar1 = (undefined4 *)puVar1[1];
      local_10 = puVar1;
    }
    if (this == (void *)0x0) break;
    if (*(int *)((int)this + 0x18) == 3) {
      FUN_00446ec0(this,param_1);
      puVar1 = local_10;
    }
    else if (*(int *)((int)this + 0x18) == 8) {
      FUN_00440f50(this,param_1);
      puVar1 = local_10;
    }
  }
  local_4 = 0xffffffff;
  if (local_14 != 0) {
    FUN_00429fe0((void *)(local_14 + 0x28),(int)&local_14);
  }
  ExceptionList = pvStack_c;
  return;
}


