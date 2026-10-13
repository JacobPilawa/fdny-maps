
/* Algorithm:
   Reject messages when FireRescue mode +0x70 is 5. For message 0x7530, forward the four-float
   payload (contact xyz and influence) to ApplyFireSuppressionContact with finalize=0.
   Parameters:
   this is the FireRescue root; messageId is the message code; payload points to contact xyz
   followed by influence.
   Returns:
   void. Recovered from primary vtable slot +0x08 at 0x004E1788. */

void __thiscall DispatchFireRescueSprayMessage(void *this,int messageId,float *payload)

{
  if ((*(int *)((int)this + 0x70) != 5) && (messageId == 30000)) {
    ApplyFireSuppressionContact(this,*payload,payload[1],payload[2],payload[3],'\0');
  }
  return;
}

