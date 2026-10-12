/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044c360
 * Match: function entry
 * Entry: 0044c360 FUN_0044c360
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __fastcall FUN_0044c360(void *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  iVar3 = 0;
  if (0 < *(int *)((int)param_1 + 0x3568)) {
    do {
      if ((iVar3 < 0) || (*(int *)((int)param_1 + 0x3568) <= iVar3)) {
        piVar4 = &local_18;
        for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_18 = 1;
        local_14 = "Array index out of bounds";
        local_10 = "..\\BluelAPI/Container/SArray.h";
        local_c = 0x1cd;
        FUN_00422060(&local_18);
      }
      if ((*(int *)(*(int *)((int)param_1 + 0x3564) + iVar3 * 4) != 0) &&
         (uVar1 = FUN_0044c2a0(param_1,iVar3,'\x01'), (char)uVar1 != '\0')) {
        return;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < *(int *)((int)param_1 + 0x3568));
  }
  *(undefined4 *)((int)param_1 + 0x357c) = 0;
  return;
}


