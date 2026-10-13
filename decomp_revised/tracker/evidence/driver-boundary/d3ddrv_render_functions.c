/* Ghidra decompilation excerpts from D3DDrv.dll.
 * SHA-256: 7727baabe0cc941cf63e4088cf140dbc9389e0d451bd1ac30a053c2e7c9d49e7
 * PE32 x86, image base 0x10000000. Ghidra generic names are preserved.
 */

/* 0x10008990, driver table +0x38 */
undefined4 __fastcall FUN_10008990(undefined4 param_1,undefined4 param_2,
                            undefined4 param_3,undefined4 *param_4)
{
  uint uVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;

  iVar2 = FUN_10008200();
  if (iVar2 == 0) {
    FUN_10006020(7,"D3DDRV:THandle_Create:  Out of texture handles.\n");
    return 0;
  }
  *(undefined4 *)(iVar2 + 0x18) = *param_4;
  *(undefined4 *)(iVar2 + 0x1c) = param_4[1];
  uVar1 = param_4[1];
  if ((uVar1 & 2) == 0) {
    if ((uVar1 & 4) == 0) {
      if ((uVar1 & 1) != 0) {
        puVar4 = &DAT_100307e4;
        puVar5 = &DAT_101242a0;
        for (iVar3 = 0x1f; iVar3 != 0; iVar3 = iVar3 + -1) {
          *puVar5 = *puVar4;
          puVar4 = puVar4 + 1;
          puVar5 = puVar5 + 1;
        }
        iVar3 = FUN_10008900(iVar2,param_1,param_2,param_3,param_4);
        if (iVar3 == 0) {
          return 0;
        }
      }
      return iVar2;
    }
    iVar3 = FUN_10008240(*param_4);
    if (iVar3 == 0) {
      return 0;
    }
    iVar3 = FUN_10008850(iVar2,param_1,param_2,param_3,param_4);
  }
  else {
    iVar3 = FUN_10008240(*param_4);
    if (iVar3 == 0) {
      return 0;
    }
    iVar3 = FUN_10008760(iVar2,param_1,param_2,param_3,param_4);
  }
  if (iVar3 == 0) {
    return 0;
  }
  DAT_1052b720 = 1;
  return iVar2;
}

