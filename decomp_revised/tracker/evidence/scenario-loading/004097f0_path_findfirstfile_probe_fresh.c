
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

bool FUN_004097f0(int *param_1)

{
  int iVar1;
  int *piVar2;
  HANDLE hFindFile;
  uint unaff_retaddr;
  _WIN32_FIND_DATAA local_144;
  uint local_4;

  piVar2 = param_1;
  local_4 = DAT_005007e0 ^ unaff_retaddr;
  hFindFile = FindFirstFileA((LPCSTR)(param_1 + 3),&local_144);
  FindClose(hFindFile);
  if ((piVar2 != &DAT_00501908) && (iVar1 = *piVar2, *piVar2 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&param_1);
  }
  return hFindFile != (HANDLE)0xffffffff;
}
