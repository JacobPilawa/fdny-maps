/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __fastcall FUN_004358d0(int param_1)

{
  char cVar1;
  
  cVar1 = FUN_0047dcd0(param_1 + 0x144);
  if (cVar1 == '\0') {
    FUN_0047dc90((void *)(param_1 + 0x144),'\x01');
  }
  FUN_0047d300(*(void **)(param_1 + 0x36c),0);
  *(undefined4 *)(param_1 + 0x390) = 0;
  *(undefined4 *)(param_1 + 0x38c) = 0x3f800000;
  return;
}

 (GhidraScript)
