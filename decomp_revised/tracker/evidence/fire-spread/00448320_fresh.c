/* Current Ghidra output; FDNYFirefighter.exe SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm: Receives the rounded maximum-axis separation from DispatchFireRescueExposureUpdate. If
   the FireRescue eligibility field is set, update DAT_00502B90 when the stored +0x120 distance
   crosses the 750-unit boundary. Store the latest distance and call ProcessFireRescueSpread when
   the new distance is strictly below 750.
   
   Parameters: this is the FireRescue object; param_1 is the rounded proximity distance.
   
   Returns: void; updates proximity bookkeeping and may attempt randomized spread. */

void __thiscall UpdateFireRescueProximityAndSpread(void *this,int distance)

{
  if (*(int *)(*(int *)((int)this + 0x48) + 4) != 0) {
    if ((0x2ee < distance) || (*(float *)((int)this + 0x120) <= _DAT_004e16b0)) {
      if ((*(float *)((int)this + 0x120) < _DAT_004e16b0 !=
           (*(float *)((int)this + 0x120) == _DAT_004e16b0)) && (0x2ee < distance)) {
        DAT_00502b90 = DAT_00502b90 + -1;
      }
    }
    else {
      DAT_00502b90 = DAT_00502b90 + 1;
    }
  }
  *(float *)((int)this + 0x120) = (float)distance;
  if ((float)distance < _DAT_004e16b0) {
    ProcessFireRescueSpread(this);
  }
  return;
}


