/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00429490 FUN_00429490 */

void __thiscall FUN_00429490(void *this,int *param_1)

{
  uint uVar1;
  
  uVar1 = (**(code **)(*param_1 + 0x1c))();
  if ((*(uint *)((int)this + 0x4c) & uVar1) != 0) {
    (**(code **)(*(int *)((int)this + 0xc) + 4))(param_1,1);
  }
  uVar1 = (**(code **)(*param_1 + 0x1c))();
  if ((*(uint *)((int)this + 0x94) & uVar1) != 0) {
    (**(code **)(*(int *)((int)this + 0x54) + 4))(param_1,1);
  }
  uVar1 = (**(code **)(*param_1 + 0x1c))();
  if ((*(uint *)((int)this + 0xdc) & uVar1) != 0) {
    (**(code **)(*(int *)((int)this + 0x9c) + 4))(param_1,1);
  }
  uVar1 = (**(code **)(*param_1 + 0x1c))();
  if ((*(uint *)((int)this + 0x124) & uVar1) != 0) {
    (**(code **)(*(int *)((int)this + 0xe4) + 4))(param_1,1);
  }
  return;
}



/* 0x0049c0e0 FUN_0049c0e0 */

void __fastcall FUN_0049c0e0(int *param_1)

{
  undefined4 *puVar1;
  void *_Memory;
  int iVar2;
  char cVar3;
  int *piVar4;
  
  param_1[2] = param_1[5];
  param_1[3] = 0;
  *(undefined1 *)(param_1 + 4) = 0;
  *(undefined1 *)((int)param_1 + 0x11) = 1;
  while( true ) {
    puVar1 = (undefined4 *)param_1[2];
    if (puVar1 == (undefined4 *)0x0) {
      return;
    }
    if (*(char *)((int)param_1 + 0x11) == '\0') {
      if ((char)param_1[4] == '\0') {
        param_1[3] = (int)puVar1;
        puVar1 = *(undefined4 **)(param_1[2] + 4);
        param_1[2] = (int)puVar1;
        if (puVar1 == (undefined4 *)0x0) {
          return;
        }
        piVar4 = (int *)*puVar1;
      }
      else {
        *(undefined1 *)(param_1 + 4) = 0;
        piVar4 = *(int **)param_1[2];
      }
    }
    else {
      *(undefined1 *)((int)param_1 + 0x11) = 0;
      piVar4 = (int *)*puVar1;
    }
    if (piVar4 == (int *)0x0) break;
    (**(code **)(*param_1 + 4))(piVar4);
    cVar3 = (**(code **)(*piVar4 + 4))();
    if (cVar3 != '\0') {
      _Memory = (void *)param_1[2];
      iVar2 = *(int *)((int)_Memory + 4);
      param_1[2] = iVar2;
      if (param_1[3] != 0) {
        *(int *)(param_1[3] + 4) = iVar2;
      }
      if ((void *)param_1[5] == _Memory) {
        param_1[5] = iVar2;
      }
      if ((void *)param_1[6] == _Memory) {
        param_1[6] = param_1[3];
      }
      if ((param_1[6] == 0) && (param_1[5] != 0)) {
        param_1[6] = param_1[5];
      }
      FUN_00429d20(param_1 + 1,(int)_Memory);
      _free(_Memory);
      param_1[7] = param_1[7] + -1;
      *(undefined1 *)(param_1 + 4) = 1;
    }
  }
  return;
}



/* 0x0049d420 FUN_0049d420 */

void __thiscall FUN_0049d420(void *this,int param_1)

{
  *(undefined ***)this = &PTR_FUN_004e4288;
  *(undefined4 *)((int)this + 0x14) = 0xffeb;
  *(undefined4 *)((int)this + 0x20) = 0;
  *(undefined4 *)((int)this + 0x18) = 0;
  *(undefined4 *)((int)this + 0xc) = *(undefined4 *)(param_1 + 0xc);
  *(undefined1 *)((int)this + 0x10) = 0;
  *(undefined1 *)((int)this + 0x1c) = 0;
  *(undefined1 *)((int)this + 0x1d) = 0;
  return;
}



