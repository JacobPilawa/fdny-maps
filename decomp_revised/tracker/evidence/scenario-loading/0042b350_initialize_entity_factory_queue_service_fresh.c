/* Fresh Ghidra 12.0.4 output from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e; captured 2026-10-13. */
/* Initializes the 0x4c-byte entity-factory queue service. It installs primary vtable 0x004E4158 and
   initializes a nested queue/pump interface at this+0x40 with vtable 0x004DF8F8. That nested
   table's slot +0x04 is ProcessEntitySpawnQueue (0x0049C670). The global service pointer is
   assigned by the parent subsystem constructor. */

undefined4 * __fastcall InitializeEntityFactoryQueueService(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  puStack_8 = &LAB_004cd8a8;
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
  param_1[0x10] = &PTR_FUN_004df8f8;
  *param_1 = &PTR_FUN_004e4158;
  ExceptionList = local_c;
  return param_1;
}
