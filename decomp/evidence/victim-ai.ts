import type { Vec3 } from "./physics.ts";

export type VictimRescueState = "downed" | "escaping" | "roaming" | "carried";

// VictimRescueEnt::TryRoam (0x0045C7F0) stores 0x42100000 before choosing a
// target: the authored rescue-bot walk speed is exactly 36 units/second.
export const RETAIL_ROAM_SPEED = 36;

// VictimRescueEnt::TryEscape (0x0045C7F0) probes these four offsets in order.
export const RETAIL_ESCAPE_OFFSETS: Readonly<Vec3[]> = Object.freeze([
  Object.freeze({ x: 5, y: 0, z: 5 }),
  Object.freeze({ x: 5, y: 0, z: -5 }),
  Object.freeze({ x: -5, y: 0, z: 5 }),
  Object.freeze({ x: -5, y: 0, z: -5 }),
]);

/** Return the first viable native escape probe, or null when the victim is stuck. */
export function chooseVictimEscape(canMove: (offset: Vec3) => boolean): Vec3 | null {
  for (const offset of RETAIL_ESCAPE_OFFSETS) if (canMove(offset)) return offset;
  return null;
}
