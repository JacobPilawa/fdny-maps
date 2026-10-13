/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 004749A0
 * Analysis output, not recovered original source.
 */

/* Algorithm:
   Search the sound-manager array for a reusable instance. Recently started or currently active
   entries are skipped; if none is reusable, allocate a new record and add it to the array. Store
   the source position, sound string, ScriptActor owner/context, and current game-clock value, then
   initialize the per-instance sound object.

   Parameters:
   this is the sound-instance manager; param_1 is the sound string; param_2 is an optional
   parameter; param_3 points to the source position; param_4 is the ScriptActor owner/context.

   Returns:
   No value.

   Evidence:
   Called by HandleActorSpatialSoundCommand and other script sound paths. The manager array is
   DAT_00503060 and the sound-instance timestamp is compared with DAT_00502AFC.

   Limits:
   This maps instance allocation/reuse and stored spatial data; exact audio playback, resource
   resolution, and audible output remain open. */

void __thiscall
GetOrCreateSpatialSoundInstance
          (void *this,undefined4 *param_1,undefined1 *param_2,undefined4 *param_3,undefined4 param_4
          )

{
  uint uVar1;
  void **ppvVar2;
  bool bVar3;
  undefined4 *puVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int local_54;
  char *local_50;
  char *local_4c;
  undefined4 local_48;
  int local_3c;
  char *local_38;
  char *local_34;
  undefined4 local_30;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d2afb;
  local_c = ExceptionList;
  iVar5 = *(int *)((int)this + 4);
  iVar7 = 0;
  ExceptionList = &local_c;
  ppvVar2 = &local_c;
  if (0 < iVar5) {
LAB_004749d0:
    ExceptionList = ppvVar2;
    uVar1 = DAT_00502afc;
    if ((iVar7 < 0) || (iVar5 <= iVar7)) {
      piVar6 = &local_3c;
      for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
        *piVar6 = 0;
        piVar6 = piVar6 + 1;
      }
      local_3c = 1;
      local_38 = "Array index out of bounds";
      local_34 = "..\\BluelAPI/Container/SArray.h";
      local_30 = 0x1cd;
      FUN_00422060(&local_3c);
    }
    if (uVar1 < *(int *)(*(int *)(*(int *)this + iVar7 * 4) + 0x28) + 1000U) goto LAB_00474a7d;
    if ((iVar7 < 0) || (*(int *)((int)this + 4) <= iVar7)) {
      piVar6 = &local_54;
      for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
        *piVar6 = 0;
        piVar6 = piVar6 + 1;
      }
      local_54 = 1;
      local_50 = "Array index out of bounds";
      local_4c = "..\\BluelAPI/Container/SArray.h";
      local_48 = 0x1cd;
      FUN_00422060(&local_54);
    }
    bVar3 = thunk_FUN_0049f260((int *)(*(int *)(*(int *)this + iVar7 * 4) + 4));
    if (bVar3) goto LAB_00474a7d;
    if ((iVar7 < 0) || (*(int *)((int)this + 4) <= iVar7)) {
      piVar6 = &local_24;
      for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
        *piVar6 = 0;
        piVar6 = piVar6 + 1;
      }
      local_24 = 1;
      local_20 = "Array index out of bounds";
      local_1c = "..\\BluelAPI/Container/SArray.h";
      local_18 = 0x1cd;
      FUN_00422060(&local_24);
    }
    puVar4 = *(undefined4 **)(*(int *)this + iVar7 * 4);
    if (puVar4 != (undefined4 *)0x0) goto LAB_00474b2d;
  }
LAB_00474ad7:
  puVar4 = operator_new(0x2c);
  iVar5 = 0;
  local_4 = 0;
  if (puVar4 == (undefined4 *)0x0) {
    puVar4 = (undefined4 *)0x0;
  }
  else {
    FUN_004a13a0(puVar4 + 1);
  }
  local_4 = 0xffffffff;
  if (0 < *(int *)((int)this + 4)) {
    piVar6 = *(int **)this;
    iVar5 = 0;
    do {
      if (*piVar6 == 0) goto LAB_00474b28;
      iVar5 = iVar5 + 1;
      piVar6 = piVar6 + 1;
    } while (iVar5 < *(int *)((int)this + 4));
  }
  FUN_0043b6e0(this,iVar5 + 1);
LAB_00474b28:
  *(undefined4 **)(*(int *)this + iVar5 * 4) = puVar4;
LAB_00474b2d:
  local_54 = 0;
  local_50 = (char *)0x0;
  local_4c = (char *)0x0;
  FUN_0042d280(&local_54,&local_3c,*param_3,param_3[1],param_3[2]);
  *puVar4 = param_4;
  puVar4[2] = local_54;
  puVar4[3] = local_50;
  puVar4[4] = local_4c;
  FUN_004a12f0(puVar4 + 1,param_1,(int)(puVar4 + 2),2);
  FUN_004a11a0(puVar4 + 1,param_2);
  puVar4[10] = DAT_00502afc;
  ExceptionList = local_c;
  return;
LAB_00474a7d:
  iVar5 = *(int *)((int)this + 4);
  iVar7 = iVar7 + 1;
  ppvVar2 = ExceptionList;
  if (iVar5 <= iVar7) goto LAB_00474ad7;
  goto LAB_004749d0;
}
