/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00497300
 * Match: function entry
 * Entry: 00497300 FUN_00497300
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void * __cdecl FUN_00497300(void *param_1,int param_2,int param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  undefined4 *puVar7;
  float local_50;
  float local_4c;
  float local_48;
  undefined1 local_44 [4];
  float local_40;
  float local_3c;
  float local_38;
  float local_34;
  float local_30;
  float local_2c;
  float local_28;
  float local_24;
  float local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined1 local_10 [16];
  
  FUN_0042d2d0(local_44,0);
  FUN_0042d280(&local_1c,&local_50,*(float *)(param_3 + 0x28) * *(float *)(param_2 + 0x34),
               *(float *)(param_3 + 0x2c) * *(float *)(param_2 + 0x38),
               *(float *)(param_3 + 0x30) * *(float *)(param_2 + 0x3c));
  puVar7 = (undefined4 *)FUN_00495d20(&local_50,param_2,(float *)(param_3 + 0x28));
  fVar1 = *(float *)(param_2 + 4);
  fVar2 = *(float *)(param_2 + 0x10);
  local_1c = *puVar7;
  fVar3 = *(float *)(param_2 + 0x1c);
  local_18 = puVar7[1];
  fVar4 = *(float *)(param_2 + 0x14);
  local_14 = puVar7[2];
  fVar5 = *(float *)(param_2 + 0x20);
  fVar6 = *(float *)(param_2 + 8);
  local_4c = *(float *)(param_2 + 0x18);
  local_48 = *(float *)(param_2 + 0x24);
  local_50 = *(float *)(param_2 + 0xc);
  local_40 = fVar1 * *(float *)(param_3 + 4) +
             fVar2 * *(float *)(param_3 + 8) + fVar3 * *(float *)(param_3 + 0xc);
  local_3c = fVar6 * *(float *)(param_3 + 4) +
             fVar4 * *(float *)(param_3 + 8) + fVar5 * *(float *)(param_3 + 0xc);
  local_38 = local_50 * *(float *)(param_3 + 4) +
             local_4c * *(float *)(param_3 + 8) + local_48 * *(float *)(param_3 + 0xc);
  local_34 = fVar1 * *(float *)(param_3 + 0x10) +
             fVar2 * *(float *)(param_3 + 0x14) + fVar3 * *(float *)(param_3 + 0x18);
  local_30 = fVar6 * *(float *)(param_3 + 0x10) +
             fVar4 * *(float *)(param_3 + 0x14) + fVar5 * *(float *)(param_3 + 0x18);
  local_2c = local_50 * *(float *)(param_3 + 0x10) +
             local_4c * *(float *)(param_3 + 0x14) + local_48 * *(float *)(param_3 + 0x18);
  local_28 = fVar1 * *(float *)(param_3 + 0x1c) +
             fVar2 * *(float *)(param_3 + 0x20) + fVar3 * *(float *)(param_3 + 0x24);
  local_24 = fVar6 * *(float *)(param_3 + 0x1c) +
             fVar4 * *(float *)(param_3 + 0x20) + fVar5 * *(float *)(param_3 + 0x24);
  local_20 = local_50 * *(float *)(param_3 + 0x1c) +
             local_4c * *(float *)(param_3 + 0x20) + local_48 * *(float *)(param_3 + 0x24);
  FUN_0042d280(local_10,&local_50,*(float *)(param_3 + 0x34) * *(float *)(param_2 + 0x34),
               *(float *)(param_3 + 0x38) * *(float *)(param_2 + 0x38),
               *(float *)(param_3 + 0x3c) * *(float *)(param_2 + 0x3c));
  FUN_0042d0a0(param_1,(int)local_44);
  return param_1;
}


