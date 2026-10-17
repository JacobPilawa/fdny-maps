/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __fastcall FUN_00435780(int param_1)

{
  undefined4 uStack00000004;
  
  RequestSoundInstancePlayback
            ((void *)(*(int *)(*(int *)(param_1 + 0x18) + 0x28) + 0x208),(undefined1 *)0x0);
  uStack00000004 = *(undefined4 *)(param_1 + 4);
                    /* WARNING: Could not recover jumptable at 0x004357a3. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)(param_1 + 0x18) + 4))();
  return;
}

 (GhidraScript)
