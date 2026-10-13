/* Fresh Ghidra 12.0.4 output from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e; captured 2026-10-13. */
/* Processes the subsystem's pending factory queues. On every execution, its first queue-specific
   call dispatches through the nested service at this+0x3C0+0x40, whose vtable slot +0x04 resolves
   to ProcessEntitySpawnQueue (0x0049C670). ProcessEverGameUpdate calls this routine after handling
   a pending level-load request. This establishes per-update-invocation ordering, not rendered-frame
   cadence. */

void __fastcall ProcessEntityFactoryQueues(int param_1)

{
  (*(code *)**(undefined4 **)(param_1 + 8))(0,0);
  (*(code *)**(undefined4 **)(param_1 + 0x50))(0,0);
  (*(code *)**(undefined4 **)(param_1 + 0x98))(0,0);
  (*(code *)**(undefined4 **)(param_1 + 0xe0))(0,0);
  (**(code **)(*(int *)(*(int *)(param_1 + 0x3c0) + 0x40) + 4))(0,0);
  (**(code **)(*(int *)(*(int *)(param_1 + 0x3bc) + 0x40) + 4))(0,0);
  FUN_004a0660(*(int **)(param_1 + 0x3b0));
  (**(code **)(*(int *)(*(int *)(param_1 + 0x3c4) + 0x40) + 4))(0,0);
  return;
}
