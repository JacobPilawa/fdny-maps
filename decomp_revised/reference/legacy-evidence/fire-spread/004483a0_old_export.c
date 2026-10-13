/* Ghidra decompilation; names/types are provisional.
 * Entry: 004483a0
 * Function: FUN_004483a0
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_004483a0(void *param_1,int *param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  bool bVar4;
  int *piVar5;
  float10 fVar6;
  ulonglong uVar7;
  double dStack_8;
  
  if ((((*(int *)((int)param_1 + 0x70) != 5) && (-1 < *(int *)((int)param_1 + 0xb8))) &&
      (*(int *)((int)param_1 + 0xb8) < 4)) &&
     (DAT_00502b94 = (float *)(**(code **)(*param_2 + 0xc))(), DAT_00502b94 != (float *)0x0)) {
    dStack_8 = (double)ABS(DAT_00502b94[2] - *(float *)((int)param_1 + 0x84));
    uVar7 = FUN_004b9400();
    FUN_00448320(param_1,(int)uVar7);
    if (*(float *)((int)param_1 + 0x120) <= _DAT_004e16c0) {
      fVar2 = _DAT_004dfc24 - *(float *)((int)param_1 + 0x128);
      fVar3 = (float)_DAT_004e0508;
      fVar1 = *(float *)((int)param_1 + 0x128);
      fVar6 = FUN_0044a7a0((int)param_1 + 300,DAT_00502b94,DAT_00502b94[3]);
      fVar6 = fVar6 * (float10)(fVar2 * fVar3 + fVar1);
      _DAT_004ff8e0 = (float)fVar6;
      if ((float10)_DAT_004e16b8 <= fVar6) {
        bVar4 = FUN_0048fea0(*(int *)(DAT_00503a78 + 4),DAT_00502b94,
                             (undefined4 *)((int)param_1 + 0x7c),'\0');
        if (!bVar4) {
          _DAT_004ff8e0 = _DAT_004ff8e0 * (float)_DAT_004df7b8;
        }
        fVar1 = _DAT_004ff8e0;
        *(void **)((int)param_1 + *(int *)((int)param_1 + 0xb8) * 8 + 0xbc) = param_1;
        *(float *)((int)param_1 + *(int *)((int)param_1 + 0xb8) * 8 + 0xc0) = fVar1;
        piVar5 = (int *)FUN_0049d350(param_2,(undefined4 *)&dStack_8);
        FUN_0049d4e0(*piVar5,piVar5[1],0x7531,
                     (int)param_1 + *(int *)((int)param_1 + 0xb8) * 8 + 0xbc);
        *(int *)((int)param_1 + 0xb8) = *(int *)((int)param_1 + 0xb8) + 1;
      }
    }
  }
  return;
}

