import { GenesisBspCollision, type TraceHit } from "./collision.ts";
import { FDNY, applyCollisionResponse, commitDisplacement, hullMax, integratePlayer, type MoveInput, type PlayerPhysics, type Vec3 } from "./physics.ts";
import { addBinary, binary, float32Binary, multiplyBinary } from "./binary-float.ts";

export const COLLISION_RESPONSE = Object.freeze({ stepHeight: 16, stepNormalLimit: 0.9, attempts: 5 });
const add = (a: Vec3, b: Vec3): Vec3 => ({ x: Math.fround(a.x + b.x), y: Math.fround(a.y + b.y), z: Math.fround(a.z + b.z) });
const dot = (a: Vec3, b: Vec3): number => a.x * b.x + a.y * b.y + a.z * b.z;

function x87DotF32(a: Vec3, b: Vec3): number {
  let sum = multiplyBinary(binary(Math.fround(a.x)), binary(Math.fround(b.x)));
  sum = addBinary(sum, multiplyBinary(binary(Math.fround(a.y)), binary(Math.fround(b.y))));
  sum = addBinary(sum, multiplyBinary(binary(Math.fround(a.z)), binary(Math.fround(b.z))));
  return float32Binary(sum);
}

/** Original 0x0049E0A0 slide response. `baseAcceleration` remains unchanged
 * across contacts while `clippedAcceleration` accumulates plane projections.
 */
export function slideCollisionResponse(
  player: PlayerPhysics, normal: Vec3, baseAcceleration: Vec3,
  clippedAcceleration: Vec3, rawDt: number,
): void {
  const f = Math.fround;
  if (f(normal.y) >= 0.5) player.grounded = true;
  const inverse = { x: f(-normal.x), y: f(-normal.y), z: f(-normal.z) };
  const inwardVelocity = x87DotF32(player.velocity, inverse);
  if (inwardVelocity > 0) for (const axis of ["x", "y", "z"] as const) {
    player.velocity[axis] = f(f(player.velocity[axis]) - f(inwardVelocity * inverse[axis]));
  }
  const inwardAcceleration = x87DotF32(baseAcceleration, inverse);
  if (inwardAcceleration > 0) for (const axis of ["x", "y", "z"] as const) {
    clippedAcceleration[axis] = f(f(clippedAcceleration[axis]) - f(inwardAcceleration * inverse[axis]));
  }
  const dt = f(rawDt), dtSquared = f(dt * dt);
  for (const axis of ["x", "y", "z"] as const) {
    player.displacement[axis] = f(
      f(f(player.velocity[axis]) * dt)
      + f(f(0.5 * f(clippedAcceleration[axis])) * dtSquared),
    );
  }
}

/** Player vtable 0x004E1C48 + 0x38 -> 0x0043F810, NOT the empty
 * base callback. Step checks precede sliding (0x0049E0A0); the hard stop
 * (0x0049E040) is only the five-contact fallback in 0x0043F6D0.
 */
export function stepMovement(player: PlayerPhysics, input: MoveInput, dt: number, collision: GenesisBspCollision): TraceHit[] {
  dt = Math.min(dt, FDNY.maxDeltaTime);
  if (!integratePlayer(player, input, dt, () => !collision.isBoxInSolid(player.position, FDNY.hullMins, FDNY.hullMaxStanding))) return [];
  if (player.state === "spectator") { commitDisplacement(player); return []; }
  const hits = resolveMovementCollisions(player, dt, collision);
  commitDisplacement(player);
  return hits;
}

/** Original retry loop after integration and before the body position commit. */
export function resolveMovementCollisions(player: PlayerPhysics, dt: number, collision: Pick<GenesisBspCollision, "traceBox">): TraceHit[] {
  const gravity = player.state === "climbing" || player.state === "spectator" ? 0 : FDNY.gravity;
  const acceleration = { x: 0, y: gravity, z: 0 };
  const clippedAcceleration = { ...acceleration };
  const hits: TraceHit[] = [];
  let stepped = false;
  const trace = (start: Vec3, delta: Vec3) => collision.traceBox(start, delta, FDNY.hullMins, hullMax(player.state));
  for (let attempt = 0; attempt < COLLISION_RESPONSE.attempts; attempt++) {
    const hit = trace(player.position, player.displacement);
    if (!hit) break;
    hits.push(hit);
    let didStep = false;
    if (!stepped && hit.normal.y <= COLLISION_RESPONSE.stepNormalLimit) {
      const up = { x: 0, y: COLLISION_RESPONSE.stepHeight, z: 0 };
      const raised = add(player.position, up);
      const horizontal = { x: player.displacement.x, y: 0, z: player.displacement.z };
      if (!trace(player.position, up) && !trace(raised, horizontal)) {
        const landing = trace(add(raised, horizontal), { x: 0, y: -COLLISION_RESPONSE.stepHeight - 1, z: 0 });
        if (landing) {
          // Retail updates body and collision origins, then rechecks the
          // remaining displacement. Do not replace this with a teleport past a door.
          player.position = { ...landing.point };
          hits.push(landing);
          stepped = didStep = true;
        }
      }
    }
    if (!didStep) {
      slideCollisionResponse(player, hit.normal, acceleration, clippedAcceleration, dt);
    }
    if (attempt === COLLISION_RESPONSE.attempts - 1) {
      if (hit.normal.y >= 0.5) player.grounded = true;
      else applyCollisionResponse(player, hit.normal);
    }
  }
  return hits;
}

/** Verb_Climb 0x0044C0B0 zeros velocity on entry and rejects a carried object. */
export function enterClimb(player: PlayerPhysics, carryingObject = false): boolean {
  if (player.state === "climbing" || carryingObject) return false;
  player.state = "climbing";
  player.velocity = { x: 0, y: 0, z: 0 };
  return true;
}
