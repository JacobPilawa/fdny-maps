
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm:
   Offer the command to the common ScriptActor handler at 0x0047A1F0. If unhandled, parse the
   command token and handle BModel commands including loop, open, openaway, opentowards, close,
   settime, settimescale, sethitpoints, addtimecmd, and removealltimecmds. The open branch iterates
   the command-associated entries; it queues an "open" action through 0x0046E450 and 0x0046E9D0 when
   the BModel state at root+0x70 equals 4, otherwise it writes state 3 at root+0x70.
   
   Parameters:
   this is the BModel ScriptActor subobject at BModel+0x24; param_1 is the parsed command object.
   
   Returns:
   Returns nonzero when common or BModel-specific command handling succeeds, and zero when neither
   layer handles the command.
   
   Evidence:
   BModelEntityConstructor (0x0043F340) installs the ScriptActor vtable at 0x004E0DF0; slot +4 at
   0x004E0DF4 points here. Bank's named BModel vault receives tell vault open. The static animation
   route is established by UpdateBModelEntity at 0x0043DC60; its callback cadence remains unknown.
   Subway A installs open/close as self-commands and has no tell subdoor open. See
   tracker/evidence/scenario-loading/bank_vault_open_dispatch.md and
   named_script_actor_dispatch.md. EXE SHA-256
   4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

undefined4 __thiscall DispatchBModelScriptCommand(void *this,int *param_1)

