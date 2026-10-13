/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Entry: 004A0BE0
 * Function: DispatchOrQueueSoundPlayback
 * Analysis output, not recovered original source.
 */

/* Resolve the requested sound resource. When the manager busy flag is clear, start playback, apply
   channel volume, optional sample positioning, 3D attributes, then unpause. When busy, append the
   instance and optional parameter to the deferred queue. Missing resource logs Sound Play() failed.
   Fresh EXE pseudocode, 2026-10-13. */

void __thiscall DispatchOrQueueSoundPlayback(void *this,int *param_1,undefined1 *param_2)

{
  undefined1 *puVar1;
  undefined4 *puVar2;
  undefined *this_00;
  void *pvVar3;
  int *piVar4;
  int *extraout_ECX;
  undefined4 *puVar5;
  int *piVar6;
  int *piVar7;
  int iVar8;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  piVar6 = param_1;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d49a0;
  local_c = ExceptionList;
  if (*(char *)((int)this + 0x38) == '\0') {
    piVar4 = param_1 + 8;
    ExceptionList = &local_c;
    piVar7 = piVar4;
    this_00 = FUN_0049ffb0();
    pvVar3 = (void *)ResolveSoundResourceByName(this_00,piVar7);
    puVar1 = param_2;
    iVar8 = 0;
    if (pvVar3 == (void *)0x0) {
      param_2 = &stack0xffffff84;
      piVar6 = extraout_ECX;
      FUN_00401cb0(&stack0xffffff84,piVar4);
      local_4 = 0;
      local_64 = &PTR_FUN_004dc554;
      FUN_004a3118();
      local_60 = "sound\\SoundSystem.cpp";
      local_5c = 0x38;
      local_58 = 0;
      local_4 = CONCAT31(local_4._1_3_,2);
      FUN_004a3a19(local_48,"ERROR: Sound Play() failed on sound \"");
      pvVar3 = (void *)FUN_0040f020(&local_64,piVar6);
      FUN_004a3a19((void *)((int)pvVar3 + 0x1c),"\"");
      FUN_00401360(pvVar3,iVar8);
      local_4 = 0xffffffff;
      FUN_004012c0(&local_64);
    }
    else {
      *(undefined1 *)(piVar6 + 7) = *(undefined1 *)((int)pvVar3 + 9);
      piVar6[6] = (int)param_2;
      piVar4 = StartFMODSampleOrStream(pvVar3,&param_1,param_2 != (undefined1 *)0x0);
      *piVar6 = *piVar4;
      FUN_0043f660();
      SetFMODChannelVolume(piVar6);
      if (puVar1 != (undefined1 *)0x0) {
        FUN_00404ac0();
        FUN_0049f2d0(piVar6);
      }
      UpdateSound3DAttributes(piVar6);
      UnpauseFMODChannel(piVar6);
    }
  }
  else {
    ExceptionList = &local_c;
    puVar2 = operator_new(8);
    puVar5 = (undefined4 *)0x0;
    if (puVar2 != (undefined4 *)0x0) {
      puVar2[1] = param_1;
      *puVar2 = param_2;
      puVar5 = puVar2;
    }
    iVar8 = *(int *)((int)this + 0x40);
    FUN_0043b6e0((int *)((int)this + 0x3c),iVar8 + 1);
    *(undefined4 **)(*(int *)((int)this + 0x3c) + iVar8 * 4) = puVar5;
  }
  ExceptionList = local_c;
  return;
}
