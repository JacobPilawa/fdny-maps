/* Fresh Ghidra decompilation: AddMessageMappingRecord at 0x0049b4c0;
   FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

   The previous work has three exports of this shared message helper. This fresh view
   confirms the fixed-capacity mapping record and its dispatch callback.
   */

void __thiscall AddMessageMappingRecord(void *this,int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  int *piVar4;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [52];
  void *pvStack_14;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d4598;
  local_c = ExceptionList;
  iVar1 = *(int *)((int)this + 0x4c);
  if (iVar1 < 0x40) {
    pcVar3 = (char *)((int)this + iVar1 * 0x14 + 0x60);
    iVar2 = iVar1;
    do {
      if (*pcVar3 == '\0') {
        piVar4 = (int *)((int)this + (iVar2 * 5 + 0x14) * 4);
        if (piVar4 != (int *)0x0) goto LAB_0049b5a1;
        break;
      }
      iVar2 = iVar2 + 1;
      pcVar3 = pcVar3 + 0x14;
    } while (iVar2 < 0x40);
  }
  iVar2 = 0;
  if (0 < iVar1) {
    pcVar3 = (char *)((int)this + 0x60);
    do {
      if (*pcVar3 == '\0') {
        piVar4 = (int *)((int)this + (iVar2 * 5 + 0x14) * 4);
        if (piVar4 != (int *)0x0) {
LAB_0049b5a1:
          ExceptionList = &local_c;
          *(int *)((int)this + 0x4c) = iVar2;
          *piVar4 = param_1;
          piVar4[1] = param_2;
          piVar4[2] = param_3;
          *(undefined1 *)(piVar4 + 4) = 1;
          piVar4[3] = param_4;
          (**(code **)(*(int *)this + 4))(piVar4,0);
          ExceptionList = pvStack_14;
          return;
        }
        break;
      }
      iVar2 = iVar2 + 1;
      pcVar3 = pcVar3 + 0x14;
    } while (iVar2 < *(int *)((int)this + 0x4c));
  }
  local_64 = &PTR_FUN_004dc554;
  ExceptionList = &local_c;
  FUN_004a3118();
  local_60 = "ROC\\Message Mapping\\mmap.cpp";
  local_5c = 0x41;
  local_58 = 0;
  local_4 = 0;
  FUN_004a3a19(local_48,"ROCMessageMapper: No free messages!");
  FUN_00401360(&local_64,0);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_64);
  ExceptionList = local_c;
  return;
}
