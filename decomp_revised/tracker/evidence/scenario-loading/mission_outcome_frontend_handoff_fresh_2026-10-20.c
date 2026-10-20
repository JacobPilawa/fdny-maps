/*
 * Selected fresh Ghidra 12.0.4 decompilation and data reads, read-only.
 * Input: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 *
 * The result-widget caller itself is preserved in
 * mission_outcome_state_fresh_2026-10-19.c. This excerpt records its timed
 * completion handoff, the manager-state writer, and the quad rotation helper.
 */

/* 00458400 result-widget completion excerpt */
if (cVar2 == '\0') {
  FUN_004295b0(DAT_00502aac,
               *(int *)(*(int *)(param_1 + 4) + 0x3438) == 1);
}

/* 004295B0 */
void __thiscall FUN_004295b0(void *this, char param_1)
{
  if (*(int *)((int)this + 0x6e4) != 0) {
    *(uint *)((int)this + 0x6ec) = (param_1 == '\0') + 1;
  }
  return;
}

/* 0047E970: rotate a 2D point by param_2 degrees, preserving its Z value. */
void __thiscall FUN_0047e970(void *this, float *param_1, float param_2)
{
  float10 cosine = fcos((float10)param_2 * (float10)0.017453292f);
  float10 sine = fsin((float10)param_2 * (float10)0.017453292f);
  float x = *(float *)this;
  float y = *(float *)((int)this + 4);
  float z = *(float *)((int)this + 8);
  param_1[0] = (float)cosine * x - (float)sine * y;
  param_1[1] = (float)sine * x + (float)cosine * y;
  param_1[2] = z;
  return;
}

/* ReadFloatBits.java output for constants used by 0047E970 / 0048D1A0:
 * 004E22BC f32=0.017453292 bits32=0x3c8efa35
 * 004E3A28 f32=1800.0 bits32=0x44e10000
 */
