/*
 * Fresh read-only Ghidra 12.0.4 decompilation from a temporary copy of the
 * saved FDNYPort project. Input: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Targets: 004A12F0 and 004A11A0. The retained analysis project was not
 * modified. See mission_outcome_status_dispatch_2026-10-19.md for the Player
 * status callers and authored resource names.
 */

/* 004A12F0, ConfigureAndRegisterSoundInstance: */
void __thiscall ConfigureAndRegisterSoundInstance(
    void *this, undefined4 *resource, undefined4 sourcePosition, undefined4 category)
{
  /* Reference-release/replacement omitted. */
  *(undefined4 *)((int)this + 0x10) = sourcePosition;
  *(undefined4 *)((int)this + 0x14) = category;
  FUN_004a0b10(*(void **)(DAT_00503a78 + 8), this);
}

/* 004576B0 constructs the PlayerStatus RescueLose instance through
 * 004A12F0(instance, resource("RescueLose"), 0, 1), so the port passes raw
 * category/group 1 to its owner-aware audio request. */

/* 004A11A0, RequestSoundInstancePlayback: */
void __thiscall RequestSoundInstancePlayback(void *this, undefined1 *suppressIfPlaying)
{
  if (suppressIfPlaying == (undefined1 *)1 && IsFMODChannelPlaying(this))
    return;
  FUN_004a0be0(*(void **)(DAT_00503a78 + 8), this, suppressIfPlaying);
}

/* Player status 00458400 calls this with NULL for RescueLose. That request is
 * always forwarded; the one-entry RC_ResLvlLose chatter path also reaches it
 * with NULL after selecting RC_ResLvlLose0. This records request semantics,
 * not audible playback or widget/result timing. */
