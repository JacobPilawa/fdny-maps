/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0045c000
 * Match: function entry
 * Entry: 0045c000 FUN_0045c000
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0045c000(void *this,int *param_1)

{
  void *pvVar1;
  undefined4 *puVar2;
  float *pfVar3;
  int iVar4;
  float fVar5;
  float fVar6;
  int *piVar7;
  float *pfVar8;
  float local_30 [3];
  float local_24 [3];
  float afStack_18 [4];
  undefined1 *puStack_8;
  
  pfVar3 = local_30;
  pfVar8 = local_24;
  *(undefined4 *)((int)this + 0x120) = 6;
  pvVar1 = (void *)(**(code **)(*param_1 + 0x18))();
  pvVar1 = (void *)FUN_00446ee0(pvVar1,pfVar8);
  puVar2 = (undefined4 *)FUN_0042d2b0(pvVar1,pfVar3);
  *(undefined4 *)((int)this + 0x28) = *puVar2;
  *(undefined4 *)((int)this + 0x2c) = puVar2[1];
  *(undefined4 *)((int)this + 0x30) = puVar2[2];
  pfVar3 = local_24;
  pfVar8 = local_30;
  pvVar1 = (void *)(**(code **)(*param_1 + 0x18))();
  pvVar1 = (void *)FUN_0049e770(pvVar1,pfVar8);
  puVar2 = (undefined4 *)FUN_0042d2b0(pvVar1,pfVar3);
  *(undefined4 *)((int)this + 0x40) = *puVar2;
  *(undefined4 *)((int)this + 0x44) = puVar2[1];
  *(undefined4 *)((int)this + 0x48) = puVar2[2];
  (**(code **)(*(int *)((int)this + 0x24) + 8))();
  fVar6 = 24.0;
  fVar5 = 5.0;
  pfVar3 = (float *)FUN_0049e770((void *)((int)this + 0x24),local_30);
  pfVar3 = (float *)FUN_00446390((float *)&stack0xffffffc4,pfVar3,fVar5);
  pfVar3 = (float *)FUN_00446390(local_24,pfVar3,fVar6);
  iVar4 = (**(code **)(*param_1 + 0x18))();
  puVar2 = (undefined4 *)FUN_0043b2d0(afStack_18,(float *)(iVar4 + 0x28),pfVar3);
  *(undefined4 *)((int)this + 0x4c) = *puVar2;
  *(undefined4 *)((int)this + 0x50) = puVar2[1];
  *(undefined4 *)((int)this + 0x54) = puVar2[2];
  FUN_0049da00(this);
  FUN_00481140((void *)((int)this + 0x170),'\x01');
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar7 = &DAT_00501908;
  puStack_8 = &stack0xffffffb4;
  FUN_00401d80(&stack0xffffffb4,"rescue");
  FUN_004705b0((void *)((int)this + 0xd4),piVar7);
  *(undefined4 *)((int)this + 0x1f8) = 0x96;
  iVar4 = DAT_00502afc;
  *(int *)((int)this + 500) = DAT_00502afc + 2000;
  *(int *)((int)this + 0x1f0) = iVar4 + 0x866;
  *(undefined1 *)((int)this + 0x1fc) = 1;
  FUN_00480d40((int)this + 0x170);
  return;
}


