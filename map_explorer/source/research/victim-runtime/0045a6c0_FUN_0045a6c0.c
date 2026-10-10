/* Ghidra decompilation; names/types are provisional.
 * Entry: 0045a6c0
 * Function: FUN_0045a6c0
 */


void __fastcall FUN_0045a6c0(int *param_1)

{
  int *piVar1;
  int *piVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 *puVar5;
  void *pvVar6;
  int *piVar7;
  int iVar8;
  int iVar9;
  int local_10;
  int local_c;
  
  piVar2 = (int *)*param_1;
  *piVar2 = *piVar2 + -1;
  *param_1 = 0;
  puVar5 = _malloc(0x24);
  *param_1 = (int)puVar5;
  for (iVar8 = 9; iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined4 *)*param_1 = 1;
  *(int *)(*param_1 + 0xc) = piVar2[3];
  if (piVar2[4] != 0) {
    puVar5 = operator_new(0x10);
    if (puVar5 == (undefined4 *)0x0) {
      puVar5 = (undefined4 *)0x0;
    }
    else {
      puVar3 = (undefined4 *)piVar2[4];
      *puVar5 = *puVar3;
      puVar5[1] = puVar3[1];
      puVar5[2] = puVar3[2];
      puVar5[3] = puVar3[3];
    }
    *(undefined4 **)(*param_1 + 0x10) = puVar5;
  }
  iVar8 = piVar2[1];
  iVar9 = iVar8;
  if ((0 < iVar8) && (iVar4 = *(int *)(*param_1 + 0xc), iVar4 != 1)) {
    iVar9 = iVar8 + -1 + (iVar4 - (iVar8 + -1) % iVar4);
  }
  if (iVar9 != *(int *)(*param_1 + 8)) {
    pvVar6 = _realloc((void *)*param_1,iVar9 * 0x10 + 0x24);
    *param_1 = (int)pvVar6;
    *(int *)((int)pvVar6 + 8) = iVar9;
  }
  iVar9 = 0;
  local_10 = 0;
  if (3 < iVar8) {
    iVar4 = -(int)piVar2;
    local_c = 3;
    piVar7 = piVar2 + 0xc;
    do {
      piVar1 = (int *)((int)piVar7 + *param_1 + iVar4 + -0x1c);
      if (piVar1 != (int *)0x0) {
        *piVar1 = piVar7[-7];
        piVar1[1] = piVar7[-6];
        piVar1[2] = piVar7[-5];
        piVar1[3] = piVar7[-4];
      }
      piVar1 = (int *)((int)piVar7 + *param_1 + iVar4 + -0xc);
      if (piVar1 != (int *)0x0) {
        *piVar1 = piVar7[-3];
        piVar1[1] = piVar7[-2];
        piVar1[2] = piVar7[-1];
        piVar1[3] = *piVar7;
      }
      piVar1 = (int *)((int)piVar7 + *param_1 + iVar4 + 4);
      if (piVar1 != (int *)0x0) {
        *piVar1 = piVar7[1];
        piVar1[1] = piVar7[2];
        piVar1[2] = piVar7[3];
        piVar1[3] = piVar7[4];
      }
      piVar1 = (int *)((int)piVar7 + *param_1 + iVar4 + 0x14);
      if (piVar1 != (int *)0x0) {
        *piVar1 = piVar7[5];
        piVar1[1] = piVar7[6];
        piVar1[2] = piVar7[7];
        piVar1[3] = piVar7[8];
      }
      local_c = local_c + 4;
      iVar9 = local_10 + 4;
      piVar7 = piVar7 + 0x10;
      local_10 = iVar9;
    } while (local_c < iVar8);
  }
  if (iVar8 <= iVar9) {
    *(int *)(*param_1 + 4) = iVar8;
    return;
  }
  piVar7 = piVar2 + iVar9 * 4 + 5;
  iVar9 = iVar8 - iVar9;
  do {
    piVar1 = (int *)((int)piVar7 + (*param_1 - (int)piVar2));
    if (piVar1 != (int *)0x0) {
      *piVar1 = *piVar7;
      piVar1[1] = piVar7[1];
      piVar1[2] = piVar7[2];
      piVar1[3] = piVar7[3];
    }
    piVar7 = piVar7 + 4;
    iVar9 = iVar9 + -1;
  } while (iVar9 != 0);
  *(int *)(*param_1 + 4) = iVar8;
  return;
}

