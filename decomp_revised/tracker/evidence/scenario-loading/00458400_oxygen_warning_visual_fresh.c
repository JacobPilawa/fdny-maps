/*
 * Source excerpt from FUN_00458400 in the pinned
 * inputs/original/binaries/FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 *
 * Read-only Ghidra decompilation, refreshed 2026-10-15. The constructor at
 * 004576B0 builds two resource-12 gauge faces tinted (1,1,1) and (1,.25,0).
 * Constants resolved from the executable: 004E2180=.25f, 004E0510=4.0f,
 * 004DFC24=1.0f, 004E0EB8=.9, and 004DF7B8=.1.
 */

if (0.25f <= displayedOxygen) {
    status->warningLatch = 0;
    setEffect(status->warningController, 0);
} else {
    const float belowThreshold = 0.25f - displayedOxygen;
    status->warningLatch = 1;
    status->warningController->field_1c =
        (1.0f - belowThreshold * 4.0f) * 0.9f + 0.1f;
}

/* Independent chatter gate in the same updater:
 * reset at displayedOxygen >= .5; enqueue RC_OxygenLow once below .25.
 */
