
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0042c800(HINSTANCE param_1)

{
  bool bVar1;
  undefined4 uVar2;
  int *piVar3;
  BOOL BVar4;
  HWND pHVar5;
  DWORD DVar6;
  wchar_t *pwVar7;
  undefined4 *puVar8;
  void *pvVar9;
  int iVar10;
  HCURSOR hCursor;
  HWND pHVar11;
  int *_Memory;
  char *pcVar12;
  char *pcVar13;
  UINT UVar14;
  int **ppiVar15;
  int *in_stack_ffffff24;
  void *in_stack_ffffff28;
  int *in_stack_ffffff2c;
  undefined1 local_c0 [60];
  undefined **local_84;
  char *local_80;
  undefined4 local_7c;
  undefined1 local_78;
  undefined1 local_68 [60];
  undefined1 *local_2c;
  undefined8 local_28;
  int *local_20;
  int *local_1c;
  int *local_18;
  undefined1 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_004cdc51;
  local_10 = ExceptionList;
  local_14 = &stack0xffffff18;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_20 = &DAT_00501908;
  ExceptionList = &local_10;
  FUN_00401d80(&local_20,"FDNYFirefighterAmericanHero");
  local_8 = 0;
  uVar2 = FUN_0042c5d0((int *)&local_20);
  local_8 = 0xffffffff;
  if ((local_20 != &DAT_00501908) && (iVar10 = *local_20, *local_20 = iVar10 + -1, iVar10 + -1 < 1))
  {
    FUN_004011c0(&local_20);
  }
  if ((char)uVar2 != '\0') {
    CoInitialize((LPVOID)0x0);
    piVar3 = FUN_0040a230(&local_1c,3);
    BVar4 = SetCurrentDirectoryA((LPCSTR)(*piVar3 + 0xc));
    if ((local_1c != &DAT_00501908) && (*local_1c = *local_1c + -1, *local_1c < 1)) {
      FUN_004011c0(&local_1c);
    }
    if (BVar4 == 0) {
      UVar14 = 0x10;
      pcVar13 = "Error";
      pcVar12 = "Invalid working directory";
      pHVar5 = GetDesktopWindow();
      MessageBoxA(pHVar5,pcVar12,pcVar13,UVar14);
      ExceptionList = local_10;
      return 1;
    }
    DVar6 = timeGetTime();
    FUN_00404b50(DVar6);
    FID_conflict___time32((__time32_t *)&local_28);
    FUN_004270a0();
    local_84 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_80 = "main\\main.cpp";
    local_7c = 0xc6;
    local_78 = 0;
    local_8 = 1;
    FUN_004a3a19(local_68,"FDNYFirefighter v2.03 Startup");
    FUN_00401360(&local_84,3);
    local_8 = 0xffffffff;
    FUN_004012c0(&local_84);
    local_84 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_80 = "main\\main.cpp";
    local_7c = 199;
    local_78 = 0;
    local_8 = 2;
    FUN_004a3a19(local_68,"  Current time is ");
    pwVar7 = FID_conflict___wctime(&local_28);
    FUN_004a3a19(local_68,(char *)pwVar7);
    FUN_00401360(&local_84,3);
    local_8 = 0xffffffff;
    FUN_004012c0(&local_84);
    local_84 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_80 = "main\\main.cpp";
    local_7c = 200;
    local_78 = 0;
    local_8 = 3;
    FUN_004a3a19(local_68,"  Compiled on ");
    FUN_004a3a19(local_68,"Mar  8 2002");
    FUN_004a3a19(local_68,"\n");
    FUN_00401360(&local_84,3);
    local_8 = 0xffffffff;
    FUN_004012c0(&local_84);
    local_18 = (int *)&stack0xffffff1c;
    FUN_0040a230((undefined4 *)&stack0xffffff1c,2);
    pcVar12 = (char *)FUN_0042c6e0(&local_18,in_stack_ffffff24);
    ppiVar15 = &local_20;
    piVar3 = (int *)&DAT_004dfb90;
    local_2c = &stack0xffffff14;
    local_8 = 4;
    FUN_0040a230((undefined4 *)&stack0xffffff14,0);
    puVar8 = FUN_0042c6e0(&local_1c,piVar3);
    local_8._0_1_ = 5;
    pvVar9 = FUN_00403e40(puVar8,ppiVar15,pcVar12);
    local_8._0_1_ = 6;
    piVar3 = FUN_004040e0(pvVar9,in_stack_ffffff28,in_stack_ffffff2c);
    local_8._0_1_ = 7;
    FUN_00402580((char *)(*piVar3 + 0xc));
    local_8._0_1_ = 6;
    if (local_20 != &DAT_00501908) {
      *local_20 = *local_20 + -1;
      if (*local_20 < 1) {
        FUN_004011c0(&local_20);
      }
      else {
        local_20 = &DAT_00501908;
      }
    }
    local_8._0_1_ = 5;
    if (local_28._4_4_ != &DAT_00501908) {
      *local_28._4_4_ = *local_28._4_4_ + -1;
      if (*local_28._4_4_ < 1) {
        FUN_004011c0((undefined4 *)((int)&local_28 + 4));
      }
      else {
        local_28._4_4_ = &DAT_00501908;
      }
    }
    local_8 = CONCAT31(local_8._1_3_,4);
    if (local_1c != &DAT_00501908) {
      *local_1c = *local_1c + -1;
      if (*local_1c < 1) {
        FUN_004011c0(&local_1c);
      }
      else {
        local_1c = &DAT_00501908;
      }
    }
    local_8 = 0xffffffff;
    if ((local_18 != &DAT_00501908) && (*local_18 = *local_18 + -1, *local_18 < 1)) {
      FUN_004011c0(&local_18);
    }
    local_8 = 8;
    SetEnvironmentVariableA("FX_GLIDE_NO_SPLASH","1");
    while (bVar1 = FUN_00480c20(), !bVar1) {
      UVar14 = 0x41;
      pcVar13 = "FDNY Firefighter: American Hero";
      pcVar12 = "Please insert the FDNY Firefighter CD";
      pHVar5 = GetDesktopWindow();
      iVar10 = MessageBoxA(pHVar5,pcVar12,pcVar13,UVar14);
      if (iVar10 != 1) {
        ExceptionList = local_10;
        return 0;
      }
      Sleep(900);
    }
    do {
      iVar10 = ShowCursor(0);
    } while (0 < iVar10);
    local_2c = operator_new(0x78);
    local_8._0_1_ = 9;
    if (local_2c == (void *)0x0) {
      local_1c = (int *)0x0;
    }
    else {
      local_1c = (int *)FUN_00498e90(local_2c,param_1,"FDNY Firefighter",1);
    }
    piVar3 = local_1c;
    local_8._0_1_ = 8;
    uVar2 = FUN_004998b0(local_1c);
    if ((char)uVar2 == '\0') {
      local_84 = &PTR_FUN_004dc554;
      FUN_004a3118();
      local_80 = "main\\main.cpp";
      local_7c = 0xe5;
      local_78 = 0;
      local_8._0_1_ = 10;
      FUN_004a3a19(local_68,"Failed to initialize main window");
      FUN_00401360(&local_84,1);
      local_8._0_1_ = 8;
      FUN_004012c0(&local_84);
    }
    hCursor = LoadCursorA(param_1,(LPCSTR)0x68);
    SetCursor(hCursor);
    if (DAT_00502a78 == '\0') {
      pHVar5 = GetForegroundWindow();
      FUN_0042c3f0((char *)piVar3[0x1d]);
      puVar8 = FUN_0040a230(&local_18,0);
      local_8._0_1_ = 0xb;
      pHVar11 = FUN_00403e40(puVar8,&param_1,"Data\\Movies\\ActiValue.avi");
      local_8._0_1_ = 0xc;
      FUN_00485650(pHVar5,pHVar11);
      local_8._0_1_ = 0xb;
      if (param_1 != (HINSTANCE)&DAT_00501908) {
        param_1->unused = param_1->unused + -1;
        if (param_1->unused < 1) {
          FUN_004011c0(&param_1);
        }
        else {
          param_1 = (HINSTANCE)&DAT_00501908;
        }
      }
      local_8._0_1_ = 8;
      if ((local_18 != &DAT_00501908) && (*local_18 = *local_18 + -1, *local_18 < 1)) {
        FUN_004011c0(&local_18);
      }
      puVar8 = FUN_0040a230(&local_18,0);
      local_8._0_1_ = 0xd;
      pHVar11 = FUN_00403e40(puVar8,&param_1,"Data\\Movies\\Mekada.avi");
      local_8._0_1_ = 0xe;
      FUN_00485650(pHVar5,pHVar11);
      local_8._0_1_ = 0xd;
      if (param_1 != (HINSTANCE)&DAT_00501908) {
        param_1->unused = param_1->unused + -1;
        if (param_1->unused < 1) {
          FUN_004011c0(&param_1);
        }
        else {
          param_1 = (HINSTANCE)&DAT_00501908;
        }
      }
      local_8._0_1_ = 8;
      piVar3 = local_1c;
      if ((local_18 != &DAT_00501908) &&
         (*local_18 = *local_18 + -1, piVar3 = local_1c, *local_18 < 1)) {
        FUN_004011c0(&local_18);
        piVar3 = local_1c;
      }
    }
    FUN_00499280();
    FUN_004992d0((int)piVar3);
    FUN_004262e0();
    param_1 = operator_new(0xb8);
    local_8._0_1_ = 0xf;
    if (param_1 == (HINSTANCE)0x0) {
      _Memory = (int *)0x0;
    }
    else {
      _Memory = FUN_0042bcb0(param_1);
    }
    local_8 = CONCAT31(local_8._1_3_,8);
    (**(code **)(*_Memory + 0xa4))();
    FUN_00499410((int)piVar3);
    pcVar12 = FUN_0049ffb0();
    FUN_0049fd80(pcVar12);
    FUN_0042bdf0(_Memory);
    _free(_Memory);
    if (piVar3 != (int *)0x0) {
      FUN_00499850((int)piVar3);
      _free(piVar3);
    }
    pvVar9 = DAT_00503a78;
    if (DAT_00503a78 != (void *)0x0) {
      FUN_00499ca0();
      _free(pvVar9);
    }
    DAT_00503a78 = (void *)0x0;
    FUN_004262e0();
    local_8 = 0xffffffff;
    FUN_004a3118();
    local_8 = 0x12;
    FUN_004a3a19(local_c0,"Game closed.");
    FUN_00401360(&stack0xffffff24,0);
    local_8 = 0xffffffff;
    FUN_004012c0((undefined4 *)&stack0xffffff24);
    CoUninitialize();
  }
  ExceptionList = local_10;
  return 0;
}

