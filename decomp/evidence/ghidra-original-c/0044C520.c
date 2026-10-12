/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044c520
 * Match: function entry
 * Entry: 0044c520 FUN_0044c520
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


uint __thiscall FUN_0044c520(void *this,int param_1)

{
  int iVar1;
  int *piVar2;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  if (*(int *)((int)this + 0x3568) < 5) {
    piVar2 = &local_18;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar2 = 0;
      piVar2 = piVar2 + 1;
    }
    local_18 = 1;
    local_14 = "Array index out of bounds";
    local_10 = "..\\BluelAPI/Container/SArray.h";
    local_c = 0x1cd;
    FUN_00422060(&local_18);
  }
  iVar1 = *(int *)(*(int *)((int)this + 0x3564) + 0x10);
  if ((((iVar1 == 0) || (*(int *)(iVar1 + 0xc68) != 3)) && (*(char *)((int)this + 300) != '\0')) &&
     (*(int *)((int)this + 0x3440) == 0)) {
    return (*(int *)(param_1 + 0x25c) != 0) - 1 & 0x3ec;
  }
  return 0;
}


