/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Entry: 0049B140
 * Function: ParseSoundBufferDefinition
 * Analysis output, not recovered original source.
 */

/* WARNING: Function: __chkstk replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm: Parse one sound-buffer definition. The caller-supplied type byte is 0 for
   ROC_SOUND_BUFFER2D and 1 for ROC_SOUND_BUFFER3D. For 3D, read two numeric tokens before the
   quoted audio path; pass the type byte to FindOrCreateSoundResourceByName and apply the parsed
   distances.
   Parameters: this is parser state; the caller pushes the buffer-type byte, confirmed at the
   0049B3B0 callsites.
   Returns: parser status for the terminating token; -1 on an invalid token sequence.
   Evidence: Fresh pseudocode and caller assembly; `sound.rrf` lines for SubDoor and Zapp3d.
   Limits: Numeric arguments are confirmed as min/max by the downstream FMOD setter. */

int __thiscall ParseSoundBufferDefinition(void *this)

{
  int *piVar1;
  char *pcVar2;
  char cVar3;
  float fVar4;
  float fVar5;
  int iVar6;
  char *pcVar7;
  undefined *this_00;
  int iVar8;
  uint unaff_retaddr;
  char in_stack_0000001c;
  void *in_stack_0000003c;
  undefined1 uStack00000074;
  uint3 uStack00000075;
  undefined1 in_stack_0000007c;
  int **ppiVar9;
  int **ppiVar10;
  int *piStack_1fac;
  int *piStack_1fa8;
  undefined1 *puStack_1fa4;
  int iStack_1fa0;
  int iStack_1f9c;
  char acStack_1f98 [4000];
  char acStack_ff8 [96];
  char acStack_f98 [3976];
  uint local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;

  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d4586;
  pvStack_c = ExceptionList;
  local_10 = DAT_005007e0 ^ unaff_retaddr;
  piVar1 = (int *)((int)this + 4);
  iVar8 = (int)this + 0x70;
  pcVar2 = (char *)((int)this + 0xc);
  ExceptionList = &pvStack_c;
  iVar6 = FUN_0049a5f0(*(FILE **)this,pcVar2,iVar8,piVar1,0,*(char *)((int)this + 0x74));
  if (iVar6 != 1) {
    ExceptionList = in_stack_0000003c;
    return -1;
  }
  pcVar7 = pcVar2;
  do {
    cVar3 = *pcVar7;
    pcVar7[(int)(acStack_ff8 + -(int)pcVar2)] = cVar3;
    pcVar7 = pcVar7 + 1;
  } while (cVar3 != '\0');
  if (in_stack_0000001c != '\0') {
    iVar6 = FUN_0049a5f0(*(FILE **)this,pcVar2,iVar8,piVar1,0,*(char *)((int)this + 0x74));
    if (iVar6 != 2) {
      ExceptionList = in_stack_0000003c;
      return -1;
    }
    FUN_004b9400();
    iVar6 = FUN_0049a5f0(*(FILE **)this,pcVar2,iVar8,piVar1,0,*(char *)((int)this + 0x74));
    if (iVar6 != 2) {
      ExceptionList = in_stack_0000003c;
      return -1;
    }
    FUN_004b9400();
  }
  iVar6 = FUN_0049a5f0(*(FILE **)this,pcVar2,iVar8,piVar1,0,*(char *)((int)this + 0x74));
  if (iVar6 != 0x1a) {
    ExceptionList = in_stack_0000003c;
    return -1;
  }
  FUN_0049a5f0(*(FILE **)this,pcVar2,iVar8,piVar1,1,'\"');
  pcVar7 = pcVar2;
  do {
    cVar3 = *pcVar7;
    pcVar7[(int)(acStack_1f98 + -(int)pcVar2)] = cVar3;
    pcVar7 = pcVar7 + 1;
  } while (cVar3 != '\0');
  FUN_00402790(&piStack_1fac,acStack_1f98);
  _uStack00000074 = 0;
  FUN_00402790(&piStack_1fa8,acStack_f98);
  ppiVar10 = &piStack_1fac;
  ppiVar9 = &piStack_1fa8;
  uStack00000074 = 1;
  this_00 = FUN_0049ffb0();
  puStack_1fa4 = FindOrCreateSoundResourceByName
                           (this_00,(int *)ppiVar9,(int *)ppiVar10,in_stack_0000007c);
  _uStack00000074 = (uint)uStack00000075 << 8;
  if (piStack_1fa8 != &DAT_00501908) {
    *piStack_1fa8 = *piStack_1fa8 + -1;
    if (*piStack_1fa8 < 1) {
      FUN_004011c0(&piStack_1fa8);
    }
    else {
      piStack_1fa8 = &DAT_00501908;
    }
  }
  _uStack00000074 = 0xffffffff;
  if (piStack_1fac != &DAT_00501908) {
    *piStack_1fac = *piStack_1fac + -1;
    if (*piStack_1fac < 1) {
      FUN_004011c0(&piStack_1fac);
    }
    else {
      piStack_1fac = &DAT_00501908;
    }
  }
  if (puStack_1fa4 != (undefined1 *)0x0) {
    fVar4 = (float)iStack_1f9c;
    if (iStack_1f9c < 0) {
      fVar4 = fVar4 + _DAT_004dfa30;
    }
    fVar5 = (float)iStack_1fa0;
    if (iStack_1fa0 < 0) {
      fVar5 = fVar5 + _DAT_004dfa30;
    }
    SetFMODSampleMinMaxDistance(puStack_1fa4,fVar5,fVar4);
  }
  iVar8 = FUN_0049a5f0(*(FILE **)this,pcVar2,iVar8,piVar1,0,*(char *)((int)this + 0x74));
  ExceptionList = in_stack_0000003c;
  return -(uint)(iVar8 != 0x2a);
}
