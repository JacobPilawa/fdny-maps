/*
 * Hash-pinned read-only Ghidra 12.0.4 decompilation.
 * Retail input: inputs/original/binaries/FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Saved project: .local/mission-lifecycle-ghidra/FDNYPort
 * Direct caller in the current program: 0044BCE0 at 0044BD23.
 */

/* Ghidra's return represents whether the requested hull query hit contents
 * mask 0x43. The Player helper uses true as "remain crouched" and false as
 * "restore standing state". This wrapper copies three vectors, chooses a
 * Genesis world pointer through the caller object's fields, and calls
 * geWorld_GetContents with mode 2.
 */
bool __thiscall FUN_00492df0(void *this,
                             undefined4 *origin,
                             undefined4 *mins,
                             undefined4 *maxs)
{
  int result;
  undefined4 queryOrigin[3];
  undefined4 queryMins[3];
  undefined4 queryMaxs[3];
  undefined1 contentsResult[12];
  byte contentsFlags;
  int worldSource;

  queryOrigin[0] = origin[0];
  queryOrigin[1] = origin[1];
  queryOrigin[2] = origin[2];
  queryMins[0] = mins[0];
  queryMins[1] = mins[1];
  queryMins[2] = mins[2];
  queryMaxs[0] = maxs[0];
  queryMaxs[1] = maxs[1];
  queryMaxs[2] = maxs[2];

  worldSource = (int)this + 0xb4;
  if (*(char *)((int)this + 0x160) == '\0') {
    worldSource = (int)this + 8;
  }

  result = geWorld_GetContents(*(undefined4 *)(worldSource + 0x78),
                               queryOrigin,
                               queryMins,
                               queryMaxs,
                               2,
                               0,
                               0,
                               0,
                               contentsResult);
  if (result == 0) {
    return false;
  }
  return (contentsFlags & 0x43) != 0;
}
