/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044b270
 * Match: function entry
 * Entry: 0044b270 FUN_0044b270
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0044b270(void *this,void *param_1,char param_2)

{
  int iVar1;
  void *pvVar2;
  undefined4 *puVar3;
  int iVar4;
  int *piVar5;
  float10 fVar6;
  ulonglong uVar7;
  undefined4 local_18 [3];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cfaa1;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0044ad70((int)this);
  *(undefined1 *)((int)this + 0x9c) = 1;
  *(undefined4 *)((int)this + 0x88) = 0;
  *(undefined4 *)((int)this + 0x8c) = 0;
  FUN_0042d280((void *)((int)this + 0x90),local_18,0x3f800000,0x3f800000,0x3f800000);
  if ((param_2 != '\0') && (*(int *)((int)this + 0x58) == 0)) {
    pvVar2 = operator_new(0xcc);
    local_4 = 0;
    if (pvVar2 == (void *)0x0) {
      puVar3 = (undefined4 *)0x0;
    }
    else {
      puVar3 = FUN_00458e40(pvVar2,param_1);
    }
    local_4 = 0xffffffff;
    *(undefined4 **)((int)this + 0x58) = puVar3;
    fVar6 = FUN_00498b30(0.8,1.2);
    *(float *)((int)this + 0x54) = (float)(fVar6 * (float10)_DAT_004e1810);
  }
  if (*(int *)((int)this + 0x5c) != 0) {
    *(undefined1 *)(*(int *)((int)this + 0x60) + 0x30) = 1;
    *(undefined1 *)(*(int *)((int)this + 100) + 0x30) = 1;
    *(undefined1 *)(*(int *)((int)this + 0x68) + 0x30) = 1;
    *(undefined1 *)(*(int *)((int)this + 0x6c) + 0x30) = 1;
    ExceptionList = local_c;
    return;
  }
  *(void **)((int)this + 0x5c) = param_1;
  iVar4 = 3;
  piVar5 = (int *)((int)this + 0x60);
  do {
    pvVar2 = operator_new(0x168);
    local_4 = 1;
    if (pvVar2 == (void *)0x0) {
      puVar3 = (undefined4 *)0x0;
    }
    else {
      puVar3 = FUN_0047f850(pvVar2,0x502d08,0);
    }
    *piVar5 = (int)puVar3;
    *(undefined1 *)((int)puVar3 + 0xad) = 1;
    *(undefined4 *)(*piVar5 + 0xb0) = 4;
    iVar1 = *piVar5;
    local_4 = 0xffffffff;
    fVar6 = FUN_00498b30(0.5,1.5);
    *(float *)(iVar1 + 0x1c) = (float)(fVar6 * (float10)*(float *)(iVar1 + 0x1c));
    FUN_00498b30(0.0,(float)(DAT_00502d0c + -1));
    uVar7 = FUN_004b9400();
    FUN_0047d300((void *)*piVar5,(int)uVar7);
    *(undefined4 *)(*piVar5 + 0xa4) = 0xa0;
    iVar4 = iVar4 + -1;
    *(float *)(*piVar5 + 0xe8) = *(float *)(*piVar5 + 0xe8) + (float)_DAT_004dfc78;
    *(float *)(*piVar5 + 0x10c) = *(float *)(*piVar5 + 0x10c) + (float)_DAT_004dfc78;
    piVar5 = piVar5 + 1;
  } while (iVar4 != 0);
  pvVar2 = operator_new(0x168);
  local_4 = 2;
  if (pvVar2 == (void *)0x0) {
    puVar3 = (undefined4 *)0x0;
  }
  else {
    puVar3 = FUN_0047f850(pvVar2,0x502cd0,0);
  }
  *(undefined4 **)((int)this + 0x6c) = puVar3;
  *(undefined1 *)((int)puVar3 + 0xad) = 1;
  *(undefined4 *)(*(int *)((int)this + 0x6c) + 0xb0) = 4;
  iVar4 = *(int *)((int)this + 0x6c);
  local_4 = 0xffffffff;
  fVar6 = FUN_00498b30(0.5,1.5);
  *(float *)(iVar4 + 0x1c) = (float)(fVar6 * (float10)*(float *)(iVar4 + 0x1c));
  FUN_00498b30(0.0,(float)(DAT_00502d0c + -1));
  uVar7 = FUN_004b9400();
  FUN_0047d300(*(void **)((int)this + 0x6c),(int)uVar7);
  *(undefined4 *)(*(int *)((int)this + 0x6c) + 0xa4) = 0xa0;
  *(float *)(*(int *)((int)this + 0x6c) + 0xe8) =
       *(float *)(*(int *)((int)this + 0x6c) + 0xe8) + (float)_DAT_004dfc78;
  *(float *)(*(int *)((int)this + 0x6c) + 0x10c) =
       *(float *)(*(int *)((int)this + 0x6c) + 0x10c) + (float)_DAT_004dfc78;
  ExceptionList = local_c;
  return;
}


