/* Current-hash Ghidra 12.0.4 decompilation excerpts, captured 2026-10-20.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Saved project FDNYPort, read-only with -noanalysis; no runtime capture.
 * These Ghidra excerpts are analysis output, not recovered source.
 */

/* 004571A0: only the Player action-status 3 and 4 branches are shown. */
if (param_1 == (int *)0x3) {
  FUN_00455bc0((void *)((int)this + 0xa94));
  return;
}
if (param_1 == (int *)0x4) {
  FUN_0047dc90((void *)((int)this + 0xa94),'\0');
  return;
}

/* 00455BC0: enables Player +0xA94, samples four values at time 0, and refreshes its GUI rectangle/frame-style record. No Axe ACT mesh consumer or GUI draw consumer for this specific record is established. */
void __fastcall FUN_00455bc0(void *param_1)
{
  int *piVar1;
  int iVar2;
  int local_10[4];

  FUN_0047dc90(param_1,'\x01');
  iVar2 = 0;
  *(undefined4 *)((int)param_1 + 0x124) = DAT_00502afc;
  piVar1 = FUN_00484940((void *)((int)param_1 + 0x128),local_10,0.0);
  FUN_0047edf0(param_1,piVar1,iVar2);
  return;
}

/* 0047DC90: update the ActorInfo enable byte and propagate to its child. */
void __thiscall FUN_0047dc90(void *this,char enabled)
{
  if (*(char *)((int)this + 0x101) != enabled) {
    *(char *)((int)this + 0x101) = enabled;
    if (*(void **)((int)this + 0x104) != (void *)0x0) {
      FUN_0047d300(*(void **)((int)this + 0x104),0);
      *(char *)(*(int *)((int)this + 0x104) + 0x30) = enabled;
    }
  }
  return;
}
