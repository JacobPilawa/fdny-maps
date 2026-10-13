/* Fresh Ghidra decompilation, refreshed 2026-10-13 after call-site prototype correction.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 0043DB40 (GetBModelUseAction)
 * The pseudocode and listing are analysis output, not recovered original source.
 */
/* Summary:
   Queries the interaction action and optional geometry-query interface exposed by a BModel.
   
   Algorithm:
   - The receiver is the BModel secondary interaction interface at owner +0xA0.
   - If owner +0x258 (use mode) is zero, logs an invalid mode and returns 0.
   - For nonzero mode, releases any prior interface in the output slot and retains owner +0x260, the
   axis-box query interface.
   - Mode 1 asks the Player lift-action gate for its action code; other nonzero modes return 1000.
   
   Parameters:
   - ECX/this: BModel secondary interaction interface.
   - query_interface_out: pointer to the caller's retained query-interface slot. The shared Player
   scan at 0x00459350 calls virtual slot 0 with the address of one such slot.
   
   Returns:
   - 0 when use mode is disabled; otherwise the Player lift-action result or 1000.
   
   Evidence:
   - Fresh Ghidra pseudocode and assembly at 0x0043DB40.
   - Candidate scan call at 0x00459350 passes exactly one output-slot argument.
   - Constructor/vtable and initializer evidence at 0x0043F340 and 0x0043C670.
   - Gas R entity 309 authors BModel button1 with an S_Script use event and G_Liftable=0.
   
   Limits:
   - The meaning of action 1000 remains unresolved. No live selection or callback timing was
   captured. */

uint __thiscall GetBModelUseAction(void *this,void **query_interface_out)

{
  void *pvVar1;
  void *pvVar2;
  uint uVar3;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004ceab8;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0x1b8) == 0) {
    local_64 = &PTR_FUN_004dc554;
    ExceptionList = &local_c;
    FUN_004a3118();
    local_60 = "sharedents\\BModelEnt.cpp";
    local_5c = 0x8a;
    local_58 = 0;
    local_4 = 0;
    FUN_004a3a19(local_48,"TE_BModel::useMode is invalid");
    FUN_00401360(&local_64,0);
    local_4 = 0xffffffff;
    FUN_004012c0(&local_64);
    ExceptionList = local_c;
    return 0;
  }
  pvVar2 = *query_interface_out;
  pvVar1 = (void *)((int)this + 0x1c0);
  ExceptionList = &local_c;
  if (pvVar2 != pvVar1) {
    ExceptionList = &local_c;
    if (pvVar2 != (void *)0x0) {
      ExceptionList = &local_c;
      FUN_00429fe0(*(void **)((int)pvVar2 + 4),(int)query_interface_out);
      *query_interface_out = (void *)0x0;
    }
    if (pvVar1 != (void *)0x0) {
      FUN_0041b130((void *)((int)this + 0x1c4),query_interface_out);
      *query_interface_out = pvVar1;
    }
  }
  if (*(int *)((int)this + 0x1b8) == 1) {
    uVar3 = FUN_0044c520(*(void **)(DAT_00503a78 + 0x30),(int)this + -0xa0);
    ExceptionList = local_c;
    return uVar3;
  }
  ExceptionList = local_c;
  return 1000;
}


