
undefined4 __thiscall FUN_00493460(void *this,undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  
  uVar2 = geVFile_OpenNewSystem(0,1,param_2,0,1);
  puVar3 = operator_new(8);
  iVar1 = *(int *)((int)this + 100);
  FUN_0043b6e0((int *)((int)this + 0x60),iVar1 + 1);
  *(undefined4 **)(*(int *)((int)this + 0x60) + iVar1 * 4) = puVar3;
  uVar4 = geBitmap_CreateFromFile(uVar2);
  *puVar3 = uVar4;
  puVar3[1] = param_1;
  geEngine_AddBitmap(*(undefined4 *)(DAT_00503a70 + 0x74),uVar4);
  uVar2 = geVFile_Close(uVar2);
  return CONCAT31((int3)((uint)uVar2 >> 8),1);
}

