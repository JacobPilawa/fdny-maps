/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044b510
 * Match: function entry
 * Entry: 0044b510 FUN_0044b510
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void * __fastcall FUN_0044b510(int param_1)

{
  void *pvVar1;
  int iVar2;
  int *piVar3;
  void *pvVar4;
  int iVar5;
  int *piVar6;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  iVar5 = 0;
  if (0 < *(int *)(param_1 + 0x74)) {
    do {
      if ((iVar5 < 0) || (*(int *)(param_1 + 0x74) <= iVar5)) {
        piVar6 = &local_18;
        for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
          *piVar6 = 0;
          piVar6 = piVar6 + 1;
        }
        local_18 = 1;
        local_14 = "Array index out of bounds";
        local_10 = "..\\BluelAPI/Container/SArray.h";
        local_c = 0x1cd;
        FUN_00422060(&local_18);
      }
      if (**(char **)(*(int *)(param_1 + 0x70) + iVar5 * 4) == '\0') {
        if ((iVar5 < 0) || (*(int *)(param_1 + 0x74) <= iVar5)) {
          piVar6 = &local_18;
          for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
            *piVar6 = 0;
            piVar6 = piVar6 + 1;
          }
          local_18 = 1;
          local_14 = "Array index out of bounds";
          local_10 = "..\\BluelAPI/Container/SArray.h";
          local_c = 0x1cd;
          FUN_00422060(&local_18);
        }
        return *(void **)(*(int *)(param_1 + 0x70) + iVar5 * 4);
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < *(int *)(param_1 + 0x74));
  }
  pvVar1 = operator_new(0x14);
  pvVar4 = (void *)0x0;
  if (pvVar1 != (void *)0x0) {
    *(undefined4 *)((int)pvVar1 + 8) = 0;
    *(undefined4 *)((int)pvVar1 + 0xc) = 0;
    *(undefined4 *)((int)pvVar1 + 0x10) = 0;
    pvVar4 = pvVar1;
  }
  piVar6 = (int *)(param_1 + 0x70);
  iVar5 = 0;
  if (0 < *(int *)(param_1 + 0x74)) {
    piVar3 = (int *)*piVar6;
    do {
      if (*piVar3 == 0) goto LAB_0044b5ba;
      iVar5 = iVar5 + 1;
      piVar3 = piVar3 + 1;
    } while (iVar5 < *(int *)(param_1 + 0x74));
  }
  FUN_0043b6e0(piVar6,iVar5 + 1);
LAB_0044b5ba:
  *(void **)(*piVar6 + iVar5 * 4) = pvVar4;
  return pvVar4;
}


