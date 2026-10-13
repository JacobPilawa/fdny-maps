
/* Algorithm:
   If the contact count at +0 is below 32, append a 16-byte record containing three contact-position
   dwords and the influence value. Resolve the target object and dispatch message 0x7530 with the
   new record, then increment the count.
   Parameters:
   this is the bounded contact-record collection; target is the recipient; contactPosition points to
   xyz; influence is stored as word four.
   Returns:
   void. Fresh xrefs are 0x0044619C, 0x0046229C, and 0x00464B60; their individual owner semantics
   remain under review. */

void __thiscall
AppendAndSendSprayContact(void *this,void *target,float *contactPosition,float influence)

{
  float *pfVar1;
  int *piVar2;
  undefined4 local_8 [2];
  
  if (*(int *)this < 0x20) {
    pfVar1 = (float *)(*(int *)this * 0x10 + 4 + (int)this);
    *pfVar1 = *contactPosition;
    pfVar1[1] = contactPosition[1];
    pfVar1[2] = contactPosition[2];
    *(float *)((*(int *)this + 1) * 0x10 + (int)this) = influence;
    piVar2 = (int *)FUN_0049d350(target,local_8);
    FUN_0049d4e0(*piVar2,piVar2[1],30000,*(int *)this * 0x10 + 4 + (int)this);
    *(int *)this = *(int *)this + 1;
  }
  return;
}

