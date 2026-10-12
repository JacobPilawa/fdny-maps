/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044b060
 * Match: function entry
 * Entry: 0044b060 FUN_0044b060
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0044b060(void *param_1)

{
  float *this;
  bool bVar1;
  float *pfVar2;
  int iVar3;
  undefined4 *puVar4;
  float fVar5;
  float local_30;
  undefined4 local_2c;
  float local_28;
  float local_24 [3];
  float local_18 [3];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cfa6e;
  local_c = ExceptionList;
  if (*(char *)((int)param_1 + 0x9c) != '\0') {
    ExceptionList = &local_c;
    FUN_0044ae80((int)param_1);
    if (*(char *)((int)param_1 + 0x50) != '\0') {
      if ((_DAT_00502c58 & 1) == 0) {
        _DAT_00502c58 = _DAT_00502c58 | 1;
        local_4 = 0;
        FUN_00481950((undefined4 *)&DAT_00502c40);
        _atexit((_func_4879 *)&DAT_004db740);
        local_4 = 0xffffffff;
      }
      fVar5 = 0.5;
      this = (float *)((int)param_1 + 0x44);
      pfVar2 = (float *)FUN_0042d2b0(this,local_24);
      pfVar2 = (float *)FUN_00446390(local_18,pfVar2,fVar5);
      _DAT_00502c40 = *pfVar2;
      _DAT_00502c48 = pfVar2[2];
      _DAT_00502c44 = 0;
      pfVar2 = (float *)FUN_00446390(local_18,this,0.5);
      _DAT_00502c40 = _DAT_00502c40 + *(float *)((int)param_1 + 0x28);
      _DAT_00502c44 = *(undefined4 *)((int)param_1 + 0x2c);
      _DAT_00502c48 = _DAT_00502c48 + *(float *)((int)param_1 + 0x30);
      _DAT_00502c4c = *pfVar2 + *(float *)((int)param_1 + 0x28);
      _DAT_00502c50 = *(float *)((int)param_1 + 0x48) + *(float *)((int)param_1 + 0x2c);
      _DAT_00502c54 = pfVar2[2] + *(float *)((int)param_1 + 0x30);
      FUN_0044a840(param_1,(undefined4 *)&DAT_00502c40);
      if (*(int *)((int)param_1 + 0x58) != 0) {
        FUN_00447890((void *)(*(int *)((int)param_1 + 0x58) + 0x84),(int)param_1);
        local_30 = *this * (float)_DAT_004e17d8;
        local_2c = *(undefined4 *)((int)param_1 + 0x54);
        local_28 = *(float *)((int)param_1 + 0x4c) * (float)_DAT_004e17d8;
        FUN_00458da0(*(void **)((int)param_1 + 0x58),&local_30);
      }
      *(undefined1 *)((int)param_1 + 0x50) = 0;
    }
    if (((float)_DAT_004dfc78 <
         *(float *)((int)param_1 + 0x4c) * *(float *)((int)param_1 + 0x4c) +
         *(float *)((int)param_1 + 0x48) * *(float *)((int)param_1 + 0x48) +
         *(float *)((int)param_1 + 0x44) * *(float *)((int)param_1 + 0x44)) &&
       (bVar1 = FUN_0048fe20(*(int *)((int)param_1 + 0x5c),(undefined4 *)((int)param_1 + 0x28)),
       bVar1)) {
      puVar4 = (undefined4 *)((int)param_1 + 0x6c);
      iVar3 = 4;
      do {
        FUN_0047f960((void *)*puVar4,*(void **)((int)param_1 + 0x5c));
        puVar4 = puVar4 + -1;
        iVar3 = iVar3 + -1;
      } while (iVar3 != 0);
    }
    if (*(int *)((int)param_1 + 0x58) != 0) {
      FUN_00458d90(*(int *)((int)param_1 + 0x58));
    }
  }
  ExceptionList = local_c;
  return;
}


