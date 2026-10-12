/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 004467e0
 * Match: function entry
 * Entry: 004467e0 FUN_004467e0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004467e0(int param_1)

{
  void *this;
  void *this_00;
  float *pfVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  float10 fVar5;
  float10 fVar6;
  float10 fVar7;
  ulonglong uVar8;
  float fVar9;
  float fStack_54;
  float fStack_50;
  float fStack_4c;
  int local_48;
  char *local_44;
  char *local_40;
  undefined4 local_3c;
  float local_30;
  char *local_2c;
  char *local_28;
  undefined4 local_24;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  iVar3 = 0;
  if (0 < *(int *)(param_1 + 0x3c)) {
    do {
      if ((iVar3 < 0) || (*(int *)(param_1 + 0x3c) <= iVar3)) {
        pfVar1 = &local_30;
        for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
          *pfVar1 = 0.0;
          pfVar1 = pfVar1 + 1;
        }
        local_30 = 1.4013e-45;
        local_2c = "Array index out of bounds";
        local_28 = "..\\BluelAPI/Container/SArray.h";
        local_24 = 0x1cd;
        FUN_00422060((int *)&local_30);
      }
      FUN_00446510(*(int *)(*(int *)(param_1 + 0x38) + iVar3 * 4));
      if ((iVar3 < 0) || (*(int *)(param_1 + 0x3c) <= iVar3)) {
        piVar4 = &local_48;
        for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_48 = 1;
        local_44 = "Array index out of bounds";
        local_40 = "..\\BluelAPI/Container/SArray.h";
        local_3c = 0x1cd;
        FUN_00422060(&local_48);
      }
      FUN_0047f960(*(void **)(*(int *)(param_1 + 0x38) + iVar3 * 4),*(void **)(param_1 + 0x50));
      iVar3 = iVar3 + 1;
    } while (iVar3 < *(int *)(param_1 + 0x3c));
  }
  uVar8 = FUN_004b9400();
  iVar3 = 0;
  if (0 < (int)uVar8) {
    do {
      if ((iVar3 < 0) || (*(int *)(param_1 + 0x3c) <= iVar3)) {
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
      this = *(void **)(*(int *)(param_1 + 0x38) + iVar3 * 4);
      if (*(char *)((int)this + 0x30) == '\0') {
        fVar5 = FUN_00498b30(0.0,6.2831855);
        fVar6 = FUN_00498b30(10.0,400.0);
        iVar2 = (**(code **)(**(int **)(param_1 + 0x58) + 0x18))();
        fVar7 = (float10)fcos((float10)(float)fVar5);
        fStack_54 = (float)(fVar7 * (float10)(float)fVar6 + (float10)*(float *)(iVar2 + 0x28));
        iVar2 = (**(code **)(**(int **)(param_1 + 0x58) + 0x18))();
        fVar5 = (float10)fsin((float10)(float)fVar5);
        fStack_4c = (float)(fVar5 * (float10)(float)fVar6 + (float10)*(float *)(iVar2 + 0x30));
        iVar2 = (**(code **)(**(int **)(param_1 + 0x58) + 0x18))();
        fStack_50 = *(float *)(iVar2 + 0x2c) + _DAT_004e1530;
        fVar9 = 75.0;
        piVar4 = &local_48;
        this_00 = (void *)(**(code **)(**(int **)(param_1 + 0x58) + 0x18))();
        pfVar1 = (float *)FUN_0049e770(this_00,piVar4);
        pfVar1 = (float *)FUN_00446390(&local_30,pfVar1,fVar9);
        fStack_54 = fStack_54 + *pfVar1;
        fStack_50 = fStack_50 + pfVar1[1];
        fStack_4c = fStack_4c + pfVar1[2];
        FUN_00446470(this,&fStack_54);
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < (int)uVar8);
  }
  return;
}


