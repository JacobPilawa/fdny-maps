/* Fresh Ghidra decompilation: GetScriptActorSubobject at 0x0049d760;
   FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

   This was previously exported in reference/previous-work/decomp/evidence/ghidra-refresh/
   rescue-flow-20261011/0049d760_FUN_0049d760.c. The old pseudocode correctly showed the
   +0x24 return; fresh analysis ties it to VictimRescue's vtable slot +0x18 and embedded
   ScriptActor subobject.
   */

void * __thiscall GetScriptActorSubobject(void *this)

{
  if (this != (void *)0x0) {
    return (void *)((int)this + 0x24);
  }
  return (void *)0x0;
}
