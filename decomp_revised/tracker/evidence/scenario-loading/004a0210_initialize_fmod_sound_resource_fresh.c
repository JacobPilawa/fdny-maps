/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Entry: 004A0210
 * Function: InitializeFMODSoundResource
 * Analysis output, not recovered original source.
 */

/* Algorithm: Resolve the stored file extension. MP3 resources set the stream flag and use
   FSOUND_Stream_OpenFile. Other resources store the caller's buffer type at resource+9 and use
   FSOUND_Sample_Load. Store the resulting FMOD handle and log a load error on failure.
   Parameters: this is the sound resource record; param_1 is its path; param_2 is the 2D/3D buffer
   flag.
   Returns: no meaningful value is consumed by the observed callers.
   Evidence: Fresh pseudocode; caller ParseSoundDefinition passes the type byte, and
   DispatchOrQueueSoundPlayback reads resource+9.
   Limits: Backend playback behavior is not observed. */

void __thiscall InitializeFMODSoundResource(void *this,int *param_1,undefined1 param_2)

{
  int *piVar1;
  int *piVar2;
  void *this_00;
  int iVar3;
  int *extraout_ECX;
  int *extraout_ECX_00;
  char *pcVar4;
  char *pcVar5;
  bool bVar6;
  undefined1 auStack_54 [60];
  void *pvStack_18;
  undefined4 uStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined1 *puStack_4;

  piVar1 = param_1;
  puStack_4 = (undefined1 *)0xffffffff;
  puStack_8 = &LAB_004d4940;
  pvStack_c = ExceptionList;
  piVar2 = *(int **)this;
  ExceptionList = &pvStack_c;
  if (piVar2 != (int *)*param_1) {
    ExceptionList = &pvStack_c;
    if (piVar2 != &DAT_00501908) {
      ExceptionList = &pvStack_c;
      *piVar2 = *piVar2 + -1;
      if (**(int **)this < 1) {
        FUN_004011c0(this);
      }
      else {
        *(undefined4 **)this = &DAT_00501908;
      }
    }
    piVar1 = (int *)*piVar1;
    *(int **)this = piVar1;
    if (piVar1 != &DAT_00501908) {
      *piVar1 = *piVar1 + 1;
    }
  }
  piVar2 = FUN_00404050(this,&param_1,4);
  bVar6 = true;
  iVar3 = 5;
  pcVar4 = (char *)(*piVar2 + 0xc);
  pcVar5 = ".mp3";
  do {
    if (iVar3 == 0) break;
    iVar3 = iVar3 + -1;
    bVar6 = *pcVar4 == *pcVar5;
    pcVar4 = pcVar4 + 1;
    pcVar5 = pcVar5 + 1;
  } while (bVar6);
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  if (bVar6) {
    *(undefined1 *)((int)this + 8) = 1;
  }
  if (*(char *)((int)this + 8) == '\0') {
    *(undefined1 *)((int)this + 9) = param_2;
    iVar3 = _FSOUND_Sample_Load_16();
    piVar2 = extraout_ECX_00;
  }
  else {
    iVar3 = _FSOUND_Stream_OpenFile_12();
    piVar2 = extraout_ECX;
  }
  *(int *)((int)this + 4) = iVar3;
  if (iVar3 == 0) {
    iVar3 = 2;
    puStack_4 = &stack0xffffff74;
    FUN_00401cb0(&stack0xffffff74,this);
    uStack_10 = 0;
    FUN_004a3118();
    uStack_10 = CONCAT31(uStack_10._1_3_,2);
    FUN_004a3a19(auStack_54,"TSoundSample: Error loading file: ");
    this_00 = (void *)FUN_0040f020(&stack0xffffff90,piVar2);
    FUN_00401360(this_00,iVar3);
    uStack_10 = 0xffffffff;
    FUN_004012c0((undefined4 *)&stack0xffffff90);
  }
  ExceptionList = pvStack_18;
  return;
}
