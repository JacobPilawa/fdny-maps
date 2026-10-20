/* Fresh Ghidra 12.0.4 decompilation of throw-gate vector helpers.
 * Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured 2026-10-20 from the saved FDNYPort project using
 * -readOnly -noanalysis, JDK 21, and DecompileTargetAndRefs.java. The
 * decompilation is analysis output, not recovered original source.
 */

/* 0x0044BED0: selected endpoint from BModel root +0x220 and +0x1F0/+0x1FC. */
void __thiscall FUN_0044bed0(void *this, undefined4 *param_1)
{
  undefined4 *endpoint;

  if (*(char *)((int)this + 0x220) == '\0') {
    endpoint = (undefined4 *)((int)this + 0x1fc);
  }
  else {
    endpoint = (undefined4 *)((int)this + 0x1f0);
  }
  *param_1 = *endpoint;
  param_1[1] = endpoint[1];
  param_1[2] = endpoint[2];
  return;
}

/* 0x0043B2D0: component-wise first vector minus second vector. */
void __cdecl FUN_0043b2d0(float *result, float *first, float *second)
{
  float firstY;
  float secondY;
  float firstZ;
  float secondZ;

  firstY = first[1];
  secondY = second[1];
  firstZ = first[2];
  secondZ = second[2];
  *result = *first - *second;
  result[1] = firstY - secondY;
  result[2] = firstZ - secondZ;
  return;
}

/* 0x0049E770: copy the vector stored at frame offset +0x1C. */
void __thiscall FUN_0049e770(void *this, undefined4 *param_1)
{
  *param_1 = *(undefined4 *)((int)this + 0x1c);
  param_1[1] = *(undefined4 *)((int)this + 0x20);
  param_1[2] = *(undefined4 *)((int)this + 0x24);
  return;
}

/* Player update 0x00452570 at 0x00453930 selects the BModel endpoint, then
 * 0x0045393B subtracts the Player movement vector at +0x338C. The subsequent
 * 0x0045394E call copies movement-subobject +0x3364 = Player-root +0x34A4,
 * the Player view follower's vector at +0x1C. This is distinct from the
 * held-target writer's Player-root +0x24 frame, returned by the vtable slot
 * at root +0x18. Both values are passed to ComputeVectorAngleDegrees at
 * 0x00453964; see 00497aa0_compute_vector_angle_degrees_fresh.c.
 */
