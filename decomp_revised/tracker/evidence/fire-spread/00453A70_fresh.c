/* Current Ghidra output; FDNYFirefighter.exe SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* Algorithm: Player vtable data at 0x004E1C50 points here. If Player +0x3438 is zero, dispatch
   message 30000 to 0x0044BC00, message 0x7531 to ApplyPlayerFireExposure, or message 0x7532 to
   0x00450440. For 0x7531 the payload contains a FireRescue pointer and influence; the handler reads
   the source point from its visual subobject at +0x12C.
   
   Parameters: this is the Player; messageId is the message number; payload is the message's word
   array.
   
   Returns: void; routes the supported messages to their handlers. */

void __thiscall DispatchPlayerMessage(void *this,int messageId,int *payload)

{
  float *fireSource;
  float exposureInfluence;
  undefined4 auStack_c [3];
  
  if (*(int *)((int)this + 0x3438) == 0) {
    if (messageId == 30000) {
      FUN_0044bc00((int)this);
    }
    else {
      if (messageId == 0x7531) {
        exposureInfluence = (float)payload[1];
        fireSource = (float *)FUN_00447870((void *)(*payload + 300),auStack_c);
        ApplyPlayerFireExposure(this,fireSource,exposureInfluence);
        return;
      }
      if (messageId == 0x7532) {
        FUN_00450440(this,(int)payload);
        return;
      }
    }
  }
  return;
}


