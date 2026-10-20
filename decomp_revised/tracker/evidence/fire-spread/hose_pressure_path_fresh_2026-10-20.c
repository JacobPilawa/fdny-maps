/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x004653b0 FUN_004653b0 */

void __thiscall FUN_004653b0(void *this,int *param_1)

{
  uint uVar1;
  int *extraout_ECX;
  int *piVar2;
  char cVar3;
  
  uVar1 = FUN_00481f40((void *)((int)this + 0xbc0),param_1,5);
  if ((char)uVar1 != '\0') {
    cVar3 = '\x01';
    piVar2 = extraout_ECX;
    FUN_00402790(&stack0xfffffff4,"RC_HoseTimeWarn");
    FUN_004897c0((void *)(*(int *)((int)this + 4) + 0x208),piVar2,cVar3);
  }
  return;
}



/* 0x00464d50 FUN_00464d50 */

void __thiscall FUN_00464d50(void *this,int param_1,int param_2)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  int local_6c;
  char *local_68;
  char *local_64;
  undefined4 local_60;
  int local_54;
  char *local_50;
  char *local_4c;
  undefined4 local_48;
  int local_3c;
  char *local_38;
  char *local_34;
  undefined4 local_30;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1a3b;
  local_c = ExceptionList;
  if (param_1 == 2) {
    ExceptionList = &local_c;
    *(undefined4 *)((int)this + 0x10) = DAT_00502afc;
    *(int *)((int)this + 0xc) = param_2;
    iVar4 = 0;
    if (0 < *(int *)(param_2 + 0xc)) {
      piVar6 = (int *)((int)this + 0x14);
      do {
        if (*(int *)((int)this + 0x18) <= iVar4) {
          puVar1 = operator_new(0x124);
          puVar3 = (undefined4 *)0x0;
          local_4 = 0;
          if (puVar1 != (undefined4 *)0x0) {
            puVar3 = FUN_0047ddc0(puVar1);
          }
          iVar2 = *(int *)((int)this + 0x18);
          local_4 = 0xffffffff;
          FUN_0043b6e0(piVar6,iVar2 + 1);
          *(undefined4 **)(*piVar6 + iVar2 * 4) = puVar3;
          if ((iVar4 < 0) || (*(int *)((int)this + 0x18) <= iVar4)) {
            piVar5 = &local_6c;
            for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
              *piVar5 = 0;
              piVar5 = piVar5 + 1;
            }
            local_6c = 1;
            local_68 = "Array index out of bounds";
            local_64 = "..\\BluelAPI/Container/SArray.h";
            local_60 = 0x1cd;
            FUN_00422060(&local_6c);
          }
          FUN_0047ef60(*(void **)(*piVar6 + iVar4 * 4),0x13,'\x01');
          if ((iVar4 < 0) || (*(int *)((int)this + 0x18) <= iVar4)) {
            piVar5 = &local_54;
            for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
              *piVar5 = 0;
              piVar5 = piVar5 + 1;
            }
            local_54 = 1;
            local_50 = "Array index out of bounds";
            local_4c = "..\\BluelAPI/Container/SArray.h";
            local_48 = 0x1cd;
            FUN_00422060(&local_54);
          }
          FUN_0047dd30(*(void **)(*piVar6 + iVar4 * 4),0.75);
        }
        if ((iVar4 < 0) || (*(int *)((int)this + 0x18) <= iVar4)) {
          piVar5 = &local_3c;
          for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
            *piVar5 = 0;
            piVar5 = piVar5 + 1;
          }
          local_3c = 1;
          local_38 = "Array index out of bounds";
          local_34 = "..\\BluelAPI/Container/SArray.h";
          local_30 = 0x1cd;
          FUN_00422060(&local_3c);
        }
        FUN_0047dc90(*(void **)(*piVar6 + iVar4 * 4),'\x01');
        iVar4 = iVar4 + 1;
      } while (iVar4 < *(int *)(param_2 + 0xc));
    }
    if (iVar4 < *(int *)((int)this + 0x18)) {
      do {
        if ((iVar4 < 0) || (*(int *)((int)this + 0x18) <= iVar4)) {
          piVar6 = &local_3c;
          for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
            *piVar6 = 0;
            piVar6 = piVar6 + 1;
          }
          local_3c = 1;
          local_38 = "Array index out of bounds";
          local_34 = "..\\BluelAPI/Container/SArray.h";
          local_30 = 0x1cd;
          FUN_00422060(&local_3c);
        }
        FUN_0047dc90(*(void **)(*(int *)((int)this + 0x14) + iVar4 * 4),'\0');
        iVar4 = iVar4 + 1;
      } while (iVar4 < *(int *)((int)this + 0x18));
    }
  }
  else if (param_1 == 1) {
    iVar4 = 0;
    ExceptionList = &local_c;
    if (0 < *(int *)((int)this + 0x18)) {
      do {
        if ((iVar4 < 0) || (*(int *)((int)this + 0x18) <= iVar4)) {
          piVar6 = &local_24;
          for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
            *piVar6 = 0;
            piVar6 = piVar6 + 1;
          }
          local_24 = 1;
          local_20 = "Array index out of bounds";
          local_1c = "..\\BluelAPI/Container/SArray.h";
          local_18 = 0x1cd;
          FUN_00422060(&local_24);
        }
        FUN_0047dc90(*(void **)(*(int *)((int)this + 0x14) + iVar4 * 4),'\0');
        iVar4 = iVar4 + 1;
      } while (iVar4 < *(int *)((int)this + 0x18));
    }
    *(undefined4 *)((int)this + 0xc) = 0;
  }
  ExceptionList = local_c;
  return;
}



