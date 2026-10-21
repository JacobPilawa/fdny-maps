/* Fresh Ghidra decompilation: SelectVictimRoamAnimation at 0x00459a50; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */


/* Called by StartVictimRoam after at least one initial candidate passes.
   00480D00 initializes animation context +0x170 and stores geActor motion
   count at context +0x10 (VictimRescue +0x180). With six motions, this
   two-entry roam selector chooses motions 3-5 and may change movement scalar
   +0x228; with any other count, it chooses within that count at speed 0.2 and
   writes state 4. */

void __thiscall SelectVictimRoamAnimation(void *this,int param_1)

{
  uint uVar1;
  undefined4 uVar2;

  if (param_1 == 0) {
    FUN_00480d40((int)this + 0x170);
    return;
  }
  if (*(uint *)((int)this + 0x180) != 6) {
    uVar2 = 0x3e4ccccd;
    uVar1 = FUN_00404b30(*(uint *)((int)this + 0x180));
    FUN_00480ea0((void *)((int)this + 0x170),uVar1,uVar2);
    *(undefined4 *)((int)this + 0x120) = 4;
    return;
  }
  if (param_1 == 1) {
    uVar2 = 0x3f800000;
    uVar1 = FUN_00404b30(3);
    FUN_00480ea0((void *)((int)this + 0x170),uVar1,uVar2);
    return;
  }
  uVar1 = FUN_00404b30(3);
  if (uVar1 == 0) {
    *(undefined4 *)((int)this + 0x228) = 0x4249999a;
  }
  else if ((uVar1 == 1) || (uVar1 == 2)) {
    *(undefined4 *)((int)this + 0x228) = 0x4199999a;
  }
  FUN_00480ea0((void *)((int)this + 0x170),uVar1 + 3,0x3f800000);
  return;
}
