
undefined4 * __fastcall FUN_0049bf10(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004d4618;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  param_1[1] = &PTR_FUN_004e4158;
  param_1[0x10] = 0x10;
  param_1[0xf] = FUN_0043f660;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  param_1[3] = 0;
  param_1[2] = 0;
  param_1[6] = 0;
  param_1[5] = 0;
  *(undefined1 *)((int)param_1 + 0x11) = 0;
  *(undefined1 *)(param_1 + 4) = 0;
  param_1[7] = 0;
  local_4 = 0;
  *param_1 = &PTR_LAB_004e4124;
  param_1[1] = &PTR_LAB_004e4118;
  _eh_vector_constructor_iterator_
            (param_1 + 0x12,0x48,9,(_func_void_void_ptr *)&LAB_0049bb90,FUN_0042b400);
  param_1[0x11] = 0x20;
  ExceptionList = local_c;
  return param_1;
}

