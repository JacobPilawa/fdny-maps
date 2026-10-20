/* Fresh Ghidra decompilation from the hash-pinned FDNYPort project.
 * Program: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Function: FUN_0042b2c0 at 0042B2C0
 * Query: Ghidra 12.0.4 headless, -noanalysis -readOnly, 2026-10-20
 */

undefined4 * __fastcall FUN_0042b2c0(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  puStack_8 = &LAB_004cd888;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *param_1 = &PTR_FUN_004e4158;
  param_1[0xf] = 0x10;
  param_1[0xe] = FUN_0043f660;
  param_1[10] = 0;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  param_1[2] = 0;
  param_1[1] = 0;
  param_1[5] = 0;
  param_1[4] = 0;
  *(undefined1 *)((int)param_1 + 0xd) = 0;
  *(undefined1 *)(param_1 + 3) = 0;
  param_1[6] = 0;
  local_4 = 0;
  FUN_0049c610(param_1 + 0x10);
  param_1[0x10] = &PTR_FUN_004df8e8;
  *param_1 = &PTR_FUN_004e4158;
  ExceptionList = local_c;
  return param_1;
}
