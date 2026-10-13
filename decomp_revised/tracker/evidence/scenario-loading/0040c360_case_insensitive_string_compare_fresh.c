/* Fresh Ghidra decompilation: CompareStringObjectCaseInsensitive at 0x0040c360; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */


/* CompareStringObjectCaseInsensitive */

bool __thiscall CompareStringObjectCaseInsensitive(void *this,char *param_1)

{
  int iVar1;

  iVar1 = __stricmp((char *)(**(int **)this + 0xc),param_1);
  return (bool)('\x01' - (iVar1 != 0));
}
