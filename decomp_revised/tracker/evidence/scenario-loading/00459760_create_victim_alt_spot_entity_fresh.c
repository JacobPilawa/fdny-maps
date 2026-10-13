
/* Creates a 0x28-byte VictimAltSpot object and copies its parsed entity fields. The object root
   vtable slot +0x20 is RegisterVictimAlternateSpawnSpot (0x00459840). */

undefined4 * CreateVictimAltSpotEntity(void *param_1)

{
  void *this;
  undefined4 *puVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0e4b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  this = operator_new(0x28);
  puVar1 = (undefined4 *)0x0;
  local_4 = 0;
  if (this != (void *)0x0) {
    puVar1 = ConstructVictimAltSpotEntity(this,param_1);
  }
  local_4 = 0xffffffff;
  FUN_0049d450((int)puVar1);
  ExceptionList = local_c;
  return puVar1;
}
