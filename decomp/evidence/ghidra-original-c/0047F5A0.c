/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0047f5a0
 * Match: function entry
 * Entry: 0047f5a0 FUN_0047f5a0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0047f5a0(int param_1)

{
  float *pfVar1;
  undefined4 *puVar2;
  float fVar3;
  float local_74 [3];
  float local_68 [3];
  float local_5c [3];
  undefined1 local_50 [68];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004d347e;
  local_c = ExceptionList;
  if (*(char *)(param_1 + 0x78) != '\0') {
    ExceptionList = &local_c;
    if ((_DAT_005030bc & 1) == 0) {
      _DAT_005030bc = _DAT_005030bc | 1;
      local_4 = 0;
      ExceptionList = &local_c;
      _eh_vector_constructor_iterator_
                (&DAT_0050308c,0xc,4,(_func_void_void_ptr *)&LAB_0044c060,FUN_0043f660);
      _atexit((_func_4879 *)&LAB_004db900);
    }
    local_4 = 0xffffffff;
    pfVar1 = (float *)FUN_00446390(local_74,(float *)(param_1 + 0xb4),0.5);
    DAT_00503098 = *pfVar1;
    DAT_00503090 = pfVar1[1];
    DAT_0050308c = DAT_00503098 * _DAT_004e1608;
    DAT_00503094 = pfVar1[2];
    fVar3 = 0.5;
    DAT_0050309c = DAT_00503090;
    DAT_005030a0 = DAT_00503094;
    pfVar1 = (float *)FUN_0042d2b0((float *)(param_1 + 0xb4),local_74);
    pfVar1 = (float *)FUN_00446390(local_68,pfVar1,fVar3);
    DAT_005030b0 = *pfVar1;
    DAT_005030a8 = pfVar1[1];
    DAT_005030a4 = DAT_005030b0 * _DAT_004e1608;
    DAT_005030b8 = 0;
    DAT_005030ac = 0;
    DAT_005030a0 = 0.0;
    DAT_00503094 = 0.0;
    DAT_005030b4 = DAT_005030a8;
    FUN_004977b0(local_50,param_1 + 0x34);
    puVar2 = (undefined4 *)FUN_00495d20(local_68,(int)local_50,&DAT_0050308c);
    DAT_0050308c = (float)*puVar2;
    DAT_00503090 = (float)puVar2[1];
    DAT_00503094 = (float)puVar2[2];
    puVar2 = (undefined4 *)FUN_00495d20(local_68,(int)local_50,&DAT_00503098);
    DAT_00503098 = (float)*puVar2;
    DAT_0050309c = (float)puVar2[1];
    DAT_005030a0 = (float)puVar2[2];
    puVar2 = (undefined4 *)FUN_00495d20(local_68,(int)local_50,&DAT_005030a4);
    DAT_005030a4 = (float)*puVar2;
    DAT_005030a8 = (float)puVar2[1];
    DAT_005030ac = puVar2[2];
    puVar2 = (undefined4 *)FUN_00495d20(local_5c,(int)local_50,&DAT_005030b0);
    DAT_005030b0 = (float)*puVar2;
    DAT_005030b4 = (float)puVar2[1];
    DAT_005030b8 = puVar2[2];
    *(float *)(param_1 + 0xd8) = DAT_0050308c;
    *(float *)(param_1 + 0xdc) = DAT_00503090;
    *(float *)(param_1 + 0xe0) = DAT_00503094;
    *(float *)(param_1 + 0xfc) = DAT_00503098;
    *(float *)(param_1 + 0x100) = DAT_0050309c;
    *(float *)(param_1 + 0x104) = DAT_005030a0;
    *(float *)(param_1 + 0x120) = DAT_005030a4;
    *(float *)(param_1 + 0x124) = DAT_005030a8;
    *(undefined4 *)(param_1 + 0x128) = DAT_005030ac;
    *(float *)(param_1 + 0x144) = DAT_005030b0;
    *(float *)(param_1 + 0x148) = DAT_005030b4;
    *(undefined4 *)(param_1 + 0x14c) = DAT_005030b8;
    *(undefined1 *)(param_1 + 0x78) = 0;
  }
  ExceptionList = local_c;
  return;
}


