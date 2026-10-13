/* Ghidra decompilation; names/types are provisional.
 * Entry: 00463ef0
 * Function: FUN_00463ef0
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00463ef0(int param_1)

{
  float fVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  void *pvVar4;
  float10 fVar5;
  ulonglong uVar6;
  undefined4 uVar7;
  float fVar8;
  undefined4 uVar9;
  float fVar10;
  undefined4 uVar11;
  float fVar12;
  int local_38;
  undefined4 *local_34;
  undefined4 local_30 [3];
  undefined4 local_24 [3];
  undefined4 local_18 [3];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1918;
  local_c = ExceptionList;
  if (*(char *)(param_1 + 0x624) != '\0') {
    fVar1 = *(float *)(param_1 + 0x620);
    ExceptionList = &local_c;
    FUN_00481a50((undefined4 *)(param_1 + 0x404));
    FUN_00481c00(&local_38);
    local_4 = 0;
    puVar2 = local_34;
    while( true ) {
      if ((local_38 == 0) || (puVar2 == (undefined4 *)0x0)) {
        pvVar4 = (void *)0x0;
      }
      else {
        pvVar4 = (void *)*puVar2;
      }
      if ((local_38 != 0) && (puVar2 != (undefined4 *)0x0)) {
        puVar2 = (undefined4 *)puVar2[1];
        local_34 = puVar2;
      }
      if (pvVar4 == (void *)0x0) break;
      if (*(int *)((int)pvVar4 + 0x18) == 6) {
        fVar5 = FUN_0044a7a0((int)pvVar4 + 300,(float *)(param_1 + 0x614),fVar1);
        puVar2 = local_34;
        if ((((float10)_DAT_004dfa28 <= fVar5) &&
            (FUN_00448970(pvVar4), puVar2 = local_34,
            (float)_DAT_004dfa10 <= *(float *)((int)pvVar4 + 0x128))) &&
           (*(uint *)(param_1 + 0x3b0) <= DAT_00502afc)) {
          FUN_00498b30(10.0,250.0);
          uVar6 = FUN_004b9400();
          *(int *)(param_1 + 0x3b0) = (int)uVar6;
          puVar2 = (undefined4 *)FUN_00447870((void *)((int)pvVar4 + 300),local_30);
          puVar3 = FUN_0047ff30((void *)(param_1 + 0x3d0),puVar2);
          puVar2 = local_34;
          if (puVar3 != (undefined4 *)0x0) {
            uVar11 = 0;
            uVar9 = 0;
            uVar7 = 0;
            puVar2 = local_24;
            pvVar4 = (void *)FUN_0047f1c0((int)puVar3);
            FUN_0042d280(pvVar4,puVar2,uVar7,uVar9,uVar11);
            fVar5 = FUN_00498b30(-50.0,50.0);
            fVar12 = (float)fVar5;
            fVar5 = FUN_00498b30(100.0,400.0);
            fVar10 = (float)fVar5;
            fVar5 = FUN_00498b30(-50.0,50.0);
            fVar8 = (float)fVar5;
            puVar2 = local_18;
            pvVar4 = (void *)FUN_0047f1b0((int)puVar3);
            FUN_0042d280(pvVar4,puVar2,fVar8,fVar10,fVar12);
            puVar3[0x29] = 0x20;
            puVar2 = local_34;
          }
        }
      }
    }
    local_4 = 0xffffffff;
    if (local_38 != 0) {
      FUN_00429fe0((void *)(local_38 + 0x28),(int)&local_38);
    }
  }
  ExceptionList = local_c;
  return;
}

