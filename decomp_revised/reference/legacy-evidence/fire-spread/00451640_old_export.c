/* Ghidra decompilation; names/types are provisional.
 * Entry: 00451640
 * Function: FUN_00451640
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00451640(void *this,float *param_1,float param_2)

{
  float fVar1;
  float *pfVar2;
  float10 fVar3;
  float10 fVar4;
  ulonglong uVar5;
  int *piVar6;
  char cVar7;
  float local_74;
  float local_70;
  float local_6c;
  float local_68;
  float local_64;
  float local_60;
  undefined4 local_5c [3];
  undefined1 local_50 [68];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0318;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0x18c) != 6) {
    ExceptionList = &local_c;
    FUN_0042d2d0(local_50,0);
    pfVar2 = (float *)FUN_0043b2d0(&local_74,param_1,(float *)((int)this + 0x4c));
    local_68 = *pfVar2;
    local_64 = pfVar2[1];
    local_60 = pfVar2[2];
    pfVar2 = (float *)FUN_0049e770((void *)((int)this + 0x24),local_5c);
    local_74 = *pfVar2;
    local_6c = pfVar2[2];
    local_70 = pfVar2[1];
    fVar3 = (float10)fpatan((float10)local_6c,(float10)local_74);
    fVar4 = (float10)fpatan((float10)local_60,(float10)local_68);
    param_1 = (float *)(float)(fVar3 - fVar4);
    if ((float)_DAT_004e1840 < (float)param_1) {
      param_1 = (float *)((float)param_1 - (float)_DAT_004e1b30);
    }
    if ((float)param_1 < (float)_DAT_004e1830) {
      param_1 = (float *)((float)param_1 + (float)_DAT_004e1b30);
    }
    FUN_0044bb00(this,(float)param_1);
    if (*(int **)((int)this + 0x390) < DAT_00502afc) {
      cVar7 = '\0';
      piVar6 = DAT_00502afc;
      FUN_00402790(&stack0xffffff78,"RC_Pain");
      FUN_004897c0((void *)((int)this + 0x3540),piVar6,cVar7);
      FUN_00498b30(1000.0,2500.0);
      uVar5 = FUN_004b9400();
      *(int *)((int)this + 0x390) = (int)uVar5;
      FUN_00402790(&param_1,"Serious Burns");
      local_4 = 0;
      FUN_00486980((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&param_1,1);
      local_4 = 0xffffffff;
      if (param_1 != (float *)&DAT_00501908) {
        *param_1 = (float)((int)*param_1 + -1);
        if ((int)*param_1 < 1) {
          FUN_004011c0(&param_1);
        }
        else {
          param_1 = (float *)&DAT_00501908;
        }
      }
    }
    fVar1 = _DAT_004e1818 * DAT_00502b04 * param_2 + *(float *)((int)this + 0x3538);
    *(float *)((int)this + 0x3538) = fVar1;
    if ((float)_DAT_004dd808 < fVar1) {
      FUN_00450fb0(this,1);
    }
  }
  ExceptionList = local_c;
  return;
}

