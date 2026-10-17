/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

undefined4 * __thiscall
FUN_00435a10(void *this,undefined4 param_1,undefined4 param_2,int *param_3,undefined4 param_4,
            int param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,undefined4 param_9)

{
  undefined4 *this_00;
  float10 fVar1;
  ulonglong uVar2;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004ce42b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_004357d0(this,param_1,param_2,param_3);
  this_00 = (undefined4 *)((int)this + 0x20);
  *(undefined ***)this = &PTR_LAB_004e07c0;
  FUN_0047ddc0(this_00);
  *(int *)((int)this + 8) = *param_3;
  *(int *)((int)this + 0x10) = param_3[2];
  *(int *)((int)this + 0xc) = param_3[1];
  local_4 = 0;
  *(int *)((int)this + 0x14) = param_3[3];
  if (param_5 == 0) {
    FUN_0047ef60(this_00,param_4,'\0');
  }
  else {
    FUN_0047ee10(this_00,param_5,'\0');
    ScalePseudoRandomFloatToRange
              (0.0,(float)(*(int *)(*(int *)(*(int *)((int)this + 0x124) + 0xc) + 4) + -1));
    uVar2 = ConvertX87FloatToIntegerTowardZero();
    FUN_0047d300(*(void **)((int)this + 0x124),(int)uVar2);
    fVar1 = ScalePseudoRandomFloatToRange(0.5,1.5);
    *(float *)(*(int *)((int)this + 0x124) + 0x1c) = (float)fVar1;
  }
  FUN_0047ec80(this_00,param_6,param_7,param_8,param_9);
  FUN_0047edf0(this_00,(int *)((int)this + 8),0);
  *(undefined4 *)((int)this + 300) = 0;
  ExceptionList = local_c;
  return this;
}

 (GhidraScript)
