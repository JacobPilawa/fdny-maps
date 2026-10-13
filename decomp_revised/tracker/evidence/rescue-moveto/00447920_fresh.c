/* Fresh Ghidra decompiler output after recovering the four-argument prototype.
   FDNYFirefighter.exe SHA-256:
   4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

   Important limitation: Ghidra's C-like output still omits the x87 use of
   `duration`. The direct instruction evidence is in
   `00447920_timing_key_offsets.asm`. */

int __thiscall SetFireRescueSizeMotion(void *this,int size_x,int size_y,int size_z,float duration)

{
  void *this_00;
  uint uVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  ulonglong uVar4;
  float local_c;
  float local_8;
  float local_4;
  
  this_00 = (void *)((int)this + 300);
  *(undefined4 *)((int)this + 0x70) = 0;
  *(undefined4 *)((int)this + 0x100) = 0;
  FUN_0044ad70((int)this_00);
  uVar1 = DAT_00502afc + 10;
  puVar2 = (undefined4 *)FUN_00447850(this_00,&local_c);
  AddFireRescueMotionKey(this_00,puVar2,uVar1);
  local_c = (float)size_x;
  local_8 = (float)size_y;
  local_4 = (float)size_z;
  uVar4 = FUN_004b9400();
  uVar3 = AddFireRescueMotionKey(this_00,&local_c,(uint)uVar4);
  return CONCAT31((int3)((uint)uVar3 >> 8),1);
}
