
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* Algorithm:
   - Treat param_1 as the pending-level request block beginning at EverGame +0x660; store it as
   active state and enter the loading state.
   - Log the label string at block +0x04.
   - When the spawned-level fallback flag at +0x14 is set, compare the resource-definition string at
   +0x08 to rescue and update rescue mode.
   - Build the selected BSP path and parse its initial EVS mode-1 pass.
   - Create and append a World entity request to the service queue. A later call from
   ProcessEverGameUpdate drains that queue; this function itself does not construct World.
   - Clear level-transition state.
   
   Parameters:
   - this: EverGame state object.
   - param_1: pending-level request block. Established fields: +0x00 map-name string; +0x04 label;
   +0x08 resource-definition path; +0x14 spawned-level fallback flag (inferred from setter
   behavior).
   
   Returns:
   - No value.
   
   Evidence:
   - Called by ProcessEverGameUpdate after consuming this+0x68C.
   - The command-side map-key path is checked earlier by HandlePlayerFinalExitLoadMap. The queueing
   setter receives the original command key at EverGame +0x660; raw caller stack offsets prove it is the same value used for the BSP existence check. Successful runtime
   transition is unverified. */

void __thiscall ProcessLevelLoadRequest(void *this,void *param_1)

{
  void *pvVar1;
  int *piVar2;
  undefined4 uVar3;
  char *pcVar4;
  undefined4 extraout_EDX;
  int *unaff_EBP;
  uint unaff_retaddr;
  int iVar5;
  int local_f0;
  int *local_ec;
  int local_e8 [2];
  undefined **local_e0;
  char *local_dc;
  undefined4 local_d8;
  undefined1 local_d4;
  undefined1 local_c4 [60];
  undefined1 local_88 [116];
  void *pvStack_14;
  uint local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cd7b0;
  pvStack_c = ExceptionList;
  local_10 = DAT_005007e0 ^ unaff_retaddr;
  iVar5 = 0;
  local_ec = (int *)&stack0xfffffef8;
  ExceptionList = &pvStack_c;
  piVar2 = this;
  FUN_00401cb0(&stack0xfffffef8,(undefined4 *)((int)param_1 + 4));
  local_4 = 0;
  local_e0 = &PTR_FUN_004dc554;
  FUN_004a3118();
  local_dc = "main\\evergame.cpp";
  local_d8 = 0xf5;
  local_d4 = 0;
  local_4 = CONCAT31(local_4._1_3_,2);
  FUN_004a3a19(local_c4,"\n\n!!-- TEverGame: Loading level \"");
  pvVar1 = (void *)FUN_0040f020(&local_e0,piVar2);
  FUN_004a3a19((void *)((int)pvVar1 + 0x1c),"\" --!!");
  FUN_00401360(pvVar1,iVar5);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_e0);
  *(void **)((int)this + 0x688) = param_1;
  *(undefined4 *)((int)this + 0x6f0) = 2;
  FUN_004a0b60(*(int *)(DAT_00503a78 + 8));
  FUN_0042a070((int)this);
  if (*(char *)((int)param_1 + 0x14) != '\0') {
    iVar5 = FUN_00404c80((void *)((int)param_1 + 8),"rescue",0,'\x01');
    FUN_00424710(*(void **)((int)this + 0x6e4),
                 (int *)CONCAT31((int3)((uint)extraout_EDX >> 8),-1 < iVar5));
  }
  FUN_0047dc60();
  *(void **)((int)this + 0x688) = param_1;
  FUN_00471030();
  FUN_0049a520((int)local_88);
  FUN_0049b3b0(local_88,(char *)(*(int *)((int)param_1 + 8) + 0xc));
  piVar2 = FUN_00403e40(param_1,&local_ec,".bsp");
  local_4 = 3;
  FUN_00471310((char *)(*piVar2 + 0xc),'\x01');
  local_4 = 0xffffffff;
  if ((local_ec != &DAT_00501908) && (*local_ec = *local_ec + -1, *local_ec < 1)) {
    FUN_004011c0(&local_ec);
  }
  local_ec = operator_new(0x14);
  local_4 = 4;
  if (local_ec == (int *)0x0) {
    pvVar1 = (void *)0x0;
  }
  else {
    pvVar1 = (void *)FUN_0049f230(local_ec);
  }
  local_4 = 0xffffffff;
  FUN_0043aee0((int)local_e8);
  local_4 = 5;
  FUN_0049b690(local_e8,"World",100);
  uVar3 = FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),local_e8);
  *(undefined4 *)((int)pvVar1 + 0xc) = uVar3;
  *(undefined4 *)((int)pvVar1 + 8) = 0x2711;
  FUN_0042a050(param_1,&local_f0);
  local_4 = CONCAT31(local_4._1_3_,6);
  pcVar4 = operator_new(*(int *)(local_f0 + 4) + 1);
  iVar5 = *(int *)(local_f0 + 4) + 1;
  if (0 < iVar5) {
    if (*(char *)(local_f0 + 0xc) == '\0') {
      *pcVar4 = '\0';
    }
    else {
      FUN_00401240(pcVar4,(char *)(local_f0 + 0xc),iVar5);
    }
  }
  FUN_0049d130(pvVar1,pcVar4);
  if (0 < *(int *)(*(int *)(DAT_00503a78 + 0x24) + 0x18)) {
    local_e0 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_dc = "main\\evergame.cpp";
    local_d8 = 0x12a;
    local_d4 = 0;
    local_4._0_1_ = 7;
    FUN_004a3a19(local_c4,"WARNING: Stray items found in spawn queue; flushing");
    FUN_00401360(&local_e0,4);
    local_4 = CONCAT31(local_4._1_3_,6);
    FUN_004012c0(&local_e0);
    (**(code **)(**(int **)(DAT_00503a78 + 0x24) + 8))();
  }
  (**(code **)(**(int **)(DAT_00503a78 + 0x24) + 4))();
  *(undefined1 *)((int)this + 0x6a8) = 0;
  FUN_004a07c0(*(void **)(DAT_00503a78 + 8),1,DAT_004ff488);
  FUN_004a07c0(*(void **)(DAT_00503a78 + 8),2,DAT_004ff488);
  FUN_0049f2a0((int *)((int)this + 0x690));
  *(undefined4 *)((int)this + 0x6ec) = 0;
  pvStack_c = (void *)CONCAT31(pvStack_c._1_3_,5);
  if ((unaff_EBP != &DAT_00501908) && (*unaff_EBP = *unaff_EBP + -1, *unaff_EBP < 1)) {
    FUN_004011c0((undefined4 *)&stack0xffffff08);
  }
  pvStack_c = (void *)0xffffffff;
  FUN_0049b6f0((int)&local_f0);
  ExceptionList = pvStack_14;
  return;
}


