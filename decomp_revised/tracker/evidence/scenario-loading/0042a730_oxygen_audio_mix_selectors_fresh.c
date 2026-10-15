/* Fresh read-only Ghidra decompilation excerpt.
 * Retail file: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Function: 0042A730, global audio-manager selector dispatch.
 */

/* selector 4: oxygen visor enabled */
if (selector == 4) {
    *(uint8_t *)((int)this + 0x6A8) = 1;
    FUN_004A07C0(soundManager, 2, (int)(*(float *)0x004DF7B0 * *(float *)0x004FF488));
    SetFMODChannelVolume((int *)((int)this + 0x690));
    return;
}

/* selector 5: oxygen visor disabled */
if (selector == 5) {
    *(uint8_t *)((int)this + 0x6A8) = 0;
    FUN_004A07C0(soundManager, 1, (int)*(float *)0x004FF488);
    FUN_004A07C0(soundManager, 2, (int)*(float *)0x004FF488);
    SetFMODChannelVolume((int *)((int)this + 0x690));
    return;
}

/* 004DF7B0 = 0.3f; 004FF488 = 1.0f; 004FF48C = 1.0f.
 * FUN_004A07C0 stores a category gain and updates registered sounds whose
 * instance +0x14 category matches. The +0x690 streamed-music channel is separate.
 */