/* 0x0043f340 BModelEntityConstructor */

/* Algorithm:
   Initialize the EngineEntity base and the embedded ScriptActor at this+0x24; install the BModel
   entity and ScriptActor vtables; initialize model-child, script/time-command, and animation state;
   register the entity in its category.
   
   Parameters:
   this is the allocated BModel entity; param_1 is the spawn/definition data consumed by the
   EngineEntity initializer.
   
   Returns:
   Returns this BModel entity pointer.
   
   Evidence:
   The constructor installs the ScriptActor vtable at 0x004E0DF0. Its +4 handler pointer at
   0x004E0DF4 is DispatchBModelScriptCommand (0x0043E680). The bank BSP includes RCS_BModel
   A_Name=vault with S_Script01=*VaultDoor. See
   tracker/evidence/scenario-loading/bank_vault_open_dispatch.md. EXE SHA-256
   4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void * __thiscall BModelEntityConstructor(void *this,void *param_1)

{
  int iVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004ced75;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0049d420(this,(int)param_1);
  local_4 = 0;
  FUN_004799b0((undefined4 *)((int)this + 0x24));
  *(undefined ***)((int)this + 0xa0) = &PTR_LAB_004e0b7c;
  *(undefined1 *)((int)this + 0xa4) = 0;
  local_4._0_1_ = 2;
  *(undefined ***)this = &PTR_FUN_004e0df8;
  *(undefined4 *)((int)this + 0x24) = &PTR_UpdateBModelEntity_004e0df0;
  *(undefined ***)((int)this + 0xa0) = &PTR_GetBModelUseAction_004e0de4;
  FUN_0042d2d0((void *)((int)this + 0xa8),0);
  *(undefined4 *)((int)this + 0xf0) = 0;
  *(undefined4 *)((int)this + 0xf4) = 0;
  *(undefined4 *)((int)this + 0xf8) = 0;
  *(undefined4 *)((int)this + 0x100) = 0;
  *(undefined4 *)((int)this + 0x104) = 0;
  *(undefined4 *)((int)this + 0x108) = 0;
  *(undefined4 *)((int)this + 0x10c) = 0;
  *(undefined4 *)((int)this + 0x110) = 0;
  *(undefined4 *)((int)this + 0x114) = 0;
  *(undefined4 *)((int)this + 0x118) = 0;
  *(undefined4 *)((int)this + 0x11c) = 0;
  *(undefined4 *)((int)this + 0x120) = 0;
  *(undefined4 *)((int)this + 0xfc) = 0;
  InitializeSoundInstance((undefined4 *)((int)this + 0x124));
  *(undefined4 *)((int)this + 0x15c) = 0x10;
  *(code **)((int)this + 0x158) = FUN_0043f660;
  *(undefined4 *)((int)this + 0x148) = 0;
  *(undefined4 *)((int)this + 0x14c) = 0;
  *(undefined4 *)((int)this + 0x150) = 0;
  *(undefined4 *)((int)this + 0x154) = 0;
  local_4._0_1_ = 4;
  FUN_00490270((void *)((int)this + 0x160),0x10);
  local_4._0_1_ = 5;
  _eh_vector_constructor_iterator_((void *)((int)this + 0x178),0x18,3,FUN_0043b780,FUN_00470c40);
  *(undefined4 *)((int)this + 0x1d4) = 0x10;
  *(code **)((int)this + 0x1d0) = FUN_0043f660;
  *(undefined4 *)((int)this + 0x1c0) = 0;
  *(undefined4 *)((int)this + 0x1c4) = 0;
  *(undefined4 *)((int)this + 0x1c8) = 0;
  *(undefined4 *)((int)this + 0x1cc) = 0;
  *(undefined4 *)((int)this + 0x1f0) = 0;
  *(undefined4 *)((int)this + 500) = 0;
  *(undefined4 *)((int)this + 0x1f8) = 0;
  *(undefined4 *)((int)this + 0x1fc) = 0;
  *(undefined4 *)((int)this + 0x200) = 0;
  *(undefined4 *)((int)this + 0x204) = 0;
  local_4._0_1_ = 7;
  *(undefined4 *)((int)this + 0x224) = 0;
  *(undefined4 *)((int)this + 0x228) = 0;
  *(undefined4 *)((int)this + 0x22c) = 0;
  *(undefined4 *)((int)this + 0x230) = 0;
  *(undefined4 *)((int)this + 0x234) = 0;
  *(undefined4 *)((int)this + 0x238) = 0;
  FUN_00481950((undefined4 *)((int)this + 0x240));
  *(undefined4 *)((int)this + 0x264) = 0;
  *(undefined4 *)((int)this + 0x270) = 0;
  *(undefined ***)((int)this + 0x260) = &PTR_LAB_004e0cbc;
  *(undefined4 *)((int)this + 0x278) = 0;
  *(undefined4 *)((int)this + 0x27c) = 0;
  *(undefined4 *)((int)this + 0x280) = 0;
  *(undefined4 *)((int)this + 0x284) = 0;
  *(undefined4 *)((int)this + 0x288) = 0;
  *(undefined4 *)((int)this + 0x28c) = 0;
  local_4 = CONCAT31(local_4._1_3_,8);
  FUN_0049d160(param_1,(int *)((int)this + 0xec),0x70);
  iVar1 = *(int *)((int)this + 0xec);
  *(undefined4 *)((int)this + 0xd0) = *(undefined4 *)(iVar1 + 100);
  *(undefined4 *)((int)this + 0xd4) = *(undefined4 *)(iVar1 + 0x68);
  *(undefined4 *)((int)this + 0xd8) = *(undefined4 *)(iVar1 + 0x6c);
  *(undefined4 *)((int)this + 0x18) = 4;
  FUN_0049d330(this,1);
  *(undefined4 *)((int)this + 0x25c) = 0;
  ExceptionList = local_c;
  return this;
}



/* 0x00453d50 ConstructPlayerRescueEntity */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm: Construct the PlayerRescue base entity, initialize its embedded ScriptActor at byte
   offset +0x140, install root and ScriptActor vtables, and initialize model, rescue, carry, sound,
   and player state fields.
   
   Parameters: this is the allocated PlayerRescue entity; param_1 is the factory request/context
   used to resolve its model.
   
   Returns: this. */

