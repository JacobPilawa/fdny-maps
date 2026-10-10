/* Ghidra decompilation; names/types are provisional.
 * Entry: 0043d6a0
 * Function: FUN_0043d6a0
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint __fastcall FUN_0043d6a0(int param_1)

{
  undefined4 uVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  int *extraout_ECX;
  int iVar5;
  int *piVar6;
  float10 fVar7;
  float fStack_128;
  float fStack_124;
  float fStack_120;
  float fStack_11c;
  float fStack_118;
  float fStack_114;
  float fStack_110;
  float fStack_10c;
  float fStack_108;
  undefined1 *local_104;
  undefined1 auStack_100 [4];
  float fStack_fc;
  float fStack_f8;
  float fStack_f4;
  float fStack_f0;
  undefined4 uStack_ec;
  undefined4 uStack_e8;
  float afStack_e4 [3];
  int iStack_d8;
  char *pcStack_d4;
  char *pcStack_d0;
  undefined4 uStack_cc;
  int iStack_c0;
  char *pcStack_bc;
  char *pcStack_b8;
  undefined4 uStack_b4;
  undefined1 auStack_a8 [12];
  undefined1 auStack_9c [12];
  float afStack_90 [9];
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined1 auStack_60 [48];
  undefined1 auStack_30 [48];
  
  if (*(int *)(param_1 + 0x70) == 4) {
    FUN_0046e450(param_1 + 0x24);
    local_104 = &stack0xfffffec4;
    piVar6 = extraout_ECX;
    FUN_00402790(&stack0xfffffec4,"openaway");
    uVar2 = FUN_0046e9d0((void *)(param_1 + 0x24),piVar6);
    return uVar2 & 0xffffff00;
  }
  iVar3 = (**(code **)(**(int **)(DAT_00503a78 + 0x30) + 0x18))();
  fStack_f0 = *(float *)(iVar3 + 0x28);
  uStack_ec = *(undefined4 *)(iVar3 + 0x2c);
  uStack_e8 = *(undefined4 *)(iVar3 + 0x30);
  iVar3 = *(int *)(param_1 + 0x1c4);
  iVar5 = 0;
  if (0 < iVar3) {
    do {
      if ((iVar5 < 0) || (*(int *)(param_1 + 0x1c4) <= iVar5)) {
        piVar6 = &iStack_d8;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar6 = 0;
          piVar6 = piVar6 + 1;
        }
        iStack_d8 = 1;
        pcStack_d4 = "Array index out of bounds";
        pcStack_d0 = "..\\BluelAPI/Container/SArray.h";
        uStack_cc = 0x1cd;
        FUN_00422060(&iStack_d8);
      }
      uVar1 = *(undefined4 *)(*(int *)(param_1 + 0x1c0) + iVar5 * 4);
      if ((iVar5 < 0) || (*(int *)(param_1 + 0x1c4) <= iVar5)) {
        piVar6 = &iStack_c0;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar6 = 0;
          piVar6 = piVar6 + 1;
        }
        iStack_c0 = 1;
        pcStack_bc = "Array index out of bounds";
        pcStack_b8 = "..\\BluelAPI/Container/SArray.h";
        uStack_b4 = 0x1cd;
        FUN_00422060(&iStack_c0);
      }
      iVar3 = geWorld_ModelGetMotion();
      if (iVar3 != 0) {
        uVar4 = geMotion_GetPath(iVar3,0);
        iVar3 = gePath_GetKeyframeCount(uVar4,1);
        if (iVar3 == 2) {
          iVar3 = *(int *)(DAT_00503a78 + 4);
          if (*(char *)(iVar3 + 0x160) == '\0') {
            iVar3 = iVar3 + 8;
          }
          else {
            iVar3 = iVar3 + 0xb4;
          }
          geWorld_GetModelRotationalCenter(*(undefined4 *)(iVar3 + 0x78),uVar1);
          gePath_GetKeyframe(uVar4,0,1,auStack_100);
          gePath_GetKeyframe(uVar4,1,1,auStack_100);
          geXForm3d_GetTranspose(auStack_30,auStack_30);
          geXForm3d_Multiply(auStack_30,auStack_60);
          fStack_108 = *(float *)(param_1 + 0x248);
          fStack_110 = *(float *)(param_1 + 0x240);
          fStack_11c = *(float *)(param_1 + 0x24c);
          fStack_114 = *(float *)(param_1 + 0x254);
          uStack_6c = 0;
          uStack_68 = 0;
          uStack_64 = 0;
          fStack_10c = 0.0;
          fStack_118 = 0.0;
          geVec3d_Subtract(&fStack_110,afStack_e4);
          fVar7 = (float10)geVec3d_Length(auStack_a8);
          local_104 = (undefined1 *)(float)fVar7;
          geVec3d_Subtract(&fStack_11c,afStack_e4);
          fVar7 = (float10)geVec3d_Length(auStack_9c);
          if ((float10)(float)local_104 <= fVar7) {
            fStack_128 = fStack_11c - fStack_110;
            fStack_124 = fStack_118 - fStack_10c;
            fStack_120 = fStack_114 - fStack_108;
          }
          else {
            fStack_128 = fStack_110 - fStack_11c;
            fStack_124 = fStack_10c - fStack_118;
            fStack_120 = fStack_108 - fStack_114;
          }
          fVar7 = (float10)geVec3d_DotProduct(&fStack_128,&fStack_128);
          if (SQRT(fVar7) != (float10)_DAT_004dfc28) {
            fVar7 = (float10)_DAT_004dfc24 / SQRT(fVar7);
            fStack_128 = (float)((float10)fStack_128 * fVar7);
            fStack_124 = (float)((float10)fStack_124 * fVar7);
            fStack_120 = (float)((float10)fStack_120 * fVar7);
          }
          FUN_0043b3b0(afStack_90,&fStack_128,&fStack_128);
          FUN_0043b2d0(&fStack_fc,&fStack_f0,afStack_e4);
          if (_DAT_004dc50c <=
              fStack_fc * fStack_128 + fStack_f8 * fStack_124 + fStack_f4 * fStack_120) {
            geXForm3d_GetTranspose(auStack_60,auStack_60);
            gePath_ModifyKeyframe(uVar4,1,1);
          }
        }
      }
      iVar3 = *(int *)(param_1 + 0x1c4);
      iVar5 = iVar5 + 1;
    } while (iVar5 < iVar3);
  }
  *(undefined4 *)(param_1 + 0x70) = 3;
  return CONCAT31((int3)((uint)iVar3 >> 8),1);
}

