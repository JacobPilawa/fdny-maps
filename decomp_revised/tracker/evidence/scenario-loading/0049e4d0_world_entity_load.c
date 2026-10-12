
/* Fresh read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */

void __fastcall FUN_0049e4d0(int param_1)

{
  void *this;
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  void *this_00;
  int local_c4;
  undefined **local_bc;
  char *local_b8;
  undefined4 local_b4;
  undefined1 local_b0;
  undefined1 local_a0 [60];
  undefined **local_64;
  char *pcStack_60;
  undefined4 uStack_5c;
  undefined1 uStack_58;
  undefined1 auStack_48 [60];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d4821;
  pvStack_c = ExceptionList;
  local_bc = &PTR_FUN_004dc554;
  ExceptionList = &pvStack_c;
  FUN_004a3118();
  local_b8 = "ROC\\ROC AI\\Entity\\WorldEntity.cpp";
  local_b4 = 0x36;
  local_b0 = 0;
  local_4 = 0;
  FUN_004a3a19(local_a0,"ROCWorldEntity: Loading world from \"");
  FUN_004a3a19(local_a0,*(char **)(param_1 + 0x24));
  FUN_004a3a19(local_a0,"\".");
  FUN_00401360(&local_bc,0);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_bc);
  FUN_00495210(*(void **)(DAT_00503a78 + 4),*(char **)(param_1 + 0x24));
  local_c4 = 0;
  FUN_0049efe0(*(int *)(DAT_00503a78 + 0x14));
  iVar1 = FUN_0049f000(*(int *)(DAT_00503a78 + 0x14));
  while (iVar1 != 0) {
    this = *(void **)(DAT_00503a78 + 4);
    FUN_00492ea0(this,*(undefined4 *)(iVar1 + 8));
    puVar2 = (undefined4 *)FUN_00492a20((int)this);
    while (puVar2 != (undefined4 *)0x0) {
      puVar3 = operator_new(0x14);
      local_4 = 1;
      if (puVar3 == (undefined4 *)0x0) {
        this_00 = (void *)0x0;
      }
      else {
        this_00 = (void *)FUN_0049f230(puVar3);
      }
      local_4 = 0xffffffff;
      *(undefined4 *)((int)this_00 + 8) = *puVar2;
      *(int *)((int)this_00 + 0xc) = iVar1;
      FUN_0049d130(this_00,puVar2);
      (**(code **)(**(int **)(DAT_00503a78 + 0x24) + 4))(this_00,1);
      local_c4 = local_c4 + 1;
      puVar2 = (undefined4 *)FUN_00492a20((int)this);
    }
    iVar1 = FUN_0049f000(*(int *)(DAT_00503a78 + 0x14));
  }
  local_64 = &PTR_FUN_004dc554;
  FUN_004a3118();
  pcStack_60 = "ROC\\ROC AI\\Entity\\WorldEntity.cpp";
  uStack_5c = 0x4d;
  uStack_58 = 0;
  local_4 = 2;
  FUN_004a3a19(auStack_48,"ROCWorldEntity: ");
  FUN_004a3f7a(auStack_48,local_c4);
  FUN_004a3a19(auStack_48," entities added to spawn queue.");
  FUN_00401360(&local_64,0);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_64);
  ExceptionList = pvStack_c;
  return;
}

