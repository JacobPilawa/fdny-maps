
/* Summary: Selects an already-equipped item by name.

   Algorithm: Parses the equipment slot, verifies the inventory entry exists, and calls
   SetPlayerSelectedEquipmentSlot with force false.

   Parameters: this is the Player entity; param_1 points to the parsed item name.

   Returns: Nonzero for a recognized, equipped slot; the lower-level selection gate may refuse to
   switch from the active item. */

uint __thiscall SelectPlayerEquipment(void *this,int *param_1)

{
  byte bVar1;
  undefined3 extraout_var;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  int iVar2;

  puStack_8 = &LAB_004d0318;
  local_c = ExceptionList;
  local_4 = 0;
  ExceptionList = &local_c;
  bVar1 = GetPlayerEquipmentSlotFromName((int *)&param_1);
  iVar2 = CONCAT31(extraout_var,bVar1);
  if (iVar2 == 0) {
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (iVar2 = *param_1, *param_1 = iVar2 + -1, iVar2 + -1 < 1)) {
      param_1 = (int *)FUN_004011c0(&param_1);
    }
    ExceptionList = local_c;
    return (uint)param_1 & 0xffffff00;
  }
  FUN_0048d490((int *)((int)this + 0x3564),iVar2);
  if (*(int *)(*(int *)((int)this + 0x3564) + iVar2 * 4) != 0) {
    SetPlayerSelectedEquipmentSlot(this,iVar2,'\0');
  }
  local_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (iVar2 = *param_1, *param_1 = iVar2 + -1, iVar2 + -1 < 1)) {
    param_1 = (int *)FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)param_1 >> 8),1);
}
