/* Fresh Ghidra decompilation: HandleVictimPickupMessage at 0x00450440;
   FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

   The previous work has five exports of this same function, including the full Training
   pickup/rescue trace. This fresh export confirms the carry-empty gate and call order.
   */

void __thiscall HandleVictimPickupMessage(void *this,int param_1)

{
  if (*(int *)((int)this + 0x3534) == 0) {
    *(int *)((int)this + 0x3534) = param_1;
    BeginVictimPickup(param_1);
    FUN_0044fce0('\x01');
    FUN_00490e60((void *)((int)this + 0x198),
                 (float)(int)(((*(int *)((int)this + 0x18c) != 4) - 1 & 0xffffffe2) + 0x32));
    FUN_004571a0((void *)((int)this + 0x5f4),(int *)0x1,(undefined1 *)0x0);
    FUN_00484260((undefined4 *)((int)this + 0x5d8));
    *(undefined4 *)((int)this + 0x5c4) = DAT_00502afc;
  }
  return;
}
