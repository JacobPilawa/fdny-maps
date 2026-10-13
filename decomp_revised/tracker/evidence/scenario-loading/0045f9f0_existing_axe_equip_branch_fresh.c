
void __thiscall FUN_0045f9f0(void *this,char param_1)

{
  *(char *)((int)this + 0x1fc) = param_1;
  if ((param_1 != '\0') && (*(char *)((int)this + 0x158) != '\0')) {
    *(int *)((int)this + 0xa90) = DAT_00502afc + 100;
                    /* WARNING: Could not recover jumptable at 0x0045fa2a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*(int *)((int)this + 0x200) + 0x3c))();
    return;
  }
  return;
}
