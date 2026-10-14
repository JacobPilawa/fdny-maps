/* Fresh Ghidra decompilation: FDNYFirefighter.exe
   SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
   Ghidra address: 00483580. Sole retail caller: 0045B220 at 0045B385. */

float *__cdecl StepVectorTowardTarget(float *out, float *current,
                                      float *target, float movementScalar)
{
  float step;
  float delta[3];
  float normalizedDelta[3];

  FUN_0043b2d0(delta, target, current);
  step = DAT_00502b04 * movementScalar;

  /* The x87 sequence compares deltaX² + deltaY² + deltaZ² with step².
     FCOMP is strict; unordered and equal comparisons take the movement path. */
  if ((0.0f < step) &&
      (delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2] < step * step)) {
    out[0] = target[0];
    out[1] = target[1];
    out[2] = target[2];
    return out;
  }

  FUN_0045a230(delta, normalizedDelta);
  FUN_00447710(delta, step, normalizedDelta);
  FUN_0042d050(out, current, normalizedDelta);
  return out;
}

/* Caller 0045B220 passes the frame delta in DAT_00502B04 and the victim's
   movement scalar from +0x228. The caller cadence remains unverified. */
