/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0045c1a0
 * Match: function entry
 * Entry: 0045c1a0 FUN_0045c1a0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0045c1a0(void *this,int *param_1)

{
  void *this_00;
  uint uVar1;
  void *pvVar2;
  undefined4 *puVar3;
  int iVar4;
  int *piVar5;
  undefined4 uVar6;
  float *pfVar7;
  float *pfVar8;
  float local_18 [3];
  float local_c;
  undefined1 *puStack_8;
  
  *(undefined4 *)((int)this + 0x120) = 4;
  this_00 = (void *)((int)this + 0x170);
  if (*(uint *)((int)this + 0x180) == 6) {
    uVar6 = 0x3f800000;
    uVar1 = FUN_00404b30(3);
    FUN_00480ea0(this_00,uVar1,uVar6);
  }
  else {
    uVar6 = 0x3e4ccccd;
    uVar1 = FUN_00404b30(*(uint *)((int)this + 0x180));
    FUN_00480ea0(this_00,uVar1,uVar6);
    *(undefined4 *)((int)this + 0x120) = 4;
  }
  pfVar8 = local_18;
  pfVar7 = &local_c;
  pvVar2 = (void *)(**(code **)(*param_1 + 0x18))();
  pvVar2 = (void *)FUN_00446ee0(pvVar2,pfVar7);
  puVar3 = (undefined4 *)FUN_0042d2b0(pvVar2,pfVar8);
  *(undefined4 *)((int)this + 0x28) = *puVar3;
  *(undefined4 *)((int)this + 0x2c) = puVar3[1];
  *(undefined4 *)((int)this + 0x30) = puVar3[2];
  pfVar8 = &local_c;
  pfVar7 = local_18;
  pvVar2 = (void *)(**(code **)(*param_1 + 0x18))();
  pvVar2 = (void *)FUN_0049e770(pvVar2,pfVar7);
  puVar3 = (undefined4 *)FUN_0042d2b0(pvVar2,pfVar8);
  *(undefined4 *)((int)this + 0x40) = *puVar3;
  *(undefined4 *)((int)this + 0x44) = puVar3[1];
  *(undefined4 *)((int)this + 0x48) = puVar3[2];
  (**(code **)(*(int *)((int)this + 0x24) + 8))();
  iVar4 = (**(code **)(*param_1 + 0x18))();
  *(undefined4 *)((int)this + 0x4c) = *(undefined4 *)(iVar4 + 0x28);
  *(undefined4 *)((int)this + 0x50) = *(undefined4 *)(iVar4 + 0x2c);
  *(undefined4 *)((int)this + 0x54) = *(undefined4 *)(iVar4 + 0x30);
  FUN_0049da00(this);
  FUN_00481140(this_00,'\x01');
  puStack_8 = &stack0xffffffcc;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar5 = &DAT_00501908;
  FUN_00401d80(&stack0xffffffcc,"passout");
  FUN_004705b0((void *)((int)this + 0xd4),piVar5);
  return;
}


