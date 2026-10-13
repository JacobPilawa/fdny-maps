
/* Algorithm: Open the requested resource-definition file and repeatedly read tokens. Dispatch
   ROC_SOUND_BUFFER2D/3D to ParseSoundBufferDefinition, ROC_MODEL to the model parser, ROC_BITMAP to
   the bitmap parser, and ROC_ALPHA_TEXTURE to the alpha-texture parser.
   Parameters: this is parser state; the argument is the file path.
   Returns: 0 after the input ends; -1 if opening the file fails.
   Evidence: Fresh pseudocode and xrefs to the two sound-buffer type strings.
   Limits: This documents static parse dispatch, not successful game startup. */

undefined4 __thiscall LoadResourceDefinitionsFile(void *this,char *param_1)

{
  char *pcVar1;
  FILE *pFVar2;
  int iVar3;
  char *pcVar4;
  char *pcVar5;
  bool bVar6;

  pFVar2 = (FILE *)FUN_004ba01a(param_1,"rb");
  *(FILE **)this = pFVar2;
  if (pFVar2 == (FILE *)0x0) {
    return 0xffffffff;
  }
  pcVar1 = (char *)((int)this + 0xc);
  iVar3 = FUN_0049a5f0(pFVar2,pcVar1,(int)this + 0x70,(int *)((int)this + 4),0,
                       *(char *)((int)this + 0x74));
  do {
    if (iVar3 != 1) {
      return 0;
    }
    iVar3 = 0x13;
    bVar6 = true;
    pcVar4 = pcVar1;
    pcVar5 = "ROC_SOUND_BUFFER2D";
    do {
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      bVar6 = *pcVar4 == *pcVar5;
      pcVar4 = pcVar4 + 1;
      pcVar5 = pcVar5 + 1;
    } while (bVar6);
    if (bVar6) {
      ParseSoundBufferDefinition(this);
    }
    else {
      iVar3 = 0x13;
      bVar6 = true;
      pcVar4 = pcVar1;
      pcVar5 = "ROC_SOUND_BUFFER3D";
      do {
        if (iVar3 == 0) break;
        iVar3 = iVar3 + -1;
        bVar6 = *pcVar4 == *pcVar5;
        pcVar4 = pcVar4 + 1;
        pcVar5 = pcVar5 + 1;
      } while (bVar6);
      if (bVar6) {
        ParseSoundBufferDefinition(this);
      }
      else {
        iVar3 = 10;
        bVar6 = true;
        pcVar4 = pcVar1;
        pcVar5 = "ROC_MODEL";
        do {
          if (iVar3 == 0) break;
          iVar3 = iVar3 + -1;
          bVar6 = *pcVar4 == *pcVar5;
          pcVar4 = pcVar4 + 1;
          pcVar5 = pcVar5 + 1;
        } while (bVar6);
        if (bVar6) {
          FUN_0049ade0(this);
        }
        else {
          iVar3 = 0xb;
          bVar6 = true;
          pcVar4 = pcVar1;
          pcVar5 = "ROC_BITMAP";
          do {
            if (iVar3 == 0) break;
            iVar3 = iVar3 + -1;
            bVar6 = *pcVar4 == *pcVar5;
            pcVar4 = pcVar4 + 1;
            pcVar5 = pcVar5 + 1;
          } while (bVar6);
          if (bVar6) {
            FUN_0049aee0(this);
          }
          else {
            iVar3 = 0x12;
            bVar6 = true;
            pcVar4 = pcVar1;
            pcVar5 = "ROC_ALPHA_TEXTURE";
            do {
              if (iVar3 == 0) break;
              iVar3 = iVar3 + -1;
              bVar6 = *pcVar4 == *pcVar5;
              pcVar4 = pcVar4 + 1;
              pcVar5 = pcVar5 + 1;
            } while (bVar6);
            if (bVar6) {
              FUN_0049afe0(this);
            }
          }
        }
      }
    }
    iVar3 = FUN_0049a5f0(*(FILE **)this,pcVar1,(int)this + 0x70,(int *)((int)this + 4),0,
                         *(char *)((int)this + 0x74));
  } while( true );
}
