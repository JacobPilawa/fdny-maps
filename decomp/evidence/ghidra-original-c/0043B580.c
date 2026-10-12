/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0043b580
 * Match: function entry
 * Entry: 0043b580 FUN_0043b580
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0043b580(int param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float *pfVar4;
  float local_c [3];
  
  *(undefined4 *)(param_1 + 0x208) = DAT_00502afc;
  pfVar4 = (float *)FUN_0043b2d0(local_c,(float *)(param_1 + 0x1f0),(float *)(param_1 + 0x1fc));
  fVar1 = pfVar4[2];
  fVar2 = pfVar4[1];
  fVar3 = *pfVar4;
  *(undefined4 *)(param_1 + 0x210) = 0xc3c00000;
  *(undefined4 *)(param_1 + 0x218) = *(undefined4 *)(param_1 + 500);
  *(undefined4 *)(param_1 + 0x25c) = 2;
  fVar1 = SQRT(fVar1 * fVar1 + fVar2 * fVar2 + fVar3 * fVar3) * _DAT_004e0bb8;
  *(float *)(param_1 + 0x20c) = fVar1;
  *(float *)(param_1 + 0x214) =
       ((*(float *)(param_1 + 0x200) - *(float *)(param_1 + 500)) - fVar1 * fVar1 * _DAT_004e0bb4) /
       fVar1;
  return;
}


