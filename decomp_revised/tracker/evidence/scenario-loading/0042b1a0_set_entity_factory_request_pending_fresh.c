/* Fresh Ghidra 12.0.4 output from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e; captured 2026-10-13. */
/* Appends a non-null factory request to this service's singly linked queue. Each 0x0c-byte node
   stores the request pointer, next pointer, and one-byte flag; the service count at +0x18
   increases. The request is not constructed here. The service primary vtable at 0x004E4158 maps
   slot +0x04 to this routine. */

void __thiscall SetEntityFactoryRequestPending(void *this,int param_1,undefined1 param_2)

{
  int *piVar1;

  if (param_1 != 0) {
    *(int *)((int)this + 0x18) = *(int *)((int)this + 0x18) + 1;
    piVar1 = operator_new(0xc);
    piVar1[1] = 0;
    *piVar1 = param_1;
    *(undefined1 *)(piVar1 + 2) = param_2;
    if (*(int *)((int)this + 0x14) == 0) {
      *(int **)((int)this + 0x10) = piVar1;
      *(int **)((int)this + 0x14) = piVar1;
      return;
    }
    *(int **)(*(int *)((int)this + 0x14) + 4) = piVar1;
    *(int **)((int)this + 0x14) = piVar1;
  }
  return;
}
