/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0042bf70
 * Match: function entry
 * Entry: 0042bf70 FUN_0042bf70
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0042bf70(int *param_1)

{
  float fVar1;
  float fVar2;
  DWORD DVar3;
  int iVar4;
  ulonglong uVar5;
  
  if ((char)param_1[7] == '\0') {
    *param_1 = param_1[1];
    DVar3 = timeGetTime();
    param_1[1] = DVar3 - param_1[2];
    iVar4 = (DVar3 - param_1[2]) - *param_1;
    fVar1 = (float)iVar4;
    if (iVar4 < 0) {
      fVar1 = fVar1 + _DAT_004dfa30;
    }
    fVar2 = (float)_DAT_004dfa28;
    param_1[4] = (int)(fVar1 * fVar2);
    if (fVar1 * fVar2 <= (float)_DAT_004dfa20) {
      do {
        Sleep(1);
        DVar3 = timeGetTime();
        param_1[1] = DVar3 - param_1[2];
        iVar4 = (DVar3 - param_1[2]) - *param_1;
        fVar1 = (float)iVar4;
        if (iVar4 < 0) {
          fVar1 = fVar1 + _DAT_004dfa30;
        }
        fVar2 = (float)_DAT_004dfa28;
        param_1[4] = (int)(fVar1 * fVar2);
      } while (fVar1 * fVar2 <= (float)_DAT_004dfa20);
    }
    if ((float)_DAT_004df7b8 < (float)param_1[4]) {
      uVar5 = FUN_004b9400();
      param_1[1] = (int)uVar5;
      uVar5 = FUN_004b9400();
      param_1[2] = (int)uVar5;
      param_1[4] = 0x3dcccccd;
    }
    fVar1 = (float)_DAT_004dd808;
    param_1[5] = (int)(fVar1 / (float)param_1[4]);
    param_1[6] = (int)((float)param_1[6] * (float)_DAT_004dfa08 +
                      (fVar1 / (float)param_1[4]) * (float)_DAT_004dfa10);
  }
  return;
}


