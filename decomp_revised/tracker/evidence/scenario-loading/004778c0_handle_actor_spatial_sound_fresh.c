/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 004778C0
 * Analysis output, not recovered original source.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm:
   Resolve the actor name supplied by the ScriptActor context. If found, query its geometry
   interface, read the position vector at the returned object +0x28, and pass the sound string,
   optional parameters, source position, and ScriptActor owner to GetOrCreateSpatialSoundInstance
   (004749A0) on manager DAT_00503060. If the name is missing, log `Actor not found` and return
   false.

   Parameters:
   param_1 is the ScriptActor owner; param_2 is the sound command text; param_3 is the source actor
   name; param_4 is an optional sound parameter.

   Returns:
   Truth-like success when the actor is resolved and the sound instance is submitted.

   Evidence:
   Called by HandlePlaySoundCommand (00477AF0) and related sound commands. It calls
   FindScriptActorSoundOrigin (00477740) and the shared sound-instance manager.

   Limits:
   This maps script sound submission and its spatial source. Playback timing, attenuation, and
   audible output remain unobserved. */

uint __cdecl
HandleActorSpatialSoundCommand(undefined4 param_1,int *param_2,int *param_3,undefined1 *param_4)

{
  int *piVar1;
  int *piVar2;
  void *this;
  int *piVar3;
  int iVar4;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  uint local_4;

  puStack_8 = &LAB_004d2cf0;
  local_c = ExceptionList;
  local_4 = 1;
  ExceptionList = &local_c;
  piVar2 = (int *)FindScriptActorSoundOrigin(&param_3);
  piVar1 = param_3;
  if (piVar2 == (int *)0x0) {
    iVar4 = 0;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar2 = &DAT_00501908;
    if (param_3 != &DAT_00501908) {
      *param_3 = *param_3 + 1;
      piVar2 = param_3;
    }
    local_4._0_1_ = 2;
    local_64 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_60 = "script\\scripttrigger.cpp";
    local_5c = 0x2d1;
    local_58 = 0;
    local_4._0_1_ = 4;
    FUN_004a3a19(local_48,"Actor not found: ");
    this = (void *)FUN_0040f020(&local_64,piVar2);
    FUN_00401360(this,iVar4);
    local_4._0_1_ = 1;
    FUN_004012c0(&local_64);
    piVar2 = param_2;
    local_4 = (uint)local_4._1_3_ << 8;
    if (param_2 != &DAT_00501908) {
      *param_2 = *param_2 + -1;
      if (*param_2 < 1) {
        piVar2 = (int *)FUN_004011c0(&param_2);
      }
      else {
        param_2 = &DAT_00501908;
      }
    }
    if (piVar1 == &DAT_00501908) goto LAB_00477a4d;
    piVar3 = (int *)(*piVar1 - 1);
    *piVar1 = (int)piVar3;
    piVar2 = piVar3;
  }
  else {
    iVar4 = (**(code **)(*piVar2 + 0x18))();
    if (iVar4 != 0) {
      iVar4 = (**(code **)(*piVar2 + 0x18))();
      GetOrCreateSpatialSoundInstance
                (&DAT_00503060,&param_2,param_4,(undefined4 *)(iVar4 + 0x28),param_1);
      local_4 = local_4 & 0xffffff00;
      if (param_2 != &DAT_00501908) {
        *param_2 = *param_2 + -1;
        if (*param_2 < 1) {
          FUN_004011c0(&param_2);
        }
        else {
          param_2 = &DAT_00501908;
        }
      }
      local_4 = 0xffffffff;
      if ((param_3 != &DAT_00501908) && (iVar4 = *param_3, *param_3 = iVar4 + -1, iVar4 + -1 < 1)) {
        param_3 = (int *)FUN_004011c0(&param_3);
      }
      ExceptionList = local_c;
      return CONCAT31((int3)((uint)param_3 >> 8),1);
    }
    local_4 = local_4 & 0xffffff00;
    if (param_2 != &DAT_00501908) {
      *param_2 = *param_2 + -1;
      if (*param_2 < 1) {
        FUN_004011c0(&param_2);
      }
      else {
        param_2 = &DAT_00501908;
      }
    }
    piVar2 = param_3;
    if (param_3 == &DAT_00501908) goto LAB_00477a4d;
    piVar3 = (int *)(*param_3 - 1);
    *param_3 = (int)piVar3;
  }
  local_4 = 0xffffffff;
  if ((int)piVar3 < 1) {
    piVar2 = (int *)FUN_004011c0(&param_3);
  }
LAB_00477a4d:
  ExceptionList = local_c;
  return (uint)piVar2 & 0xffffff00;
}
