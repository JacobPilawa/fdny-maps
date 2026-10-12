/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0045dff0
 * Match: function entry
 * Entry: 0045dff0 FUN_0045dff0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __fastcall FUN_0045dff0(void *param_1)

{
  void *pvVar1;
  int iVar2;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  iVar2 = 0x14;
  do {
    local_c = 0x3f800000;
    local_8 = 0x3f800000;
    local_4 = 0x3f800000;
    FUN_0047db10((void *)((int)param_1 + 0x38),iVar2,0x3d4ccccd,0x3f800000,0x3f800000,&local_c);
    iVar2 = iVar2 + 1;
  } while (iVar2 < 0x27);
  iVar2 = 0x78;
  do {
    local_c = 0x3f800000;
    local_8 = 0x3f800000;
    local_4 = 0x3f800000;
    FUN_0047db10(param_1,iVar2,0x3d4ccccd,0x3f800000,0x3f800000,&local_c);
    iVar2 = iVar2 + 1;
  } while (iVar2 < 0x8b);
  local_c = 0x3f800000;
  local_8 = 0x3f800000;
  local_4 = 0x3f800000;
  FUN_0047db10((void *)((int)param_1 + 0x1c),0x28,0x3f800000,0x3f800000,0x3f800000,&local_c);
  iVar2 = 0xa1;
  do {
    local_c = 0x3f800000;
    local_8 = 0x3f800000;
    local_4 = 0x3f800000;
    FUN_0047db10((void *)((int)param_1 + 0x70),iVar2,0x3d99999a,0x3f800000,0x3f800000,&local_c);
    iVar2 = iVar2 + 1;
  } while (iVar2 < 0xb3);
  iVar2 = 0x8c;
  do {
    local_c = 0x3f800000;
    local_8 = 0x3f800000;
    local_4 = 0x3f800000;
    FUN_0047db10((void *)((int)param_1 + 0xa8),iVar2,0x3d4ccccd,0x3f800000,0x3f800000,&local_c);
    iVar2 = iVar2 + 1;
  } while (iVar2 < 0x97);
  pvVar1 = (void *)((int)param_1 + 0xc4);
  local_c = 0x3f800000;
  local_8 = 0x3f000000;
  local_4 = 0;
  FUN_0047db10(pvVar1,1,0x3f000000,0,0x3f800000,&local_c);
  local_c = 0x3f800000;
  local_8 = 0x3f800000;
  local_4 = 0x3f800000;
  FUN_0047db10(pvVar1,1,0x3f400000,0x3f800000,0x3f800000,&local_c);
  local_c = 0x3f800000;
  local_8 = 0x3f800000;
  local_4 = 0x3f800000;
  FUN_0047db10(pvVar1,1,0x3f800000,0,0x3fc00000,&local_c);
  pvVar1 = (void *)((int)param_1 + 0xe0);
  *(uint *)((int)param_1 + 0xdc) = *(uint *)((int)param_1 + 0xdc) & 0xfffffffe | 8;
  local_c = 0x3f800000;
  local_8 = 0x3f800000;
  local_4 = 0x3f800000;
  FUN_0047db10(pvVar1,0xe,0x3f000000,0x3e800000,0x40000000,&local_c);
  local_c = 0x3f800000;
  local_8 = 0x3f800000;
  local_4 = 0x3f800000;
  FUN_0047db10(pvVar1,0xe,0x3f800000,0x3f400000,0x40800000,&local_c);
  local_c = 0x3f800000;
  local_8 = 0x3f800000;
  local_4 = 0x3f800000;
  FUN_0047db10(pvVar1,0xe,0x3f000000,0,0x3f800000,&local_c);
  *(uint *)((int)param_1 + 0xf8) = *(uint *)((int)param_1 + 0xf8) & 0xfffffffe | 8;
  local_c = 0;
  local_8 = 0;
  local_4 = 0;
  pvVar1 = (void *)((int)param_1 + 0xfc);
  FUN_0047db10(pvVar1,2,0x3e800000,0x3f000000,0x3d99999a,&local_c);
  local_c = 0x3f800000;
  local_8 = 0x3f000000;
  local_4 = 0;
  FUN_0047db10(pvVar1,2,0x3e800000,0x3f800000,0x3de66667,&local_c);
  local_c = 0x3f800000;
  local_8 = 0x3f800000;
  local_4 = 0;
  FUN_0047db10(pvVar1,2,0x3f000000,0x3f400000,0x3e19999a,&local_c);
  local_c = 0;
  local_8 = 0;
  local_4 = 0;
  FUN_0047db10(pvVar1,2,0x3f000000,0x3f000000,0x3d99999a,&local_c);
  *(uint *)((int)param_1 + 0x114) = *(uint *)((int)param_1 + 0x114) | 8;
  local_c = 0x3f800000;
  local_8 = 0x3f800000;
  local_4 = 0x3f800000;
  FUN_0047db10((void *)((int)param_1 + 0x118),0x2ee,0x3f800000,0x3f800000,0x3f800000,&local_c);
  return;
}


