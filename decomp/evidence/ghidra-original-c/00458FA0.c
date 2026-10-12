/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00458fa0
 * Match: function entry
 * Entry: 00458fa0 FUN_00458fa0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


uint __fastcall FUN_00458fa0(int param_1)

{
  int iVar1;
  int *piVar2;
  uint in_EAX;
  undefined4 uVar3;
  
  iVar1 = *(int *)(param_1 + 0x1a0);
  if ((-1 < iVar1) && (iVar1 < *(int *)(param_1 + 0x18c))) {
    FUN_0048d490((uint *)(param_1 + 0x188),iVar1);
    in_EAX = *(uint *)(param_1 + 0x188);
    piVar2 = *(int **)(in_EAX + iVar1 * 4);
    if (piVar2 != (int *)0x0) {
      (**(code **)(*piVar2 + 4))(*(undefined4 *)(param_1 + 0x1a4));
      uVar3 = FUN_004a11a0((void *)(param_1 + 0x140),(undefined1 *)0x0);
      return CONCAT31((int3)((uint)uVar3 >> 8),1);
    }
  }
  return in_EAX & 0xffffff00;
}