{
  int *this_00;
  char cVar1;
  bool bVar2;
  int iVar3;
  undefined4 *puVar4;
  void *pvVar5;
  undefined4 uVar6;
  uint3 extraout_var;
  int *extraout_ECX;
  int **ppiVar7;
  int iVar8;
  int *piVar9;
  char *pcVar10;
  int *piStack_18;
  undefined4 uStack_14;
  undefined1 *puStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  this_00 = param_1;
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004ceb38;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  iVar3 = FUN_0047a1f0(this,param_1);
  if ((char)iVar3 == '\0') {
    puVar4 = (undefined4 *)FUN_0040c330(this_00,&param_1);
    iVar3 = __stricmp((char *)(*(int *)*puVar4 + 0xc),"loop");
    cVar1 = '\x01' - (iVar3 != 0);
    param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
    if (cVar1 != '\0') {
      iVar3 = 0;
      if (0 < this_00[7]) {
        do {
          *(undefined4 *)((int)this + 0x4c) = 5;
          *(undefined4 *)((int)this + 0x1b4) = *(undefined4 *)((int)this + 0x1c4);
          iVar3 = iVar3 + 1;
        } while (iVar3 < this_00[7]);
      }
      ExceptionList = pvStack_c;
      return CONCAT31((int3)((uint)iVar3 >> 8),1);
    }
    puVar4 = (undefined4 *)FUN_0040c330(this_00,&param_1);
    iVar3 = __stricmp((char *)(*(int *)*puVar4 + 0xc),"open");
    cVar1 = '\x01' - (iVar3 != 0);
    param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
    if (cVar1 != '\0') {
      iVar3 = this_00[7];
      iVar8 = 0;
      if (0 < iVar3) {
        do {
          if (*(int *)((int)this + 0x4c) == 4) {
            FUN_0046e450((int)this);
            param_1 = (int *)&stack0xffffffd0;
            piVar9 = extraout_ECX;
            FUN_00402790(&stack0xffffffd0,"open");
            FUN_0046e9d0(this,piVar9);
          }
          else {
            *(undefined4 *)((int)this + 0x4c) = 3;
          }
          iVar3 = this_00[7];
          iVar8 = iVar8 + 1;
        } while (iVar8 < iVar3);
      }
      ExceptionList = pvStack_c;
      return CONCAT31((int3)((uint)iVar3 >> 8),1);
    }
    puVar4 = (undefined4 *)FUN_0040c330(this_00,&param_1);
    iVar3 = __stricmp((char *)(*(int *)*puVar4 + 0xc),"openaway");
    cVar1 = '\x01' - (iVar3 != 0);
    param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
    if (cVar1 != '\0') {
      iVar3 = this_00[7];
      iVar8 = 0;
      if (0 < iVar3) {
        do {
          FUN_0043d6a0((int)this + -0x24);
          iVar3 = this_00[7];
          iVar8 = iVar8 + 1;
        } while (iVar8 < iVar3);
      }
      ExceptionList = pvStack_c;
      return CONCAT31((int3)((uint)iVar3 >> 8),1);
    }
    puVar4 = (undefined4 *)FUN_0040c330(this_00,&param_1);
    iVar3 = __stricmp((char *)(*(int *)*puVar4 + 0xc),"opentowards");
    cVar1 = '\x01' - (iVar3 != 0);
    param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
    if (cVar1 != '\0') {
      iVar3 = this_00[7];
      iVar8 = 0;
      if (0 < iVar3) {
        do {
          FUN_0043d6a0((int)this + -0x24);
          iVar3 = this_00[7];
          iVar8 = iVar8 + 1;
        } while (iVar8 < iVar3);
      }
      ExceptionList = pvStack_c;
      return CONCAT31((int3)((uint)iVar3 >> 8),1);
    }
    pcVar10 = "close";
    pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
    bVar2 = FUN_0040c360(pvVar5,pcVar10);
    if (bVar2) {
      iVar3 = this_00[7];
      iVar8 = 0;
      if (0 < iVar3) {
        do {
          FUN_0043da70((int)this + -0x24);
          iVar3 = this_00[7];
          iVar8 = iVar8 + 1;
        } while (iVar8 < iVar3);
      }
      ExceptionList = pvStack_c;
      return CONCAT31((int3)((uint)iVar3 >> 8),1);
    }
    pcVar10 = "settime";
    pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
    bVar2 = FUN_0040c360(pvVar5,pcVar10);
    if (bVar2) {
      iVar3 = this_00[7];
      iVar8 = 0;
      if (0 < iVar3) {
        do {
          FUN_00473d20(this_00,&param_1,iVar8,0);
          uVar6 = FUN_0040cc50(&param_1,&piStack_18);
          uStack_4 = 0xffffffff;
          if ((char)uVar6 == '\0') {
            FUN_00401c90(&param_1);
          }
          else {
            FUN_00401c90(&param_1);
            *(int **)((int)this + 0x1b4) = piStack_18;
          }
          iVar3 = this_00[7];
          iVar8 = iVar8 + 1;
        } while (iVar8 < iVar3);
      }
      ExceptionList = pvStack_c;
      return CONCAT31((int3)((uint)iVar3 >> 8),1);
    }
    pcVar10 = "settimescale";
    pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
    bVar2 = FUN_0040c360(pvVar5,pcVar10);
    if (bVar2) {
      iVar3 = this_00[7];
      iVar8 = 0;
      if (0 < iVar3) {
        do {
          FUN_00473d20(this_00,&param_1,iVar8,0);
          uVar6 = FUN_0040cc50(&param_1,&piStack_18);
          uStack_4 = 0xffffffff;
          if ((char)uVar6 == '\0') {
            FUN_00401c90(&param_1);
          }
          else {
            FUN_00401c90(&param_1);
            *(int **)((int)this + 0x1b8) = piStack_18;
          }
          iVar3 = this_00[7];
          iVar8 = iVar8 + 1;
        } while (iVar8 < iVar3);
      }
      ExceptionList = pvStack_c;
      return CONCAT31((int3)((uint)iVar3 >> 8),1);
    }
    pcVar10 = "sethitpoints";
    pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
    bVar2 = FUN_0040c360(pvVar5,pcVar10);
    if (bVar2) {
      iVar3 = this_00[7];
      iVar8 = 0;
      if (0 < iVar3) {
        do {
          FUN_00473d20(this_00,&param_1,iVar8,0);
          uStack_4 = 2;
          cVar1 = FUN_0040cd50(&param_1,&piStack_18);
          uStack_4 = 0xffffffff;
          if (cVar1 == '\0') {
            FUN_00401c90(&param_1);
          }
          else {
            FUN_00401c90(&param_1);
            *(int **)(*(int *)((int)this + 200) + 0x34) = piStack_18;
          }
          iVar3 = this_00[7];
          iVar8 = iVar8 + 1;
        } while (iVar8 < iVar3);
      }
      ExceptionList = pvStack_c;
      return CONCAT31((int3)((uint)iVar3 >> 8),1);
    }
    pcVar10 = "addtimecmd";
    pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
    bVar2 = FUN_0040c360(pvVar5,pcVar10);
    if (bVar2) {
      iVar3 = this_00[7];
      iVar8 = 0;
      if (0 < iVar3) {
        do {
          FUN_00473d20(this_00,&param_1,iVar8,0);
          uVar6 = FUN_0040cc50(&param_1,&uStack_14);
          ppiVar7 = &param_1;
          uStack_4 = 0xffffffff;
          if ((char)uVar6 != '\0') {
            FUN_00401c90(ppiVar7);
            FUN_00473d20(this_00,&piStack_18,iVar8,1);
            puStack_10 = &stack0xffffffd0;
            uStack_4 = 4;
            if (DAT_00501918 == '\0') {
              DAT_00501918 = '\x01';
              DAT_00501908 = 2;
              DAT_0050190c = 0;
              _DAT_00501910 = 1;
              DAT_00501914 = 0;
            }
            piVar9 = &DAT_00501908;
            if ((piStack_18 != &DAT_00501908) && (piVar9 = piStack_18, piStack_18 != &DAT_00501908))
            {
              *piStack_18 = *piStack_18 + 1;
            }
            FUN_0043e360((void *)((int)this + -0x24),uStack_14,piVar9);
            ppiVar7 = &piStack_18;
          }
          uStack_4 = 0xffffffff;
          FUN_00401c90(ppiVar7);
          iVar3 = this_00[7];
          iVar8 = iVar8 + 1;
        } while (iVar8 < iVar3);
      }
      ExceptionList = pvStack_c;
      return CONCAT31((int3)((uint)iVar3 >> 8),1);
    }
    pcVar10 = "removealltimecmds";
    pvVar5 = (void *)FUN_0040c330(this_00,&puStack_10);
    bVar2 = FUN_0040c360(pvVar5,pcVar10);
    if (!bVar2) {
      ExceptionList = pvStack_c;
      return (uint)extraout_var << 8;
    }
    iVar3 = this_00[7];
    iVar8 = 0;
    if (0 < iVar3) {
      do {
        FUN_0043c260((int)this + -0x24);
        iVar3 = this_00[7];
        iVar8 = iVar8 + 1;
      } while (iVar8 < iVar3);
    }
  }
  ExceptionList = pvStack_c;
  return CONCAT31((int3)((uint)iVar3 >> 8),1);
}