/* 0x10008760 */
int FUN_10008760(int param_1,int param_2,undefined4 param_3,int param_4)
{
  byte bVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;

  *(int *)(param_1 + 4) = param_2;
  *(undefined4 *)(param_1 + 8) = param_3;
  *(char *)(param_1 + 0x10) = (char)param_4;
  bVar1 = FUN_10005990(param_2,param_3);
  *(byte *)(param_1 + 0x11) = bVar1;
  *(int *)(param_1 + 0xc) = 1 << (bVar1 & 0x1f);
  puVar2 = _malloc(param_4 * 0x14);
  *(undefined4 **)(param_1 + 0x14) = puVar2;
  for (uVar4 = param_4 * 5 & 0x3fffffff; uVar4 != 0; uVar4 = uVar4 - 1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  for (iVar5 = 0; iVar5 != 0; iVar5 = iVar5 + -1) {
    *(undefined1 *)puVar2 = 0;
    puVar2 = (undefined4 *)((int)puVar2 + 1);
  }
  iVar5 = *(int *)(param_1 + 0x14);
  iVar6 = 0;
  if (iVar5 != 0) {
    iVar7 = 1 << (*(byte *)(param_1 + 0x11) & 0x1f);
    param_2 = 0;
    if (0 < param_4) {
      do {
        iVar5 = FUN_100083e0(iVar5 + iVar6,iVar7,iVar7,&DAT_101242a0,0,0);
        if (iVar5 == 0) goto LAB_10008833;
        uVar3 = FUN_10005b70(DAT_1052b728,iVar7,iVar7,1,0,&DAT_101242a0);
        *(undefined4 *)(*(int *)(param_1 + 0x14) + 4 + iVar6) = uVar3;
        iVar5 = *(int *)(param_1 + 0x14);
        if (*(int *)(iVar5 + 4 + iVar6) == 0) goto LAB_10008833;
        iVar7 = iVar7 >> 1;
        param_2 = param_2 + 1;
        iVar6 = iVar6 + 0x14;
      } while (param_2 < param_4);
    }
    return param_1;
  }
LAB_10008833:
  FUN_10008600();
  return 0;
}

/* 0x10008850 */
int FUN_10008850(int param_1,undefined4 param_2,undefined4 param_3,int param_4)
{
  byte bVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;

  *(undefined4 *)(param_1 + 4) = param_2;
  *(undefined4 *)(param_1 + 8) = param_3;
  *(char *)(param_1 + 0x10) = (char)param_4;
  bVar1 = FUN_10005990(param_2,param_3);
  iVar6 = 1 << (bVar1 & 0x1f);
  *(byte *)(param_1 + 0x11) = bVar1;
  *(int *)(param_1 + 0xc) = iVar6;
  puVar2 = _malloc(param_4 * 0x14);
  *(undefined4 **)(param_1 + 0x14) = puVar2;
  for (uVar4 = param_4 * 5 & 0x3fffffff; uVar4 != 0; uVar4 = uVar4 - 1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  for (iVar5 = 0; iVar5 != 0; iVar5 = iVar5 + -1) {
    *(undefined1 *)puVar2 = 0;
    puVar2 = (undefined4 *)((int)puVar2 + 1);
  }
  *(undefined1 *)(*(int *)(param_1 + 0x14) + 0x10) = 2;
  uVar3 = FUN_10005b70(DAT_1052b728,iVar6,iVar6,param_4,DAT_10042184 != 0,&DAT_101242a0);
  *(undefined4 *)(*(int *)(param_1 + 0x14) + 4) = uVar3;
  if (*(int *)(*(int *)(param_1 + 0x14) + 4) == 0) {
    FUN_10008600();
    return 0;
  }
  return param_1;
}

/* 0x10008900 */
int FUN_10008900(int param_1,undefined4 param_2,undefined4 param_3,int param_4)
{
  undefined1 uVar1;
  undefined4 *puVar2;
  uint uVar3;
  int iVar4;

  *(undefined4 *)(param_1 + 4) = param_2;
  *(undefined4 *)(param_1 + 8) = param_3;
  *(char *)(param_1 + 0x10) = (char)param_4;
  uVar1 = FUN_10005990(param_2,param_3);
  *(undefined1 *)(param_1 + 0x11) = uVar1;
  *(undefined4 *)(param_1 + 0xc) = param_2;
  puVar2 = _malloc(param_4 * 0x14);
  *(undefined4 **)(param_1 + 0x14) = puVar2;
  for (uVar3 = param_4 * 5 & 0x3fffffff; uVar3 != 0; uVar3 = uVar3 - 1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  for (iVar4 = 0; iVar4 != 0; iVar4 = iVar4 + -1) {
    *(undefined1 *)puVar2 = 0;
    puVar2 = (undefined4 *)((int)puVar2 + 1);
  }
  if (*(int *)(param_1 + 0x14) != 0) {
    iVar4 = FUN_100083e0(*(int *)(param_1 + 0x14),param_2,param_3,&DAT_101242a0,1,0);
    if (iVar4 != 0) {
      return param_1;
    }
  }
  FUN_10008600();
  return 0;
}

/* 0x100082B0, driver table +0x40 */
undefined4 __fastcall FUN_100082b0(int param_1,int param_2,undefined4 *param_3)
{
  byte *pbVar1;
  int *piVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 local_88[4];
  undefined4 uStack_78;

  puVar4 = local_88;
  for (iVar3 = 0x1f; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar4 = 0;
    puVar4 = puVar4 + 1;
  }
  local_88[0] = 0x7c;
  piVar2 = *(int **)(param_2 * 0x14 + *(int *)(param_1 + 0x14));
  iVar3 = (**(code **)(*piVar2 + 100))(piVar2,0,local_88,1,0);
  if (iVar3 != 0) {
    return 0;
  }
  pbVar1 = (byte *)(param_2 * 0x14 + 0x10 + *(int *)(param_1 + 0x14));
  *pbVar1 = *pbVar1 | 1;
  *param_3 = uStack_78;
  return 1;
}

/* 0x10008330, driver table +0x44 */
undefined4 __fastcall FUN_10008330(int param_1,int param_2)
{
  byte *pbVar1;
  int *piVar2;
  int iVar3;

  param_2 = param_2 * 0x14;
  piVar2 = *(int **)(param_2 + *(int *)(param_1 + 0x14));
  iVar3 = (**(code **)(*piVar2 + 0x80))(piVar2,0);
  if (iVar3 != 0) {
    return 0;
  }
  pbVar1 = (byte *)(param_2 + 0x10 + *(int *)(param_1 + 0x14));
  *pbVar1 = *pbVar1 | 2;
  *(byte *)(param_2 + 0x10 + *(int *)(param_1 + 0x14)) =
       *(byte *)(param_2 + 0x10 + *(int *)(param_1 + 0x14)) & 0xfe;
  return 1;
}

/* 0x10008380, driver table +0x58 */
void __fastcall FUN_10008380(int param_1,byte param_2,int *param_3)
{
  *param_3 = *(int *)(param_1 + 4) >> (param_2 & 0x1f);
  param_3[1] = *(int *)(param_1 + 8) >> (param_2 & 0x1f);
  param_3[2] = *(int *)(param_1 + 0xc) >> (param_2 & 0x1f);
  if ((*(uint *)(param_1 + 0x1c) & 0x20000) != 0) {
    param_3[4] = 1;
    param_3[3] = 1;
    param_3[5] = *(int *)(param_1 + 0x18);
    param_3[6] = *(int *)(param_1 + 0x1c);
    return;
  }
  param_3[4] = 0;
  param_3[3] = 0;
  param_3[5] = *(int *)(param_1 + 0x18);
  param_3[6] = *(int *)(param_1 + 0x1c);
  return;
}

/* 0x10007F30, driver table +0x5C */
undefined4 __fastcall FUN_10007f30(undefined4 param_1,undefined4 param_2)
{
  int iVar1;
  undefined4 uVar2;

  DAT_10124214 = DAT_10124214 + 1;
  if (DAT_100307d8 == 0) {
    FUN_10002040("BeginScene:  No D3D Device!.");
    return 0;
  }
  _DAT_101149d0 = 0;
  _DAT_101149d4 = 0;
  _DAT_101149d8 = 0;
  DAT_101149dc = 0;
  DAT_101149e0 = 0;
  iVar1 = FUN_10008540();
  if (iVar1 == 0) {
    return 0;
  }
  if (DAT_10042174 != 0) {
    iVar1 = FUN_10002a20(param_1,param_2);
    if (iVar1 == 0) {
      FUN_10002040("D3DClearBuffers failed.");
      return 0;
    }
    _DAT_1001f0d4 = 0;
    iVar1 = FUN_100068c0();
    if (iVar1 != 0) {
      uVar2 = FUN_10001000(iVar1);
      FUN_10002040("BeginScene:  D3D BeginScene Failed.\n%s.",uVar2);
      return 0;
    }
    FUN_10001890(2,2);
    FUN_10001f90(3);
    FUN_10001f50(1);
    FUN_10001f60(4);
    FUN_10001980(5,6);
    FUN_100019a0(1);
    if (DAT_10042188 != 0) {
      FUN_100067e0();
      FUN_10009414();
      FUN_10009414();
      FUN_10009414();
      FUN_100067e0();
      return 1;
    }
    FUN_100067e0();
  }
  return 1;
}

/* 0x10008080, driver table +0x60 */
undefined4 FUN_10008080(void)
{
  int iVar1;
  undefined4 uVar2;

  if (DAT_100307d8 != 0) {
    if (DAT_10042174 == 0) {
      return 1;
    }
    iVar1 = FUN_100068e0();
    if (iVar1 == 0) {
      FUN_10006b00();
      iVar1 = FUN_10004400();
      if (iVar1 != 0) {
        return 1;
      }
    }
    else {
      uVar2 = FUN_10001000(iVar1);
      FUN_10002040("EndScene:  D3D EndScene Failed.\n%s",uVar2);
    }
  }
  return 0;
}


/* 0x10007910, driver table +0x84 */
undefined4 __fastcall FUN_10007910(int param_1,int param_2,int param_3,uint param_4)
{
  int iVar1;
  float fVar2;
  float fVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  float *pfVar9;
  float *pfVar10;

  if (DAT_10042174 == 0) {
    return 1;
  }
  if ((param_2 < 0x201) && (iVar4 = FUN_10008540(), iVar4 != 0)) {
    FUN_100076b0();
    FUN_100069a0(0);
    FUN_100069a0(2);
    FUN_100069a0(0);
    FUN_100069a0(4);
    FUN_100069a0(2);
    FUN_100069a0(0);
    FUN_100069a0(4);
    FUN_100069a0(1);
    FUN_100069a0(1);
    FUN_10001880(1,0);
    iVar4 = *(int *)(param_3 + 8);
    iVar1 = *(int *)(param_3 + 4);
    FUN_10006b20((float)iVar1,(float)iVar4);
    fVar3 = _DAT_100186a8 / (float)(1 << (*(byte *)(param_3 + 0x11) & 0x1f));
    if (0 < param_2) {
      pfVar9 = (float *)(param_1 + 0x10);
      pfVar10 = (float *)&DAT_101189f0;
      param_3 = param_2;
      do {
        fVar2 = _DAT_100186a8 / pfVar9[-2];
        pfVar10[-2] = pfVar9[-4];
        pfVar10[-1] = pfVar9[-3];
        *pfVar10 = _DAT_100186a8 - fVar2;
        pfVar10[1] = fVar2;
        fVar2 = *pfVar9;
        pfVar10[4] = (float)iVar1 * fVar3 * pfVar9[-1];
        pfVar10[5] = (float)iVar4 * fVar3 * fVar2;
        if ((param_4 & 4) == 0) {
          uVar6 = FUN_10009414();
          uVar6 = uVar6 | 0xffffff00;
        }
        else {
          iVar5 = FUN_10009414();
          uVar6 = FUN_10009414();
          uVar6 = iVar5 << 8 | uVar6;
        }
        uVar7 = FUN_10009414();
        uVar8 = FUN_10009414();
        pfVar10[2] = (float)((uVar6 << 8 | uVar7) << 8 | uVar8);
        if (((param_4 & 0x80) == 0) && ((param_4 & 0x200) == 0)) {
          if (DAT_10042188 == 0) {
            pfVar10[3] = 0.0;
          }
          else {
            uVar6 = FUN_10009414();
            if (0xff < uVar6) {
              uVar6 = 0xff;
            }
            pfVar10[3] = (float)(uVar6 << 0x18);
          }
        }
        else {
          pfVar10[3] = -NAN;
        }
        pfVar9 = pfVar9 + 9;
        pfVar10 = pfVar10 + 10;
        param_3 = param_3 + -1;
      } while (param_3 != 0);
    }
    iVar4 = FUN_10006df0(0);
    if (iVar4 != 0) {
      FUN_10001f30(&DAT_101189e8,param_2);
      return 1;
    }
  }
  return 0;
}
