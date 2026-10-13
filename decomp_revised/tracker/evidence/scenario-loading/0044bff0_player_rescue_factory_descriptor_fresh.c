/* Fresh Ghidra decompilation
 * Program: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Function: InitializePlayerRescueFactoryDescriptor at 0044BFF0
 * Ghidra analysis: current hash-pinned project; saved 2026-10-13
 */


/* Algorithm: Initialize the entity factory descriptor base, install the PlayerRescue descriptor
   vtable, and store/hash the class name "RCS_PlayerRescue" with class limit 100.
   RegisterEntityClassFactories appends this descriptor to the global entity factory registry during
   startup.

   Parameters: param_1 points to the allocated factory descriptor.

   Returns: param_1. */

undefined4 * __fastcall InitializePlayerRescueFactoryDescriptor(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cfb58;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *param_1 = &PTR_LAB_004e0b88;
  FUN_0043aee0((int)(param_1 + 1));
  local_4 = 0;
  *param_1 = &PTR_LAB_004e1860;
  FUN_0049b690(param_1 + 1,"RCS_PlayerRescue",100);
  ExceptionList = local_c;
  return param_1;
}
