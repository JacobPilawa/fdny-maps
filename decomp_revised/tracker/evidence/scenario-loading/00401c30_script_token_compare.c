/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */

undefined4 __thiscall FUN_00401c30(void *this,byte *param_1)

{
  byte bVar1;
  byte *pbVar2;
  int iVar3;
  bool bVar4;
  
  pbVar2 = (byte *)(*(int *)this + 0xc);
  while( true ) {
    bVar1 = *pbVar2;
    bVar4 = bVar1 < *param_1;
    if (bVar1 != *param_1) break;
    if (bVar1 == 0) {
      return 0;
    }
    bVar1 = pbVar2[1];
    bVar4 = bVar1 < param_1[1];
    if (bVar1 != param_1[1]) break;
    pbVar2 = pbVar2 + 2;
    param_1 = param_1 + 2;
    if (bVar1 == 0) {
      return 0;
    }
  }
  iVar3 = (1 - (uint)bVar4) - (uint)(bVar4 != 0);
  return CONCAT31((int3)((uint)iVar3 >> 8),iVar3 != 0);
}
