/* Fresh Ghidra 12.0.4 headless decompilation.
 * Binary: inputs/original/binaries/FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * VA: 0049DBC0; see player_physics_timestep_2026-10-14.md for context.
 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0049dbc0(int param_1)

{
  float *pfVar1;
  float *pfVar2;
  float *pfVar3;
  undefined4 *puVar4;
  int iVar5;
  float fVar6;
  float local_30 [3];
  float local_24 [3];
  float local_18 [3];
  float local_c [3];
  
  iVar5 = 0;
  pfVar3 = (float *)(param_1 + 0xfc);
  do {
    pfVar2 = (float *)(param_1 + 0x114);
    if (iVar5 != 0) {
      if (iVar5 == 1) {
        pfVar2 = (float *)(param_1 + 0x118);
      }
      else if (iVar5 == 2) {
        pfVar2 = (float *)(param_1 + 0x11c);
      }
      else {
        pfVar2 = &DAT_00502b6c;
      }
    }
    fVar6 = DAT_00502b04 * *pfVar2;
    pfVar2 = pfVar3;
    if (iVar5 != 0) {
      if (iVar5 == 1) {
        pfVar2 = (float *)(param_1 + 0x100);
      }
      else if (iVar5 == 2) {
        pfVar2 = (float *)(param_1 + 0x104);
      }
      else {
        pfVar2 = &DAT_00502b6c;
      }
    }
    pfVar1 = pfVar3;
    if (*pfVar2 <= (float)_DAT_004e3540) {
      pfVar2 = pfVar3;
      if (iVar5 != 0) {
        if (iVar5 == 1) {
          pfVar2 = (float *)(param_1 + 0x100);
        }
        else if (iVar5 == 2) {
          pfVar2 = (float *)(param_1 + 0x104);
        }
        else {
          pfVar2 = &DAT_00502b6c;
        }
      }
      if (*pfVar2 < (float)_DAT_004e3540) {
        pfVar2 = pfVar3;
        if (iVar5 != 0) {
          if (iVar5 == 1) {
            pfVar2 = (float *)(param_1 + 0x100);
          }
          else if (iVar5 == 2) {
            pfVar2 = (float *)(param_1 + 0x104);
          }
          else {
            pfVar2 = &DAT_00502b6c;
          }
        }
        if (_DAT_004dc50c < fVar6 + *pfVar2) goto LAB_0049dd27;
        if (iVar5 != 0) {
          if (iVar5 == 1) {
            pfVar1 = (float *)(param_1 + 0x100);
          }
          else if (iVar5 == 2) {
            pfVar1 = (float *)(param_1 + 0x104);
          }
          else {
            pfVar1 = &DAT_00502b6c;
          }
        }
        fVar6 = fVar6 + *pfVar1;
        goto LAB_0049dd7d;
      }
    }
    else {
      pfVar2 = pfVar3;
      if (iVar5 != 0) {
        if (iVar5 == 1) {
          pfVar2 = (float *)(param_1 + 0x100);
        }
        else if (iVar5 == 2) {
          pfVar2 = (float *)(param_1 + 0x104);
        }
        else {
          pfVar2 = &DAT_00502b6c;
        }
      }
      if (*pfVar2 - fVar6 < _DAT_004dc50c) {
LAB_0049dd27:
        if (iVar5 == 0) {
          *pfVar3 = 0.0;
        }
        else if (iVar5 == 1) {
          *(undefined4 *)(param_1 + 0x100) = 0;
        }
        else if (iVar5 == 2) {
          *(undefined4 *)(param_1 + 0x104) = 0;
        }
        else {
          DAT_00502b6c = 0.0;
        }
      }
      else {
        if (iVar5 == 0) {
          fVar6 = *pfVar3 - fVar6;
        }
        else if (iVar5 == 1) {
          fVar6 = *(float *)(param_1 + 0x100) - fVar6;
          pfVar1 = (float *)(param_1 + 0x100);
        }
        else if (iVar5 == 2) {
          fVar6 = *(float *)(param_1 + 0x104) - fVar6;
          pfVar1 = (float *)(param_1 + 0x104);
        }
        else {
          fVar6 = DAT_00502b6c - fVar6;
          pfVar1 = &DAT_00502b6c;
        }
LAB_0049dd7d:
        *pfVar1 = fVar6;
      }
    }
    iVar5 = iVar5 + 1;
    if (2 < iVar5) {
      pfVar2 = (float *)(param_1 + 0x108);
      *(float *)(param_1 + 0x120) = *pfVar2;
      *(undefined4 *)(param_1 + 0x124) = *(undefined4 *)(param_1 + 0x10c);
      *(undefined4 *)(param_1 + 0x128) = *(undefined4 *)(param_1 + 0x110);
      *(float *)(param_1 + 0xf0) = *pfVar3;
      *(undefined4 *)(param_1 + 0xf4) = *(undefined4 *)(param_1 + 0x100);
      *(undefined4 *)(param_1 + 0xf8) = *(undefined4 *)(param_1 + 0x104);
      pfVar1 = (float *)FUN_00446390(local_30,pfVar2,DAT_00502b04);
      *pfVar3 = *pfVar1 + *pfVar3;
      *(float *)(param_1 + 0x100) = pfVar1[1] + *(float *)(param_1 + 0x100);
      *(float *)(param_1 + 0x104) = pfVar1[2] + *(float *)(param_1 + 0x104);
      fVar6 = DAT_00502b04 * DAT_00502b04;
      pfVar2 = (float *)FUN_00447710(local_30,0.5,pfVar2);
      pfVar2 = (float *)FUN_00446390(local_24,pfVar2,fVar6);
      pfVar3 = (float *)FUN_00447710(local_18,DAT_00502b04,pfVar3);
      puVar4 = (undefined4 *)FUN_0042d050(local_c,pfVar3,pfVar2);
      *(undefined4 *)(param_1 + 0x90) = *puVar4;
      *(undefined4 *)(param_1 + 0x94) = puVar4[1];
      *(undefined4 *)(param_1 + 0x98) = puVar4[2];
      *(undefined1 *)(param_1 + 0x12d) = *(undefined1 *)(param_1 + 300);
      *(undefined1 *)(param_1 + 300) = 0;
      return;
    }
  } while( true );
}
