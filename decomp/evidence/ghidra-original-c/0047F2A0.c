/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0047f2a0
 * Match: function entry
 * Entry: 0047f2a0 FUN_0047f2a0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0047f2a0(int param_1)

{
  float *pfVar1;
  float *pfVar2;
  float *pfVar3;
  int iVar4;
  float fVar5;
  float local_30 [3];
  float local_24 [3];
  float local_18 [3];
  float local_c [3];
  
  pfVar2 = (float *)FUN_00446390(local_30,(float *)(param_1 + 0x88),DAT_00502b04);
  pfVar1 = (float *)(param_1 + 0x7c);
  *pfVar1 = *pfVar2 + *(float *)(param_1 + 0x7c);
  *(float *)(param_1 + 0x80) = pfVar2[1] + *(float *)(param_1 + 0x80);
  *(float *)(param_1 + 0x84) = pfVar2[2] + *(float *)(param_1 + 0x84);
  fVar5 = DAT_00502b04;
  pfVar2 = (float *)FUN_00446390(local_30,pfVar1,DAT_00502b04);
  pfVar3 = (float *)FUN_00446390(local_24,(float *)(param_1 + 0x88),fVar5);
  pfVar3 = (float *)FUN_00446390(local_18,pfVar3,fVar5);
  pfVar2 = (float *)FUN_0042d050(local_c,pfVar3,pfVar2);
  iVar4 = 0;
  *(float *)(param_1 + 0x5c) = *pfVar2 + *(float *)(param_1 + 0x5c);
  *(float *)(param_1 + 0x60) = pfVar2[1] + *(float *)(param_1 + 0x60);
  *(float *)(param_1 + 100) = pfVar2[2] + *(float *)(param_1 + 100);
  do {
    pfVar2 = (float *)(param_1 + 0x94);
    if (iVar4 != 0) {
      if (iVar4 == 1) {
        pfVar2 = (float *)(param_1 + 0x98);
      }
      else if (iVar4 == 2) {
        pfVar2 = (float *)(param_1 + 0x9c);
      }
      else {
        pfVar2 = &DAT_00502b6c;
      }
    }
    fVar5 = DAT_00502b04 * *pfVar2;
    pfVar2 = pfVar1;
    if (iVar4 != 0) {
      if (iVar4 == 1) {
        pfVar2 = (float *)(param_1 + 0x80);
      }
      else if (iVar4 == 2) {
        pfVar2 = (float *)(param_1 + 0x84);
      }
      else {
        pfVar2 = &DAT_00502b6c;
      }
    }
    pfVar3 = pfVar1;
    if (*pfVar2 <= (float)_DAT_004e3540) {
      pfVar2 = pfVar1;
      if (iVar4 != 0) {
        if (iVar4 == 1) {
          pfVar2 = (float *)(param_1 + 0x80);
        }
        else if (iVar4 == 2) {
          pfVar2 = (float *)(param_1 + 0x84);
        }
        else {
          pfVar2 = &DAT_00502b6c;
        }
      }
      if (*pfVar2 < (float)_DAT_004e3540) {
        pfVar2 = pfVar1;
        if (iVar4 != 0) {
          if (iVar4 == 1) {
            pfVar2 = (float *)(param_1 + 0x80);
          }
          else if (iVar4 == 2) {
            pfVar2 = (float *)(param_1 + 0x84);
          }
          else {
            pfVar2 = &DAT_00502b6c;
          }
        }
        if (_DAT_004dc50c < fVar5 + *pfVar2) goto LAB_0047f491;
        if (iVar4 != 0) {
          if (iVar4 == 1) {
            pfVar3 = (float *)(param_1 + 0x80);
          }
          else if (iVar4 == 2) {
            pfVar3 = (float *)(param_1 + 0x84);
          }
          else {
            pfVar3 = &DAT_00502b6c;
          }
        }
        fVar5 = fVar5 + *pfVar3;
        goto LAB_0047f4e7;
      }
    }
    else {
      pfVar2 = pfVar1;
      if (iVar4 != 0) {
        if (iVar4 == 1) {
          pfVar2 = (float *)(param_1 + 0x80);
        }
        else if (iVar4 == 2) {
          pfVar2 = (float *)(param_1 + 0x84);
        }
        else {
          pfVar2 = &DAT_00502b6c;
        }
      }
      if (*pfVar2 - fVar5 < _DAT_004dc50c) {
LAB_0047f491:
        if (iVar4 == 0) {
          *pfVar1 = 0.0;
        }
        else if (iVar4 == 1) {
          *(undefined4 *)(param_1 + 0x80) = 0;
        }
        else if (iVar4 == 2) {
          *(undefined4 *)(param_1 + 0x84) = 0;
        }
        else {
          DAT_00502b6c = 0.0;
        }
      }
      else {
        if (iVar4 == 0) {
          fVar5 = *pfVar1 - fVar5;
        }
        else if (iVar4 == 1) {
          fVar5 = *(float *)(param_1 + 0x80) - fVar5;
          pfVar3 = (float *)(param_1 + 0x80);
        }
        else if (iVar4 == 2) {
          fVar5 = *(float *)(param_1 + 0x84) - fVar5;
          pfVar3 = (float *)(param_1 + 0x84);
        }
        else {
          fVar5 = DAT_00502b6c - fVar5;
          pfVar3 = &DAT_00502b6c;
        }
LAB_0047f4e7:
        *pfVar3 = fVar5;
      }
    }
    iVar4 = iVar4 + 1;
    if (2 < iVar4) {
      return;
    }
  } while( true );
}


