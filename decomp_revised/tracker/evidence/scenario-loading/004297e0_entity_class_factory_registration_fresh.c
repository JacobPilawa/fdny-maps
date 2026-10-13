
/* Summary:
   Registers the game entity-class factory descriptors in the startup factory registry.

   Algorithm:
   - Allocates and initializes each descriptor in the class registry list.
   - Appends each descriptor to DAT_00503A78 + 0x14 through 0x0049F160.
   - The list includes the RCS_BModel descriptor from 0x0043B4B0 and the RCS_BoundsTrigger
   descriptor from 0x00440D60.

   Parameters:
   - None.

   Returns:
   - No value.

   Evidence:
   - Fresh Ghidra decompilation at 0x004297E0.
   - The startup routine at 0x0042B4E0 calls this registry initializer.
   - The descriptor constructors store the exact class-name strings used by BSP entities.

   Limits:
   - This establishes registry setup; it does not prove any particular map entity's successful
   allocation. */

void RegisterEntityClassFactories(void)

{
  undefined4 *puVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cd6c0;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  puVar1 = operator_new(0xc);
  local_4 = 0;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_0043fce0(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 1;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = InitializeBModelFactoryDescriptor(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 2;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = InitializeBoundsTriggerFactoryDescriptor(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 3;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_004436a0(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 4;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_00441eb0(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 5;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_00465e90(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 6;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_004645e0(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 7;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_00464b70(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 8;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_00468390(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 9;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = InitializePlayerRescueFactoryDescriptor(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 10;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_00446b90(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0x3c);
  local_4 = 0xb;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_00448070(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 0xc;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = InitializeVictimRescueFactoryDescriptor(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 0xd;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = InitializeVictimAltSpotFactoryDescriptor(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 0xe;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_00442c30(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  puVar1 = operator_new(0xc);
  local_4 = 0xf;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_00446f00(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  ExceptionList = local_c;
  return;
}
