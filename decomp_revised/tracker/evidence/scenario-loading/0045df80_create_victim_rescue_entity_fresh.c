
/* Creates a 0x22c-byte VictimRescue object and initializes it from the parsed BSP record. The
   initializer registers the authored initial XYZ/heading as a spawn spot before the queue's two
   root-vtable passes. */

undefined4 * CreateVictimRescueEntity(void *param_1)

{
  void *this;
  undefined4 *puVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d114b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  this = operator_new(0x22c);
  puVar1 = (undefined4 *)0x0;
  local_4 = 0;
  if (this != (void *)0x0) {
    puVar1 = InitializeVictimRescueEntity(this,param_1);
  }
  local_4 = 0xffffffff;
  FUN_0049d450((int)puVar1);
  ExceptionList = local_c;
  return puVar1;
}