/* 0x00466010 FUN_00466010 */

undefined4 __thiscall FUN_00466010(void *this,int param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  iVar2 = 0;
  if (0 < *(int *)((int)this + 0x1a60)) {
    do {
      if ((iVar2 < 0) || (*(int *)((int)this + 0x1a60) <= iVar2)) {
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
      if (**(int **)(*(int *)((int)this + 0x1a5c) + iVar2 * 4) == param_1) {
        if ((iVar2 < 0) || (*(int *)((int)this + 0x1a60) <= iVar2)) {
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
        return *(undefined4 *)(*(int *)((int)this + 0x1a5c) + iVar2 * 4);
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)((int)this + 0x1a60));
  }
  return 0;
}



/* 0x00468540 FUN_00468540 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00468540(int param_1)

{
  float *pfVar1;
  float *pfVar2;
  int iVar3;
  undefined4 local_c [3];
  
  FUN_0042d280((void *)(*(int *)(param_1 + 0xb4) + 0xb4),local_c,
               (*(float *)(param_1 + 600) * (float)_DAT_004e27d8 + (float)_DAT_004e1b20) *
               *(float *)(param_1 + 0x254),
               (*(float *)(param_1 + 600) * (float)_DAT_004e1b20 + (float)_DAT_004e27d8) *
               *(float *)(param_1 + 0x250),0);
  iVar3 = FUN_0047f1a0(*(int *)(param_1 + 0xb4));
  *(undefined4 *)(iVar3 + 0x28) = *(undefined4 *)(param_1 + 0x4c);
  *(undefined4 *)(iVar3 + 0x2c) = *(undefined4 *)(param_1 + 0x50);
  *(undefined4 *)(iVar3 + 0x30) = *(undefined4 *)(param_1 + 0x54);
  iVar3 = FUN_0047f1a0(*(int *)(param_1 + 0xb4));
  *(float *)(iVar3 + 0x2c) =
       *(float *)(*(int *)(param_1 + 0xb4) + 0xb8) * (float)_DAT_004de6f8 + *(float *)(iVar3 + 0x2c)
  ;
  iVar3 = FUN_0047f1a0(*(int *)(param_1 + 0xb4));
  pfVar1 = (float *)(param_1 + 0x80);
  *pfVar1 = *(float *)(iVar3 + 0x28);
  *(undefined4 *)(param_1 + 0x84) = *(undefined4 *)(iVar3 + 0x2c);
  *(undefined4 *)(param_1 + 0x88) = *(undefined4 *)(iVar3 + 0x30);
  pfVar2 = (float *)(param_1 + 0x90);
  FUN_0042d280(pfVar2,local_c,*(undefined4 *)(param_1 + 0x68),*(undefined4 *)(param_1 + 0x6c),
               *(undefined4 *)(param_1 + 0x70));
  *pfVar2 = *pfVar2 - *pfVar1;
  *(float *)(param_1 + 0x94) = *(float *)(param_1 + 0x94) - *(float *)(param_1 + 0x84);
  *(float *)(param_1 + 0x98) = *(float *)(param_1 + 0x98) - *(float *)(param_1 + 0x88);
  pfVar2 = (float *)(param_1 + 0x9c);
  FUN_0042d280(pfVar2,local_c,*(undefined4 *)(param_1 + 0x74),*(undefined4 *)(param_1 + 0x78),
               *(undefined4 *)(param_1 + 0x7c));
  *pfVar2 = *pfVar2 - *pfVar1;
  *(float *)(param_1 + 0xa0) = *(float *)(param_1 + 0xa0) - *(float *)(param_1 + 0x84);
  *(float *)(param_1 + 0xa4) = *(float *)(param_1 + 0xa4) - *(float *)(param_1 + 0x88);
  return;
}



/* 0x00468680 FUN_00468680 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00468680(int param_1)

{
  int iVar1;
  float fVar2;
  
  iVar1 = *(int *)(param_1 + 0x2a8);
  if ((*(char *)(iVar1 + 0xc) != '\0') && (*(char *)(iVar1 + 0xe) != '\0')) {
    fVar2 = *(float *)(param_1 + 600) - DAT_00502b04 / *(float *)(iVar1 + 8);
    *(float *)(param_1 + 600) = fVar2;
    if (fVar2 < _DAT_004dc50c) {
      *(undefined1 *)(iVar1 + 0xd) = 0;
      *(undefined4 *)(param_1 + 600) = 0;
    }
    FUN_00468540(param_1);
    *(undefined1 *)(*(int *)(param_1 + 0x2a8) + 0xf) = 1;
  }
  return;
}



