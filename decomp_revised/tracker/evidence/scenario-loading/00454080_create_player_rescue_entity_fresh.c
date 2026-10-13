/* Fresh Ghidra decompilation
 * Program: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Function: CreatePlayerRescueEntity at 00454080
 * Ghidra analysis: current hash-pinned project; saved 2026-10-13
 */


/* Algorithm: Allocate 0x3580 bytes for a PlayerRescue entity, call ConstructPlayerRescueEntity with
   the entity and the factory request context, then run the returned object through the shared
   entity finalization helper.

   Parameters: param_1 is the class factory request/context.

   Returns: the constructed entity pointer, or null if allocation fails. */

undefined4 * CreatePlayerRescueEntity(void *param_1)

{
  void *this;
  undefined4 *puVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d077b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  this = operator_new(0x3580);
  puVar1 = (undefined4 *)0x0;
  local_4 = 0;
  if (this != (void *)0x0) {
    puVar1 = ConstructPlayerRescueEntity(this,param_1);
  }
  local_4 = 0xffffffff;
  FUN_0049d450((int)puVar1);
  ExceptionList = local_c;
  return puVar1;
}
