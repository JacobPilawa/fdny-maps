
void __fastcall FUN_004943e0(int param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  if (*(int *)(param_1 + 0x78) != 0) {
    iVar2 = 0;
    if (0 < *(int *)(param_1 + 0x98)) {
      do {
        if ((iVar2 < 0) || (*(int *)(param_1 + 0x98) <= iVar2)) {
          piVar3 = &local_18;
          for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
            *piVar3 = 0;
            piVar3 = piVar3 + 1;
          }
          local_18 = 1;
          local_14 = "Array index out of bounds";
          local_10 = "..\\BluelAPI/Container/SArray.h";
          local_c = 0x1cd;
          FUN_00422060(&local_18);
        }
        (**(code **)(**(int **)(*(int *)(param_1 + 0x94) + iVar2 * 4) + 8))();
        iVar2 = iVar2 + 1;
      } while (iVar2 < *(int *)(param_1 + 0x98));
    }
    if (*(int *)(param_1 + 0x98) != 0) {
      FUN_0043b6e0((void *)(param_1 + 0x94),0);
    }
  }
  return;
}

