/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00450440
 * Match: function entry
 * Entry: 00450440 FUN_00450440
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __thiscall FUN_00450440(void *this,int param_1)

{
  if (*(int *)((int)this + 0x3534) == 0) {
    *(int *)((int)this + 0x3534) = param_1;
    FUN_0045bef0(param_1);
    FUN_0044fce0('\x01');
    FUN_00490e60((void *)((int)this + 0x198),
                 (float)(int)(((*(int *)((int)this + 0x18c) != 4) - 1 & 0xffffffe2) + 0x32));
    FUN_004571a0((void *)((int)this + 0x5f4),(int *)0x1,(undefined1 *)0x0);
    FUN_00484260((undefined4 *)((int)this + 0x5d8));
    *(undefined4 *)((int)this + 0x5c4) = DAT_00502afc;
  }
  return;
}


