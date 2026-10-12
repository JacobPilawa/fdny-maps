
void __fastcall FUN_00492950(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = geWorld_Create(0);
  *(undefined4 *)(param_1 + 300) = uVar1;
  geEngine_AddWorld(*(undefined4 *)(DAT_00503a70 + 0x74),uVar1);
  return;
}

