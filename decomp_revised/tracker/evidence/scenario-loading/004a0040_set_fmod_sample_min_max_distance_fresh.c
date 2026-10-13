/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Entry: 004A0040
 * Function: SetFMODSampleMinMaxDistance
 * Analysis output, not recovered original source.
 */

/* Algorithm: For a non-stream sound resource, call FSOUND_Sample_SetMinMaxDistance with the sample
   handle and the two supplied numeric distances, in that order.
   Parameters: this is the sample resource; param_1 is minimum distance and param_2 is maximum
   distance.
   Returns: none.
   Evidence: Fresh pseudocode; ParseSoundBufferDefinition forwards the first and second
   ROC_SOUND_BUFFER3D numeric tokens here.
   Limits: This proves configured values, not the audible curve on a specific audio backend. */

void __thiscall SetFMODSampleMinMaxDistance(void *this,undefined4 param_1,undefined4 param_2)

{
  if (*(char *)((int)this + 8) == '\0') {
    _FSOUND_Sample_SetMinMaxDistance_12(*(undefined4 *)((int)this + 4),param_1,param_2);
  }
  return;
}
