
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary:
   Player ScriptActor command helper for finalexitloadmap.
   
   Algorithm:
   - Build data\\maps\\<map-key>.bsp and check it with FindFirstFileA.
   - On success, call FUN_0042A260 with a forwarded command context and data\\resdefs\\rescue.rrf;
   this sets the pending-level byte at offset +0x68C.
   - ProcessEverGameUpdate (0042AEA0) consumes the pending byte and calls FUN_0042AB00, which
   constructs a World factory request and reaches the WorldEntity BSP loader through
   ProcessEntitySpawnQueue.
   - On missing path, log File not found from rescue\\PlayerRescueEnt.cpp.
   
   Parameters:
   - this: Player entity root object.
   - param_1: Parsed map-key argument used to construct the existence-check path.
   - param_2: Forwarded context value stored into the pending level descriptor; its exact identity
   and relation to the map-key field need a register/data-flow audit.
   
   Returns:
   - Low-byte status is produced; the Player handler reports the command handled independently.
   
   Evidence:
   - Oil Rig BSP stage exits request oil_rig_B, oil_rig_C, and oil_rig_D in order.
   - The static request-to-loader control path is mapped; runtime completion and timing are not.
   - EXE SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

uint __thiscall HandlePlayerFinalExitLoadMap(void *this,int *param_1,int *param_2)

{
  int *piVar1;
  bool bVar2;
  void *this_00;
  int *piVar3;
  int iVar4;
  int *local_6c;
  int *local_68;
  int *piStack_64;
  undefined **ppuStack_60;
  char *pcStack_5c;
  undefined4 uStack_58;
  undefined1 uStack_54;
  undefined1 auStack_44 [56];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004d0118;
  local_c = ExceptionList;
  local_4 = 0;
  ExceptionList = &local_c;
  if (*(int *)((int)this + 0x3528) == -1) {
    ExceptionList = &local_c;
    FUN_0044fba0(*(int *)(DAT_00503a78 + 0x30));
  }
  piVar3 = (int *)((*(int *)((int)this + 0x3528) - *(int *)((int)this + 0x3530)) -
                  *(int *)((int)this + 0x352c));
  if (piVar3 == (int *)0x0) {
    FUN_00422de0(&local_6c,(int *)&param_1);
    local_68 = (int *)&stack0xffffff84;
    local_4 = CONCAT31(local_4._1_3_,1);
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar3 = &DAT_00501908;
    if (local_6c != &DAT_00501908) {
      piVar3 = local_6c;
      if (local_6c != &DAT_00501908) {
        *local_6c = *local_6c + 1;
      }
    }
    bVar2 = FUN_004097f0(piVar3);
    if (bVar2) {
      FUN_00402790(&piStack_64,"data\\resdefs\\rescue.rrf");
      FUN_0042a260(DAT_00502aac,&param_2,&piStack_64);
      if (piStack_64 != &DAT_00501908) {
        *piStack_64 = *piStack_64 + -1;
        if (*piStack_64 < 1) {
          FUN_004011c0(&piStack_64);
        }
        else {
          piStack_64 = &DAT_00501908;
        }
      }
      if (local_68 != &DAT_00501908) {
        *local_68 = *local_68 + -1;
        if (*local_68 < 1) {
          FUN_004011c0(&local_68);
        }
        else {
          local_68 = &DAT_00501908;
        }
      }
      if ((param_2 != &DAT_00501908) && (*param_2 = *param_2 + -1, *param_2 < 1)) {
        param_2 = (int *)FUN_004011c0(&param_2);
      }
      ExceptionList = puStack_8;
      return CONCAT31((int3)((uint)param_2 >> 8),1);
    }
    iVar4 = 3;
    piStack_64 = (int *)&stack0xffffff84;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar3 = &DAT_00501908;
    piVar1 = (int *)&stack0xffffff84;
    if (local_68 != &DAT_00501908) {
      piVar3 = local_68;
      piVar1 = (int *)&stack0xffffff84;
      if (local_68 != &DAT_00501908) {
        *local_68 = *local_68 + 1;
        piVar1 = piStack_64;
      }
    }
    piStack_64 = piVar1;
    ppuStack_60 = &PTR_FUN_004dc554;
    FUN_004a3118();
    pcStack_5c = "rescue\\PlayerRescueEnt.cpp";
    uStack_58 = 0x589;
    uStack_54 = 0;
    FUN_004a3a19(auStack_44,"File not found: ");
    this_00 = (void *)FUN_0040f020(&ppuStack_60,piVar3);
    FUN_00401360(this_00,iVar4);
    FUN_004012c0(&ppuStack_60);
    if (local_68 != &DAT_00501908) {
      *local_68 = *local_68 + -1;
      if (*local_68 < 1) {
        FUN_004011c0(&local_68);
      }
      else {
        local_68 = &DAT_00501908;
      }
    }
    piVar3 = param_2;
    if (param_2 == &DAT_00501908) goto LAB_00450d25;
    *param_2 = *param_2 + -1;
    iVar4 = *param_2;
  }
  else {
    local_68 = (int *)&stack0xffffff84;
    FUN_00483f20((void *)((int)this + 0x5d8),(undefined4 *)&stack0xffffff84);
    FUN_004564c0((void *)((int)this + 0x5f4),7.0,piVar3);
    local_4 = 0xffffffff;
    piVar3 = param_1;
    if (param_1 == &DAT_00501908) goto LAB_00450d25;
    *param_1 = *param_1 + -1;
    iVar4 = *param_1;
  }
  if (iVar4 < 1) {
    piVar3 = (int *)FUN_004011c0(&param_2);
  }
LAB_00450d25:
  ExceptionList = puStack_8;
  return (uint)piVar3 & 0xffffff00;
}


