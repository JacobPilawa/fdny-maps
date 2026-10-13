
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __thiscall FUN_0042f280(void *this,int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  bool bVar3;
  void *this_00;
  undefined4 uVar4;
  int *extraout_ECX;
  undefined4 *unaff_EDI;
  int *piVar5;
  undefined4 *puVar6;
  int *piVar7;
  undefined4 uVar8;
  int *piStack_10;
  undefined1 *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cde10;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  this_00 = (void *)FUN_004225a0((void *)(DAT_00502aac + 0x6ac),(int)param_1);
  if (this_00 == (void *)0x0) {
    uVar4 = 0;
  }
  else {
    puVar6 = (undefined4 *)((int)this + 600);
    *(undefined4 *)((int)this + 0x2c) = *(undefined4 *)((int)this_00 + 0x18);
    if (*(int *)((int)this + 600) != 0) {
      geEngine_RemoveBitmap();
      geBitmap_Destroy(puVar6);
      *puVar6 = 0;
    }
    FUN_00422ef0(this_00,&param_1,unaff_EDI);
    local_4 = 0;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar7 = &DAT_00501908;
    if (param_1 != &DAT_00501908) {
      piVar7 = param_1;
      if (param_1 != &DAT_00501908) {
        *param_1 = *param_1 + 1;
      }
    }
    bVar3 = FUN_004097f0(piVar7);
    if (bVar3) {
      uVar4 = FUN_004928b0(param_2 + 3);
      uVar4 = geBitmap_CreateFromFile(uVar4);
      *puVar6 = uVar4;
      geEngine_AddBitmap();
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piStack_10 = &DAT_00501908;
    FUN_00401d80(&piStack_10,"^(255,192,0)");
    FUN_00401d00(&piStack_10,(char *)(*(int *)((int)this_00 + 4) + 0xc));
    piVar7 = piStack_10;
    local_c = &stack0xffffffd0;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar5 = &DAT_00501908;
    if (piStack_10 != &DAT_00501908) {
      *piStack_10 = *piStack_10 + 1;
      piVar5 = piStack_10;
    }
    FUN_0048c780((void *)((int)this + 0x50),piVar5,(undefined4 *)&DAT_004ff590,2,0);
    uVar8 = 0;
    uVar4 = 0;
    puVar6 = (undefined4 *)&DAT_004ff5a8;
    local_c = &stack0xffffffd0;
    piVar5 = extraout_ECX;
    FUN_004228f0(this_00,(undefined4 *)&stack0xffffffd0);
    FUN_0048c780((void *)((int)this + 0x154),piVar5,puVar6,uVar4,uVar8);
    iVar1 = *(int *)((int)this_00 + 0x20) + -0xc + DAT_00502b2c;
    iVar2 = *(int *)((int)this_00 + 0x24) + -0xc + DAT_00502b30;
    FUN_0047ea10((void *)((int)this + 0x260),iVar1,iVar2,iVar1 + 0x18,iVar2 + 0x18,0);
    if ((piVar7 != &DAT_00501908) && (iVar1 = *piVar7, *piVar7 = iVar1 + -1, iVar1 + -1 < 1)) {
      FUN_004011c0(&piStack_10);
    }
    if (param_2 != &DAT_00501908) {
      *param_2 = *param_2 + -1;
      if (*param_2 < 1) {
        param_2 = (int *)FUN_004011c0(&param_2);
      }
    }
    uVar4 = CONCAT31((int3)((uint)param_2 >> 8),1);
  }
  ExceptionList = puStack_8;
  return uVar4;
}
