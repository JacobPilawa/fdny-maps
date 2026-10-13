/* Ghidra decompilation, FDNYFirefighter.exe SHA-256:
 * 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 */
/* WARNING: Function: __chkstk replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int __fastcall FUN_0049afe0(undefined4 *param_1)

{
  int *piVar1;
  undefined4 *puVar2;
  char *pcVar3;
  char cVar4;
  int iVar5;
  char *pcVar6;
  uint unaff_retaddr;
  char *in_stack_ffffe048;
  undefined4 in_stack_ffffe058;
  char *in_stack_ffffe060;
  undefined4 uStack_1f90;
  char acStack_1f8c [4096];
  char acStack_f8c [3976];
  uint local_4;

  local_4 = DAT_005007e0 ^ unaff_retaddr;
  piVar1 = param_1 + 1;
  puVar2 = param_1 + 0x1c;
  pcVar3 = (char *)(param_1 + 3);
  iVar5 = FUN_0049a5f0((FILE *)*param_1,pcVar3,puVar2,piVar1,0,*(char *)(param_1 + 0x1d));
  if (iVar5 == 2) {
    ConvertX87FloatToIntegerTowardZero();
    iVar5 = FUN_0049a5f0((FILE *)*param_1,pcVar3,puVar2,piVar1,0,*(char *)(param_1 + 0x1d));
    if (iVar5 == 0x1a) {
      FUN_0049a5f0((FILE *)*param_1,pcVar3,puVar2,piVar1,1,'"');
      pcVar6 = pcVar3;
      do {
        cVar4 = *pcVar6;
        pcVar6[(int)(&stack0xffffe044 + -(int)pcVar3)] = cVar4;
        pcVar6 = pcVar6 + 1;
      } while (cVar4 != '\0');
      iVar5 = FUN_0049a5f0((FILE *)*param_1,pcVar3,puVar2,piVar1,0,*(char *)(param_1 + 0x1d));
      if (iVar5 == 0x1a) {
        FUN_0049a5f0((FILE *)*param_1,pcVar3,puVar2,piVar1,1,'"');
        pcVar6 = pcVar3;
        do {
          cVar4 = *pcVar6;
          pcVar6[(int)(acStack_f8c + -(int)pcVar3)] = cVar4;
          pcVar6 = pcVar6 + 1;
        } while (cVar4 != '\0');
        FUN_00495040(*(void **)(DAT_00503a78 + 4),uStack_1f90,acStack_1f8c,acStack_f8c,
                     in_stack_ffffe060);
        iVar5 = FUN_0049a5f0((FILE *)*param_1,pcVar3,puVar2,piVar1,0,*(char *)(param_1 + 0x1d));
        return -(uint)(iVar5 != 0x2a);
      }
      FUN_00495040(*(void **)(DAT_00503a78 + 4),in_stack_ffffe058,&stack0xffffe05c,0,
                   in_stack_ffffe048);
      return 0;
    }
  }
  return -1;
}
