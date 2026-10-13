
/* Summary: Initializes a VictimRescue entity.

   Algorithm: Reads authored entity data, copies the initial location, initializes victim and
   embedded ScriptActor/roam fields, and registers the initial XYZ plus heading as a spawn spot.

   Parameters: this is the VictimRescue instance; the second parameter is the parsed world-entity
   data.

   Returns: the initialized instance pointer. */

undefined4 * __thiscall InitializeVictimRescueEntity(void *this,void *param_1)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1132;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0049d8c0(this,(int)param_1);
  local_4 = 0;
  FUN_004799b0((undefined4 *)((int)this + 0xd4));
  *(undefined ***)this = &PTR_FUN_004e25b8;
  *(undefined ***)((int)this + 0x24) = &PTR_FUN_004dfc30;
  *(undefined4 *)((int)this + 0xd4) = &PTR_FUN_004e25b0;
  local_4._0_1_ = 1;
  *(undefined4 *)((int)this + 0x158) = 0;
  *(undefined4 *)((int)this + 0x15c) = 0;
  *(undefined4 *)((int)this + 0x160) = 0;
  FUN_00480e00((void *)((int)this + 0x170),0);
  puVar2 = _malloc(0x24);
  *(undefined4 **)((int)this + 0x1c4) = puVar2;
  for (iVar3 = 9; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  **(undefined4 **)((int)this + 0x1c4) = 1;
  *(undefined4 *)(*(int *)((int)this + 0x1c4) + 0xc) = 1;
  *(undefined4 *)((int)this + 0x1c8) = 0;
  *(undefined4 *)((int)this + 0x1cc) = 0;
  *(undefined4 *)((int)this + 0x1d0) = 0;
  *(undefined4 *)((int)this + 0x1d4) = 0;
  *(undefined4 *)((int)this + 0x1d8) = 0;
  *(undefined4 *)((int)this + 0x1dc) = 0;
  *(undefined4 *)((int)this + 0x1e0) = 0;
  *(undefined4 *)((int)this + 0x1e4) = 0;
  *(undefined4 *)((int)this + 0x1e8) = 0;
  *(undefined1 *)((int)this + 0x1ec) = 1;
  *(undefined4 *)((int)this + 0x214) = 0x10;
  *(code **)((int)this + 0x210) = FUN_0043f660;
  *(undefined4 *)((int)this + 0x200) = 0;
  *(undefined4 *)((int)this + 0x204) = 0;
  *(undefined4 *)((int)this + 0x208) = 0;
  *(undefined4 *)((int)this + 0x20c) = 0;
  piVar1 = (int *)((int)this + 0x150);
  local_4 = CONCAT31(local_4._1_3_,3);
  *(undefined4 *)((int)this + 0x218) = 0;
  *(undefined4 *)((int)this + 0x21c) = 0;
  *(undefined4 *)((int)this + 0x220) = 0;
  FUN_0049d160(param_1,piVar1,0x34);
  iVar3 = *piVar1;
  *(undefined4 *)((int)this + 0x4c) = *(undefined4 *)(iVar3 + 4);
  *(undefined4 *)((int)this + 0x50) = *(undefined4 *)(iVar3 + 8);
  *(undefined4 *)((int)this + 0x54) = *(undefined4 *)(iVar3 + 0xc);
  RegisterVictimSpawnSpot(this,(undefined4 *)(*piVar1 + 4),*(undefined4 *)(*piVar1 + 0x18));
  *(undefined4 *)((int)this + 0x18) = 2;
  FUN_0049d330(this,1);
  *(undefined4 *)((int)this + 0x154) = 0;
  ExceptionList = local_c;
  return this;
}
