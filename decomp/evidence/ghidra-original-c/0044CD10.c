/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044cd10
 * Match: function entry
 * Entry: 0044cd10 FUN_0044cd10
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __fastcall FUN_0044cd10(int *param_1)

{
  int iVar1;
  int *piVar2;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  param_1[0x13] = param_1[0x14d];
  param_1[0x14] = param_1[0x14e];
  param_1[0x15] = param_1[0x14f];
  param_1[0xd0d] = param_1[0x14c];
  param_1[0xd00] = param_1[0x13];
  param_1[0xd01] = param_1[0x14];
  param_1[0xd02] = param_1[0x15];
  if (param_1[0xd10] != 0) {
    FUN_0043b580(param_1[0xd10]);
    param_1[0xd10] = 0;
    FUN_004559e0(param_1 + 0x17d,0);
  }
  param_1[10] = 0x3f800000;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  param_1[0xe] = 0x3f800000;
  param_1[0xf] = 0;
  local_18 = 0;
  param_1[0x10] = 0;
  local_14 = (char *)0x0;
  param_1[0x11] = 0;
  local_10 = (char *)0x3f800000;
  param_1[0x12] = 0x3f800000;
  param_1[0xd4e] = 0;
  *(undefined1 *)((int)param_1 + 0x353f) = 0;
  *(undefined1 *)((int)param_1 + 0x353e) = 0;
  *(undefined1 *)((int)param_1 + 0x353d) = 0;
  *(undefined1 *)(param_1 + 0xd4f) = 0;
  param_1[99] = 3;
  param_1[0xd14] = 0x3f800000;
  param_1[0xd16] = 0x7fffffff;
  if (param_1[0xd5a] < 3) {
    piVar2 = &local_18;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar2 = 0;
      piVar2 = piVar2 + 1;
    }
    local_18 = 1;
    local_14 = "Array index out of bounds";
    local_10 = "..\\BluelAPI/Container/SArray.h";
    local_c = 0x1cd;
    FUN_00422060(&local_18);
  }
  if (*(int *)(param_1[0xd59] + 8) != 0) {
    if (param_1[0xd5a] < 3) {
      piVar2 = &local_18;
      for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
        *piVar2 = 0;
        piVar2 = piVar2 + 1;
      }
      local_18 = 1;
      local_14 = "Array index out of bounds";
      local_10 = "..\\BluelAPI/Container/SArray.h";
      local_c = 0x1cd;
      FUN_00422060(&local_18);
    }
    *(undefined4 *)(*(int *)(param_1[0xd59] + 8) + 0x8c8) = 0x3f800000;
  }
  param_1[0xd4d] = 0;
  *(undefined1 *)(param_1 + 0xd17) = 0;
  *(undefined1 *)(param_1 + 0x14b) = 0;
  FUN_0042d280(param_1 + 0x16e,&local_18,0,0,0);
  param_1[0x172] = 0;
  param_1[0xe2] = 0;
  param_1[0xe3] = 0;
  param_1[0xe1] = 0;
  param_1[0xe4] = 0;
  FUN_004a07c0(*(void **)(DAT_00503a78 + 8),1,DAT_004ff488);
  FUN_004a07c0(*(void **)(DAT_00503a78 + 8),2,DAT_004ff488);
  FUN_0044c360(param_1);
  FUN_00484260(param_1 + 0x176);
  FUN_0049da00(param_1);
  param_1[0x3d] = 0;
  *(undefined1 *)(param_1 + 0x4b) = 1;
  *(undefined1 *)((int)param_1 + 0x12d) = 1;
  return;
}


