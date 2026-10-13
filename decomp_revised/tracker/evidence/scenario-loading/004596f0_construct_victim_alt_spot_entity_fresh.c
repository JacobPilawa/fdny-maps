
/* VictimAltSpot object constructor: installs root vtable address point 0x004E21E8 and copies 0x18
   bytes of parsed fields into object +0x24. */

undefined4 * __thiscall ConstructVictimAltSpotEntity(void *this,void *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0e28;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0049d420(this,(int)param_1);
  local_4 = 0;
  *(undefined ***)this = &PTR_FUN_004e21e8;
  FUN_0049d160(param_1,(int *)((int)this + 0x24),0x18);
  ExceptionList = local_c;
  return this;
}
