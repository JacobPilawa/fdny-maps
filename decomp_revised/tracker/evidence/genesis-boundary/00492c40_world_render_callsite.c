
void __thiscall FUN_00492c40(void *this,int param_1)

{
  int iVar1;
  
  *(undefined4 *)((int)this + 0x170) = *(undefined4 *)(param_1 + 0x5c);
  iVar1 = (int)this + 0xb4;
  if (*(char *)((int)this + 0x160) == '\0') {
    iVar1 = (int)this + 8;
  }
  geEngine_RenderWorld
            (*(undefined4 *)(*(int *)this + 0x74),*(undefined4 *)(iVar1 + 0x78),
             *(undefined4 *)(param_1 + 0x5c),0);
  return;
}

