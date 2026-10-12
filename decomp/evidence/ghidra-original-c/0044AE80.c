/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044ae80
 * Match: function entry
 * Entry: 0044ae80 FUN_0044ae80
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0044ae80(int param_1)

{
  char *pcVar1;
  int iVar2;
  float *pfVar3;
  int iVar4;
  int iVar5;
  float *pfVar6;
  float local_34;
  float local_30;
  float local_2c;
  float local_28;
  float local_24 [3];
  float local_18 [3];
  float local_c [3];
  
  if (*(undefined1 **)(param_1 + 0x88) != (undefined1 *)0x0) {
    if ((*(int *)(param_1 + 0x8c) != 0) && (*(uint *)(*(int *)(param_1 + 0x8c) + 4) < DAT_00502afc))
    {
      **(undefined1 **)(param_1 + 0x88) = 0;
      *(undefined4 *)(param_1 + 0x88) = *(undefined4 *)(param_1 + 0x8c);
      iVar5 = 0;
      *(undefined4 *)(param_1 + 0x8c) = 0;
      if (0 < *(int *)(param_1 + 0x74)) {
        do {
          FUN_0048d490((int *)(param_1 + 0x70),iVar5);
          pcVar1 = *(char **)(*(int *)(param_1 + 0x70) + iVar5 * 4);
          if ((((*pcVar1 != '\0') && (pcVar1 != *(char **)(param_1 + 0x88))) &&
              (*(int *)(*(char **)(param_1 + 0x88) + 4) < *(int *)(pcVar1 + 4))) &&
             ((*(int *)(param_1 + 0x8c) == 0 ||
              (*(int *)(pcVar1 + 4) < *(int *)(*(int *)(param_1 + 0x8c) + 4))))) {
            *(char **)(param_1 + 0x8c) = pcVar1;
          }
          iVar5 = iVar5 + 1;
        } while (iVar5 < *(int *)(param_1 + 0x74));
      }
    }
    iVar5 = *(int *)(param_1 + 0x8c);
    if (iVar5 == 0) {
      pfVar3 = (float *)(*(int *)(param_1 + 0x88) + 8);
    }
    else {
      iVar2 = *(int *)(*(int *)(param_1 + 0x88) + 4);
      iVar4 = DAT_00502afc - iVar2;
      local_34 = (float)iVar4;
      if (iVar4 < 0) {
        local_34 = local_34 + _DAT_004dfa30;
      }
      local_34 = local_34 / (float)(*(int *)(iVar5 + 4) - iVar2);
      if (local_34 <= _DAT_004dfc24) {
        if (local_34 < _DAT_004dc50c) {
          local_34 = 0.0;
        }
      }
      else {
        local_34 = 1.0;
      }
      pfVar6 = (float *)(*(int *)(param_1 + 0x88) + 8);
      pfVar3 = (float *)FUN_0043b2d0(local_24,(float *)(iVar5 + 8),pfVar6);
      pfVar3 = (float *)FUN_00446390(local_18,pfVar3,local_34);
      pfVar3 = (float *)FUN_0042d050(local_c,pfVar6,pfVar3);
    }
    local_30 = *pfVar3 * *(float *)(param_1 + 0x90);
    local_2c = pfVar3[1] * *(float *)(param_1 + 0x94);
    local_28 = pfVar3[2] * *(float *)(param_1 + 0x98);
    pfVar3 = (float *)FUN_0043b2d0(local_c,&local_30,(float *)(param_1 + 0x44));
    if ((float)_DAT_004e16b8 <= pfVar3[2] * pfVar3[2] + pfVar3[1] * pfVar3[1] + *pfVar3 * *pfVar3) {
      *(undefined1 *)(param_1 + 0x50) = 1;
      *(float *)(param_1 + 0x44) = local_30;
      *(float *)(param_1 + 0x48) = local_2c;
      *(float *)(param_1 + 0x4c) = local_28;
    }
  }
  return;
}


