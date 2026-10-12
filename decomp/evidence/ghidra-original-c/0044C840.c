/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044c840
 * Match: function entry
 * Entry: 0044c840 FUN_0044c840
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __thiscall FUN_0044c840(void *this,int *param_1,int param_2)

{
  void *this_00;
  char cVar1;
  void *pvVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 *this_01;
  int *piVar5;
  int *piVar6;
  float *pfVar7;
  int local_80;
  char *local_7c;
  char *local_78;
  undefined4 local_74;
  int local_68;
  char *local_64;
  char *local_60;
  undefined4 local_5c;
  float local_50 [3];
  undefined1 local_44 [68];
  
  piVar6 = (int *)((int)this + 0x3564);
  if (*(int *)((int)this + 0x3568) < 5) {
    piVar5 = &local_68;
    for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
      *piVar5 = 0;
      piVar5 = piVar5 + 1;
    }
    local_68 = 1;
    local_64 = "Array index out of bounds";
    local_60 = "..\\BluelAPI/Container/SArray.h";
    local_5c = 0x1cd;
    FUN_00422060(&local_68);
  }
  this_00 = *(void **)(*piVar6 + 0x10);
  if (param_2 == 0x3ea) {
    FUN_00463370(this_00,2);
    FUN_0044c360(this);
  }
  else {
    if (*(int *)((int)this + 0x3568) < 5) {
      piVar5 = &local_80;
      for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
        *piVar5 = 0;
        piVar5 = piVar5 + 1;
      }
      local_80 = 1;
      local_7c = "Array index out of bounds";
      local_78 = "..\\BluelAPI/Container/SArray.h";
      local_74 = 0x1cd;
      FUN_00422060(&local_80);
    }
    if (*(int *)(*piVar6 + 0x10) == 0) {
LAB_0044ca35:
      FUN_004a11a0((void *)((int)this + 0x508),(undefined1 *)0x0);
      return;
    }
    iVar4 = *(int *)((int)this + 0x357c);
    if (iVar4 != 4) {
      if (iVar4 == 0) {
        *(undefined4 *)((int)this + 0x357c) = 4;
        FUN_0048d490(piVar6,4);
        (**(code **)(**(int **)(*piVar6 + 0x10) + 0x3c))(1);
      }
      else {
        puVar3 = (undefined4 *)FUN_0040cdf0(piVar6,iVar4);
        cVar1 = (**(code **)(*(int *)*puVar3 + 0x38))();
        if (cVar1 == '\0') goto LAB_0044ca35;
        puVar3 = (undefined4 *)FUN_0040cdf0(piVar6,*(int *)((int)this + 0x357c));
        (**(code **)(*(int *)*puVar3 + 0x3c))(0);
        *(undefined4 *)((int)this + 0x357c) = 4;
        puVar3 = (undefined4 *)FUN_0040cdf0(piVar6,4);
        (**(code **)(*(int *)*puVar3 + 0x3c))(1);
      }
      FUN_004571a0((void *)((int)this + 0x5f4),(int *)0x5,(undefined1 *)0x0);
    }
    pfVar7 = local_50;
    piVar6 = &local_80;
    pvVar2 = FUN_004977b0(local_44,param_1[0x34]);
    pvVar2 = (void *)FUN_00446ee0(pvVar2,piVar6);
    puVar3 = (undefined4 *)FUN_0042d2b0(pvVar2,pfVar7);
    iVar4 = (**(code **)(*param_1 + 0x18))();
    this_01 = (undefined4 *)((int)this_00 + 0xc70);
    FUN_00485a80(this_01,(undefined4 *)(iVar4 + 0x28));
    FUN_00485aa0(this_01,puVar3);
    *(int **)((int)this_00 + 0xc6c) = param_1;
    FUN_00463370(this_00,3);
    iVar4 = **(int **)((int)this_00 + 0xcb8);
    *(int *)((int)this_00 + 0xcd0) = iVar4;
    *(undefined4 *)(iVar4 + 8) = *this_01;
    *(undefined4 *)(iVar4 + 0xc) = *(undefined4 *)((int)this_00 + 0xc74);
    *(undefined4 *)(iVar4 + 0x10) = *(undefined4 *)((int)this_00 + 0xc78);
  }
  FUN_004a11a0((void *)((int)this + 0x4c0),(undefined1 *)0x0);
  return;
}


