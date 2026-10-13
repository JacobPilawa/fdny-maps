/* Fresh Ghidra pseudocode captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 0043B4B0 (InitializeBModelFactoryDescriptor)
 * Analysis output, not recovered original source.
 */
/* Summary:
   Initializes the RCS_BModel factory descriptor.
   
   Algorithm:
   - Initializes the base descriptor and installs the BModel descriptor vtable.
   - Registers the class-name string RCS_BModel and the value 100 through 0x0049B690.
   - Returns the initialized descriptor to the startup class registry.
   
   Parameters:
   - param_1: caller-provided descriptor storage.
   
   Returns:
   - The initialized descriptor pointer.
   
   Evidence:
   - Fresh Ghidra decompilation at 0x0043B4B0.
   - Called by RegisterEntityClassFactories (0x004297E0).
   - The Gas R BSP contains RCS_BModel entity 309, named button1.
   
   Limits:
   - The third argument 100 is recorded without assigning undocumented semantics. */

undefined4 * __fastcall InitializeBModelFactoryDescriptor(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004ce9b8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *param_1 = &PTR_LAB_004e0b88;
  FUN_0043aee0((int)(param_1 + 1));
  local_4 = 0;
  *param_1 = &PTR_LAB_004e0b98;
  FUN_0049b690(param_1 + 1,"RCS_BModel",100);
  ExceptionList = local_c;
  return param_1;
}


