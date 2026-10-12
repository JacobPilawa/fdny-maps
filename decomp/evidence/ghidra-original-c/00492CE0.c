/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00492ce0
 * Match: function entry
 * Entry: 00492ce0 FUN_00492ce0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


undefined4 __thiscall
FUN_00492ce0(void *this,float *param_1,undefined4 *param_2,undefined4 *param_3,float *param_4,
            undefined4 *param_5,undefined4 *param_6)

{
  uint uVar1;
  int iVar2;
  float local_5c;
  float local_58;
  float local_54;
  float local_50;
  float local_4c;
  float local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined1 local_2c [12];
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  uVar1 = (int)this + 0xb4;
  if (*(char *)((int)this + 0x160) == '\0') {
    uVar1 = (int)this + 8;
  }
  if (*(int *)(uVar1 + 0x78) != 0) {
    local_50 = *param_1;
    local_4c = param_1[1];
    local_48 = param_1[2];
    local_5c = *param_1 + *param_4;
    local_58 = param_4[1] + param_1[1];
    local_54 = param_4[2] + param_1[2];
    local_38 = *param_2;
    local_34 = param_2[1];
    local_30 = param_2[2];
    local_44 = *param_3;
    local_40 = param_3[1];
    local_3c = param_3[2];
    if (*(char *)((int)this + 0x160) == '\0') {
      iVar2 = (int)this + 8;
    }
    else {
      iVar2 = (int)this + 0xb4;
    }
    iVar2 = geWorld_Collision(*(undefined4 *)(iVar2 + 0x78),&local_38,&local_44,&local_50,&local_5c,
                              0x43,2,0,0,0,local_2c);
    uVar1 = 0;
    if (iVar2 != 0) {
      *param_5 = local_20;
      param_5[1] = local_1c;
      param_5[2] = local_18;
      *param_6 = local_10;
      param_6[1] = local_c;
      param_6[2] = local_8;
      return CONCAT31((int3)((uint)param_6 >> 8),1);
    }
  }
  return uVar1 & 0xffffff00;
}