undefined4 * __thiscall ConstructPlayerRescueEntity(void *this,void *param_1)

{
  int iVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d075a;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0043f610(this,(int)param_1);
  local_4 = 0;
  FUN_0046f930((undefined4 *)((int)this + 0x140));
  local_4._0_1_ = 1;
  *(undefined ***)this = &PTR_FUN_004e1c48;
  *(undefined ***)((int)this + 0x24) = &PTR_FUN_004dfc30;
  *(undefined4 *)((int)this + 0x140) = &PTR_LAB_004e1c40;
  FUN_00491460((int)this + 0x198);
  local_4._0_1_ = 2;
  FUN_00481c40((undefined4 *)((int)this + 0x32c));
  local_4._0_1_ = 3;
  *(undefined4 *)((int)this + 0x394) = 0;
  *(undefined4 *)((int)this + 0x398) = 0;
  *(undefined4 *)((int)this + 0x39c) = 0;
  InitializeSoundInstance((undefined4 *)((int)this + 0x3a0));
  local_4._0_1_ = 4;
  InitializeSoundInstance((undefined4 *)((int)this + 0x3c4));
  local_4._0_1_ = 5;
  InitializeSoundInstance((undefined4 *)((int)this + 1000));
  local_4._0_1_ = 6;
  InitializeSoundInstance((undefined4 *)((int)this + 0x40c));
  local_4._0_1_ = 7;
  InitializeSoundInstance((undefined4 *)((int)this + 0x430));
  local_4._0_1_ = 8;
  InitializeSoundInstance((undefined4 *)((int)this + 0x454));
  local_4._0_1_ = 9;
  InitializeSoundInstance((undefined4 *)((int)this + 0x478));
  local_4._0_1_ = 10;
  InitializeSoundInstance((undefined4 *)((int)this + 0x49c));
  local_4._0_1_ = 0xb;
  InitializeSoundInstance((undefined4 *)((int)this + 0x4c0));
  local_4._0_1_ = 0xc;
  InitializeSoundInstance((undefined4 *)((int)this + 0x4e4));
  local_4._0_1_ = 0xd;
  InitializeSoundInstance((undefined4 *)((int)this + 0x508));
  *(undefined4 *)((int)this + 0x534) = 0;
  *(undefined4 *)((int)this + 0x538) = 0;
  *(undefined4 *)((int)this + 0x53c) = 0;
  *(undefined4 *)((int)this + 0x554) = 0x10;
  *(code **)((int)this + 0x550) = FUN_0044c130;
  *(undefined4 *)((int)this + 0x540) = 0;
  *(undefined4 *)((int)this + 0x544) = 0;
  *(undefined4 *)((int)this + 0x548) = 0;
  *(undefined4 *)((int)this + 0x54c) = 0;
  local_4._0_1_ = 0xf;
  FUN_004469f0((void *)((int)this + 0x55c));
  *(undefined4 *)((int)this + 0x5b8) = 0;
  *(undefined4 *)((int)this + 0x5bc) = 0;
  *(undefined4 *)((int)this + 0x5c0) = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  *(undefined4 **)((int)this + 0x5d0) = &DAT_00501908;
  *(undefined4 *)((int)this + 0x5d8) = 0;
  *(undefined4 *)((int)this + 0x5dc) = 0;
  *(undefined4 *)((int)this + 0x5e0) = 0;
  *(undefined4 *)((int)this + 0x5e4) = 0;
  *(undefined4 *)((int)this + 0x5e8) = 0;
  *(undefined4 *)((int)this + 0x5ec) = 0;
  *(undefined1 *)((int)this + 0x5f0) = 1;
  local_4._0_1_ = 0x12;
  FUN_00458b20((undefined4 *)((int)this + 0x5f4));
  local_4._0_1_ = 0x13;
  FUN_0044f170((undefined4 *)((int)this + 0x3254));
  local_4._0_1_ = 0x14;
  *(undefined4 *)((int)this + 0x3400) = 0;
  *(undefined4 *)((int)this + 0x3404) = 0;
  *(undefined4 *)((int)this + 0x3408) = 0;
  FUN_00451480((int)this + 0x340c);
  local_4._0_1_ = 0x15;
  *(undefined4 *)((int)this + 0x3444) = 0;
  *(undefined4 *)((int)this + 0x3448) = 0;
  *(undefined4 *)((int)this + 0x344c) = 0;
  FUN_0042d2d0((void *)((int)this + 0x3460),0);
  FUN_00483b00((undefined4 *)((int)this + 0x34a4));
  *(undefined4 *)((int)this + 0x3528) = 0xffffffff;
  *(undefined4 *)((int)this + 0x3530) = 0;
  *(undefined4 *)((int)this + 0x352c) = 0;
  FUN_00488f90((undefined4 *)((int)this + 0x3540));
  *(undefined4 *)((int)this + 0x3578) = 0x10;
  *(undefined1 **)((int)this + 0x3574) = &LAB_0044be00;
  *(undefined4 *)((int)this + 0x3564) = 0;
  *(undefined4 *)((int)this + 0x3568) = 0;
  *(undefined4 *)((int)this + 0x356c) = 0;
  *(undefined4 *)((int)this + 0x3570) = 0;
  local_4 = CONCAT31(local_4._1_3_,0x17);
  FUN_0049d160(param_1,(int *)((int)this + 0x194),0x3c);
  iVar1 = *(int *)((int)this + 0x194);
  *(undefined4 *)((int)this + 0x4c) = *(undefined4 *)(iVar1 + 0x30);
  *(undefined4 *)((int)this + 0x50) = *(undefined4 *)(iVar1 + 0x34);
  *(undefined4 *)((int)this + 0x54) = *(undefined4 *)(iVar1 + 0x38);
  *(undefined4 *)((int)this + 0x18) = 1;
  *(void **)(DAT_00503a78 + 0x30) = this;
  ExceptionList = local_c;
  return this;
}



