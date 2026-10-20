/* Fresh hash-pinned retail input trace; selected decompiler/assembly evidence. */
/* Program: FDNYFirefighter.exe; Ghidra 12.0.4 headless on a disposable copy. */
/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* FUN_00437f30 registers the DirectInput mouse axes as type-2 controls. */
FUN_00438bf0(inputMap, device, 2, "MOUSE_X", 0);
FUN_00438bf0(inputMap, device, 2, "MOUSE_Y", 1);

/* FUN_004395c0 creates and binds PlayerHose's named steering actions. */
FUN_00438d70(actionMap, "STEER_YAW", 2, 0);
FUN_00438d70(actionMap, "STEER_PITCH", 2, 0);
FUN_00438bf0(actionMap, owner, 2, "STEER_YAW", 0);
FUN_00438bf0(actionMap, owner, 2, "STEER_PITCH", 1);

/* FUN_00437ab0: mouse poll clears the relative accumulators each poll. */
mouse->xDelta = 0;                    /* owner + 0x08 */
mouse->yDelta = 0;                    /* owner + 0x0c */
GetDeviceData(mouseDevice, 0x14, events, &eventCount);
for (event = events; eventCount != 0; event += 5, --eventCount) {
    if (event->dwOfs == 0) mouse->xDelta += (int32_t)event->dwData;
    if (event->dwOfs == 4) mouse->yDelta += (int32_t)event->dwData;
}

/* Vtable 004e0a14 slot 3 -> 00437620, an unbounded axis accessor. */
/* At 00437620, selector 0 FILDs owner + 0x10; selector 1 FILDs + 0x14. */
/* The mouse subobject is offset from the DirectInput provider as passed to it. */

/* Vtable 004e0b44 slot 3 -> 00439310, an unbounded scaled axis wrapper. */
/* 004ff480 is initialized in the image to float32 0.10000000149011612. */
/* Fresh direct-data-xref scan found one read at 0043931e; no runtime option */
/* write was established, so 0.1 is the retail image default, not a proven */
/* immutable user setting. */
00439310: MOV EDX, [ESP + 0x4]
00439314: MOV EAX, [ECX + 0x8]
00439317: ADD ECX, 0x8
0043931a: PUSH EDX
0043931b: CALL [EAX + 0x0c]
0043931e: FMUL float ptr [0x004ff480]
00439324: FCHS
00439326: RET 0x4

/* FUN_004386f0 resolves a named axis action, invokes each bound provider */
/* and returns the sum of the bound control values. */
/* FUN_00466780 then subtracts STEER_PITCH from PlayerHose +0x230 and */
/* STEER_YAW from +0x234. With the wrapper's negative scale, positive raw */
/* X/Y counts therefore increase the pitch/yaw accumulators by 0.1 degrees. */
/* 00466780 clamps pitch to the authored symmetric N_PitchSpread and yaw to */
/* +/-60 degrees, constrains pitch to 10..170 degrees from world up, rotates */
/* the temporary aim frame by +pitch then -yaw, and leaves the entity frame */
/* used by 00465ef0 as the stream source frame. */

/* Static evidence only. The exact DirectInput poll/update cadence, sensitivity */
/* overrides, x87 intermediate rounding, SDL event timing, and rendered/live */
/* retail orientation have not been compared. */
