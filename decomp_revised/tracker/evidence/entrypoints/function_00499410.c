
void __fastcall FUN_00499410(int param_1)

{
  char cVar1;
  BOOL BVar2;
  undefined *puVar3;
  tagMSG local_d8;
  undefined **local_bc;
  char *local_b8;
  undefined4 local_b4;
  undefined1 local_b0;
  undefined1 local_a0 [60];
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_c = ExceptionList;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d44e3;
  ExceptionList = &local_c;
  *(undefined1 *)(param_1 + 0x38) = 1;
  DAT_00503a74 = 1;
  cVar1 = *(char *)(param_1 + 0x38);
  while (cVar1 != '\0') {
    BVar2 = PeekMessageA(&local_d8,(HWND)0x0,0,0,0);
    if (BVar2 == 0) {
      if (*(int **)(param_1 + 0x40) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0x40) + 0x74))();
      }
      if (*(int **)(param_1 + 0x40) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0x40) + 0xa0))();
      }
    }
    else {
      BVar2 = GetMessageA(&local_d8,(HWND)0x0,0,0);
      if (BVar2 != 0) {
        if ((local_d8.message == 0x1c) && (local_d8.wParam == 0)) {
          local_bc = &PTR_FUN_004dc554;
          FUN_004a3118();
          local_b8 = "ROC\\Engine\\engine.cpp";
          local_b4 = 0x62;
          local_b0 = 0;
          local_4 = 0;
          FUN_004a3a19(local_a0,"Application lost focus");
          FUN_00401360(&local_bc,3);
          local_4 = 0xffffffff;
          FUN_004012c0(&local_bc);
          puVar3 = &DAT_00502ad8;
          if (DAT_00502af8 == '\0') {
            puVar3 = &DAT_00502ab8;
          }
          FUN_0042bf20((int)puVar3);
          geEngine_Activate(*(undefined4 *)(param_1 + 0x74),0);
          do {
            while (BVar2 = GetMessageA(&local_d8,(HWND)0x0,0,0), BVar2 == 0) {
              Sleep(500);
            }
            TranslateMessage(&local_d8);
            DispatchMessageA(&local_d8);
          } while ((local_d8.message != 0x1c) || (local_d8.wParam == 0));
          local_64 = &PTR_FUN_004dc554;
          FUN_004a3118();
          local_60 = "ROC\\Engine\\engine.cpp";
          local_5c = 0x72;
          local_58 = 0;
          local_4 = 1;
          FUN_004a3a19(local_48,"Application regained focus");
          FUN_00401360(&local_64,3);
          local_4 = 0xffffffff;
          FUN_004012c0(&local_64);
          puVar3 = &DAT_00502ad8;
          if (DAT_00502af8 == '\0') {
            puVar3 = &DAT_00502ab8;
          }
          FUN_0042bf40((int)puVar3);
          geEngine_Activate(*(undefined4 *)(param_1 + 0x74),1);
        }
        TranslateMessage(&local_d8);
        DispatchMessageA(&local_d8);
      }
    }
    cVar1 = *(char *)(param_1 + 0x38);
  }
  ExceptionList = local_c;
  return;
}

