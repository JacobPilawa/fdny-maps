/* Fresh Ghidra decompilation: SendMappedEntityMessage at 0x0049d4e0;
   FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

   The previous work has five exports of this wrapper. This fresh view confirms that
   message submissions use the active global Message Mapper.
   */

void SendMappedEntityMessage(int param_1,int param_2,int param_3,int param_4)

{
  AddMessageMappingRecord(*(void **)(DAT_00503a78 + 0x1c),param_1,param_2,param_3,param_4);
  return;
}