/* 0x0042b4e0 FUN_0042b4e0 */

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0042b4e0(int param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  void *this;
  int *extraout_ECX;
  uint unaff_retaddr;
  undefined1 local_88 [120];
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cd950;
  local_c = ExceptionList;
  local_10 = DAT_005007e0 ^ unaff_retaddr;
  ExceptionList = &local_c;
  *(undefined1 *)(param_1 + 0x68c) = 0;
  puVar1 = operator_new(0x1044);
  local_4 = 0;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_0049b9e0(puVar1);
  }
  local_4 = 0xffffffff;
  *(undefined4 **)(param_1 + 0x3b4) = puVar1;
  puVar1 = operator_new(0x98);
  local_4 = 1;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_004a0f00(puVar1);
  }
  local_4 = 0xffffffff;
  *(undefined4 **)(param_1 + 0x3b0) = puVar1;
  piVar2 = operator_new(0x48);
  local_4 = 2;
  if (piVar2 == (int *)0x0) {
    piVar2 = (int *)0x0;
  }
  else {
    piVar2 = FUN_0049f190(piVar2);
  }
  local_4 = 0xffffffff;
  *(int **)(param_1 + 0x3b8) = piVar2;
  puVar1 = operator_new(0x550);
  local_4 = 3;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_0042b1f0(puVar1);
  }
  local_4 = 0xffffffff;
  *(undefined4 **)(param_1 + 0x3bc) = puVar1;
  puVar1 = operator_new(0x4c);
  local_4 = 4;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_0042b2c0(puVar1);
  }
  local_4 = 0xffffffff;
  *(undefined4 **)(param_1 + 0x3c4) = puVar1;
  puVar1 = operator_new(0x4c);
  local_4 = 5;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = InitializeEntityFactoryQueueService(puVar1);
  }
  *(undefined4 **)(param_1 + 0x3c0) = puVar1;
  *(undefined4 *)(DAT_00503a78 + 8) = *(undefined4 *)(param_1 + 0x3b0);
  *(undefined4 *)(DAT_00503a78 + 0x10) = *(undefined4 *)(param_1 + 0x3b4);
  *(undefined4 *)(DAT_00503a78 + 0x14) = *(undefined4 *)(param_1 + 0x3b8);
  *(undefined4 *)(DAT_00503a78 + 0x1c) = *(undefined4 *)(param_1 + 0x3bc);
  *(undefined4 *)(DAT_00503a78 + 0x20) = *(undefined4 *)(param_1 + 0x3c4);
  local_4 = 0xffffffff;
  *(undefined4 *)(DAT_00503a78 + 0x24) = *(undefined4 *)(param_1 + 0x3c0);
  puVar1 = operator_new(0xc);
  local_4 = 6;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_0049e360(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  RegisterEntityClassFactories();
  FUN_0049a520((int)local_88);
  LoadResourceDefinitionsFile(local_88,"data\\resdefs\\sound.rrf");
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar2 = &DAT_00501908;
  FUN_00401d80(&stack0xffffff5c,"data\\resdefs\\thegame.lev");
  FUN_00423160(piVar2);
  *(undefined4 *)(param_1 + 0x688) = 0;
  this = operator_new(0x18);
  local_4 = 7;
  if (this == (void *)0x0) {
    piVar2 = (int *)0x0;
  }
  else {
    piVar2 = extraout_ECX;
    FUN_00402790(&stack0xffffff5c,"data\\resdefs\\victims.srf");
    piVar2 = FUN_0048e740(this,piVar2);
  }
  local_4 = 0xffffffff;
  *(int **)(param_1 + 0x6e8) = piVar2;
  *(undefined1 *)(param_1 + 0x6a8) = 0;
  FUN_0042c1a0((int *)&DAT_00502ab8);
  ConstructMenuAndLoadMenuResources((void *)(param_1 + 0x3c8),param_1);
  FUN_0042d480(param_1 + 0x3c8);
  ExceptionList = local_c;
  return;
}



