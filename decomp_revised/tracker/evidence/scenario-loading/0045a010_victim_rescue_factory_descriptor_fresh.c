
/* Factory descriptor constructor for the BSP class key RCS_VictimRescue.
   RegisterEntityClassFactories appends it immediately before the RCS_VictimAltSpot descriptor; its
   factory vtable create slot is 0x0045DF80. */

undefined4 * __fastcall InitializeVictimRescueFactoryDescriptor(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0ea8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *param_1 = &PTR_LAB_004e0b88;
  FUN_0043aee0((int)(param_1 + 1));
  local_4 = 0;
  *param_1 = &PTR_LAB_004e22ac;
  FUN_0049b690(param_1 + 1,"RCS_VictimRescue",100);
  ExceptionList = local_c;
  return param_1;
}
