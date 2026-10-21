/* Fresh read-only Ghidra decompilation from FDNYFirefighter.exe SHA-256
   4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.
   Reached through VictimRescue's embedded updater vtable at root +0xD4. */

/* State 5 returns without the common child/audio update; state 6 advances its
   release animation, and state 7 enters the roam movement implementation. */
void __fastcall UpdateVictimActorThinkDispatch(int param_1)
{
  int state;

  state = *(int *)(param_1 + 0x4c);
  if (state == 5) {
    return;
  }
  if (state == 6) {
    FUN_004599b0(param_1 - 0xd4);
  }
  else if (state == 7) {
    UpdateVictimRoamMovement((int *)(param_1 - 0xd4));
  }
  if ((*(int *)(param_1 + 0x4c) != 6) && (*(int *)(param_1 + 0x80) != 0)) {
    FUN_0048d8f0(*(int *)(param_1 + 0x80));
  }
  FUN_00480d50(param_1 + 0x9c);
}
