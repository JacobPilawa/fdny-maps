/* Fresh read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13.
 */

/* Summary: Parses a reference-counted string's text as a float.
   
   Algorithm: Copy the string payload into a temporary C string, try sscanf with `%f`, then `%g`;
   write the parsed float through param_1 and report whether parsing succeeded.
   
   Parameters: this is the source string object; param_1 points to a float output slot.
   
   Returns: true-like value on successful conversion, false-like value otherwise.
   
   Evidence: Fresh decompilation at 0x0040CC50; common ScriptActor `wait` handling uses it to parse
   its duration. */

undefined4 __thiscall ParseFloatFromString(void *this,undefined4 param_1)

{
  char cVar1;
  char *pcVar2;
  char *_Src;
  int iVar3;
  undefined4 extraout_EAX;
  uint extraout_EAX_00;
  char *pcVar4;
  
  pcVar4 = (char *)(*(int *)this + 0xc);
  pcVar2 = pcVar4;
  do {
    cVar1 = *pcVar2;
    pcVar2 = pcVar2 + 1;
  } while (cVar1 != '\0');
  _Src = _malloc((size_t)(pcVar2 + (1 - (*(int *)this + 0xd))));
  pcVar2 = _Src;
  do {
    cVar1 = *pcVar4;
    pcVar4 = pcVar4 + 1;
    *pcVar2 = cVar1;
    pcVar2 = pcVar2 + 1;
  } while (cVar1 != '\0');
  iVar3 = _sscanf(_Src,"%f",param_1);
  if ((iVar3 != 1) && (iVar3 = _sscanf(_Src,"%g",param_1), iVar3 != 1)) {
    _free(_Src);
    return extraout_EAX_00 & 0xffffff00;
  }
  _free(_Src);
  return CONCAT31((int3)((uint)extraout_EAX >> 8),1);
}
