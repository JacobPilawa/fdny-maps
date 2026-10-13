
/* Factory descriptor constructor for the BSP class key RCS_VictimAltSpot.
   RegisterEntityClassFactories appends it after RCS_VictimRescue; its factory vtable create slot is
   0x00459760. */

undefined4 * __fastcall InitializeVictimAltSpotFactoryDescriptor(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0e68;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *param_1 = &PTR_LAB_004e0b88;
  FUN_0043aee0((int)(param_1 + 1));
  local_4 = 0;
  *param_1 = &PTR_LAB_004e2240;
  FUN_0049b690(param_1 + 1,"RCS_VictimAltSpot",100);
  ExceptionList = local_c;
  return param_1;
}
