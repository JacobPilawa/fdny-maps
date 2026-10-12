
/* WARNING: Function: __SEH_prolog replaced with injection: SEH_prolog */
/* WARNING: Function: __chkstk replaced with injection: alloca_probe */
/* WARNING: Function: __SEH_epilog replaced with injection: EH_epilog3 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

UINT entry(void)

{
  HMODULE pHVar1;
  int iVar2;
  UINT UVar3;
  int *piVar4;
  int extraout_ECX;
  _OSVERSIONINFOA local_114;
  _STARTUPINFOA local_68;
  int local_24;
  uint local_20;
  undefined1 *local_1c;
  undefined4 uStack_c;
  undefined *local_8;
  
  local_8 = &DAT_004ea660;
  uStack_c = 0x4ba0d8;
  local_114.szCSDVersion[0x7c] = -0x1c;
  local_114.szCSDVersion[0x7d] = -0x60;
  local_114.szCSDVersion[0x7e] = 'K';
  local_114.szCSDVersion[0x7f] = '\0';
  local_1c = (undefined1 *)&local_114;
  local_114.dwOSVersionInfoSize = 0x94;
  GetVersionExA(&local_114);
  DAT_00503cc8 = local_114.dwPlatformId;
  DAT_00503cd4 = local_114.dwMajorVersion;
  _DAT_00503cd8 = local_114.dwMinorVersion;
  _DAT_00503ccc = local_114.dwBuildNumber & 0x7fff;
  if (local_114.dwPlatformId != 2) {
    _DAT_00503ccc = _DAT_00503ccc | 0x8000;
  }
  _DAT_00503cd0 = local_114.dwMajorVersion * 0x100 + local_114.dwMinorVersion;
  pHVar1 = GetModuleHandleA((LPCSTR)0x0);
  if (((short)pHVar1->unused == 0x5a4d) &&
     (piVar4 = (int *)((int)&pHVar1->unused + pHVar1[0xf].unused), *piVar4 == 0x4550)) {
    if ((short)piVar4[6] == 0x10b) {
      if (0xe < (uint)piVar4[0x1d]) {
        iVar2 = piVar4[0x3a];
        goto LAB_004ba18d;
      }
    }
    else if (((short)piVar4[6] == 0x20b) && (0xe < (uint)piVar4[0x21])) {
      iVar2 = piVar4[0x3e];
LAB_004ba18d:
      local_20 = (uint)(iVar2 != 0);
      goto LAB_004ba193;
    }
  }
  local_20 = 0;
LAB_004ba193:
  iVar2 = __heap_init();
  if (iVar2 == 0) {
    if (DAT_00503d1c == 1) {
      __FF_MSGBANNER();
    }
    FUN_004c3d4d(0x1c);
    FUN_004b80e0(0xff);
  }
  FUN_004c02bc();
  local_8 = (undefined *)0x0;
  iVar2 = FUN_004ba89c();
  if (iVar2 < 0) {
    __amsg_exit(0x1b);
  }
  DAT_00505424 = GetCommandLineA();
  DAT_00503d14 = ___crtGetEnvironmentStringsA();
  iVar2 = FUN_004c436b(extraout_ECX);
  if (iVar2 < 0) {
    __amsg_exit(8);
  }
  iVar2 = __setenvp();
  if (iVar2 < 0) {
    __amsg_exit(9);
  }
  local_24 = FUN_004b8146();
  if (local_24 != 0) {
    __amsg_exit(local_24);
  }
  local_68.dwFlags = 0;
  GetStartupInfoA(&local_68);
  FUN_004c40cf();
  pHVar1 = GetModuleHandleA((LPCSTR)0x0);
  UVar3 = FUN_0042c800(pHVar1);
  if (local_20 == 0) {
    FUN_004b826d(UVar3);
  }
  FUN_004b828f();
  return UVar3;
}

