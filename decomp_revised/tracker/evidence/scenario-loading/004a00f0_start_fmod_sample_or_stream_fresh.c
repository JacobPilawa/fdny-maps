/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Entry: 004A00F0
 * Function: StartFMODSampleOrStream
 * Analysis output, not recovered original source.
 */

/* Start FMOD playback from a resolved resource. The sample branch optionally sets loop mode and
   calls FSOUND_PlaySoundEx; the stream branch calls FSOUND_Stream_PlayEx and sets pan. When the
   engine state permits, the channel is unpaused. Fresh EXE pseudocode, 2026-10-13. */

void * __thiscall StartFMODSampleOrStream(void *this,undefined4 param_1,char param_2)

{
  int iVar1;
  void *this_00;
  int *extraout_ECX;
  int *piVar2;
  int iVar3;
  undefined **ppuStack_7c;
  char *pcStack_78;
  undefined4 uStack_74;
  undefined1 auStack_60 [60];
  void *pvStack_24;
  int iStack_1c;
  void *pvStack_14;
  undefined1 *puStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;

  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d4940;
  pvStack_c = ExceptionList;
  if (*(char *)((int)this + 8) == '\0') {
    ExceptionList = &pvStack_c;
    if (param_2 != '\0') {
      pcStack_78 = *(char **)((int)this + 4);
      uStack_74 = 2;
      ppuStack_7c = (undefined **)0x4a014c;
      ExceptionList = &pvStack_c;
      _FSOUND_Sample_SetLoopMode_8();
    }
    ppuStack_7c = *(undefined ***)((int)this + 4);
    uStack_74 = 1;
    pcStack_78 = (char *)0x0;
    iVar1 = _FSOUND_PlaySoundEx_16();
    if (iVar1 == 0) {
      iVar3 = 2;
      puStack_10 = &stack0xffffff70;
      piVar2 = extraout_ECX;
      FUN_00401cb0(&stack0xffffff70,this);
      ppuStack_7c = &PTR_FUN_004dc554;
      iStack_1c = iVar1;
      FUN_004a3118();
      pcStack_78 = "sound\\SoundSample.cpp";
      uStack_74 = 0x42;
      iStack_1c = CONCAT31(iStack_1c._1_3_,2);
      FUN_004a3a19(auStack_60,"TSoundSample: Error playing sample: ");
      this_00 = (void *)FUN_0040f020(&ppuStack_7c,piVar2);
      FUN_00401360(this_00,iVar3);
      iStack_1c = 0xffffffff;
      FUN_004012c0(&ppuStack_7c);
    }
  }
  else {
    ppuStack_7c = *(undefined ***)((int)this + 4);
    uStack_74 = 1;
    pcStack_78 = (char *)0x0;
    ExceptionList = &pvStack_c;
    iVar1 = _FSOUND_Stream_PlayEx_16();
    _FSOUND_SetPan_8();
  }
  if ((char)pvStack_c == '\0') {
    _FSOUND_SetPaused_8();
  }
  FUN_0049f220(pvStack_14,iVar1);
  ExceptionList = pvStack_24;
  return pvStack_14;
}
