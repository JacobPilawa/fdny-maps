/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00493650 FUN_00493650 */

undefined4 __thiscall FUN_00493650(void *this,int param_1,char param_2)

{
  void **ppvVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int local_7c;
  char *local_78;
  char *local_74;
  undefined4 local_70;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d4258;
  local_c = ExceptionList;
  iVar3 = 0;
  ExceptionList = &local_c;
  ppvVar1 = &local_c;
  if (0 < *(int *)((int)this + 0x4c)) {
    do {
      if ((iVar3 < 0) || (*(int *)((int)this + 0x4c) <= iVar3)) {
        piVar4 = &local_7c;
        for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_7c = 1;
        local_78 = "Array index out of bounds";
        local_74 = "..\\BluelAPI/Container/SArray.h";
        local_70 = 0x1cd;
        FUN_00422060(&local_7c);
      }
      if (*(int *)(*(int *)(*(int *)((int)this + 0x48) + iVar3 * 4) + 4) == param_1) {
        if ((iVar3 < 0) || (*(int *)((int)this + 0x4c) <= iVar3)) {
          piVar4 = &local_7c;
          for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
            *piVar4 = 0;
            piVar4 = piVar4 + 1;
          }
          local_7c = 1;
          local_78 = "Array index out of bounds";
          local_74 = "..\\BluelAPI/Container/SArray.h";
          local_70 = 0x1cd;
          FUN_00422060(&local_7c);
        }
        ExceptionList = local_c;
        return **(undefined4 **)(*(int *)((int)this + 0x48) + iVar3 * 4);
      }
      iVar3 = iVar3 + 1;
      ppvVar1 = ExceptionList;
    } while (iVar3 < *(int *)((int)this + 0x4c));
  }
  ExceptionList = ppvVar1;
  if (param_2 != '\0') {
    local_64 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_60 = "ROC\\Renderer\\ROCRender.cpp";
    local_5c = 0x28a;
    local_58 = 0;
    local_4 = 0;
    FUN_004a3a19(local_48,"Undefined texture resource #");
    FUN_004a3f7a(local_48,param_1);
    FUN_00401360(&local_64,2);
    local_4 = 0xffffffff;
    FUN_004012c0(&local_64);
  }
  ExceptionList = local_c;
  return 0;
}



