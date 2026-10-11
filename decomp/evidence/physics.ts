/** Original Genesis3D units: positions/displacements in units, velocity in units/s. */
export type Vec3 = { x: number; y: number; z: number };

export type LocomotionState = "standing" | "crouched" | "climbing" | "spectator";

export interface MoveInput {
  forward: number;
  strafe: number;
  jumpHeld: boolean;
  jumpPressed?: boolean;
  crouchPressed?: boolean;
  /** Camera yaw/pitch in radians at the browser boundary. Retail stores degrees. */
  yaw: number;
  pitch: number;
}

export interface PlayerPhysics {
  position: Vec3;
  velocity: Vec3;
  displacement: Vec3;
  grounded: boolean;
  previousGrounded: boolean;
  state: LocomotionState;
  jumpCooldown: number;
}

export const FDNY = Object.freeze({
  gravity: -1000,
  jumpVelocity: 400,
  jumpCooldownSeconds: 0.5,
  standingSpeed: 400,
  crouchedSpeed: 260,
  spectatorSpeed: 800,
  groundDrag: 1000,
  climbDrag: 1000,
  groundedBlendFraction: 0.2,
  airborneBlendFraction: 0.95,
  blendReferenceHz: 27,
  maxDeltaTime: Math.fround(0.1),
  eyeStanding: 70,
  eyeCrouched: 10,
  hullMins: Object.freeze({ x: -30, y: -30, z: -30 }),
  hullMaxStanding: Object.freeze({ x: 30, y: 85, z: 30 }),
  hullMaxCrouched: Object.freeze({ x: 30, y: 25, z: 30 }),
});

export function makePlayer(position: Vec3): PlayerPhysics {
  return {
    position: { ...position }, velocity: { x: 0, y: 0, z: 0 },
    displacement: { x: 0, y: 0, z: 0 }, grounded: false,
    previousGrounded: false, state: "standing", jumpCooldown: 0,
  };
}

function moveTowardZero(value: number, amount: number): number {
  if (value > 0) return Math.max(0, value - amount);
  if (value < 0) return Math.min(0, value + amount);
  return 0;
}

/** Exact Vec3_DampToward decay used by the executable.
 * Damp_RateFromFraction(f,27) followed by Vec3_DampToward reduces to
 * currentWeight = f^(27*dt).
 */
export function dampToward(current: number, target: number, fraction: number, dt: number): number {
  const currentWeight = Math.pow(fraction, FDNY.blendReferenceHz * dt);
  return target * (1 - currentWeight) + current * currentWeight;
}

export function hullMax(state: LocomotionState): Vec3 {
  return state === "crouched" ? FDNY.hullMaxCrouched : FDNY.hullMaxStanding;
}

export function eyeHeight(state: LocomotionState): number {
  return state === "crouched" ? FDNY.eyeCrouched : FDNY.eyeStanding;
}

/** PlayerRescueEnt::Think through ModelEntity::IntegrateVelocity.
 * Collision and the final position commit are intentionally separate because
 * that is the ordering in the retail executable.
 */
export function integratePlayer(player: PlayerPhysics, input: MoveInput, rawDt: number, canStand: () => boolean = () => true): boolean {
  const dt = Math.min(rawDt, FDNY.maxDeltaTime);
  player.jumpCooldown = Math.max(0, player.jumpCooldown - dt);

  // Think queries CROUCH on the pressed edge, before choosing speed/drag.
  if (input.crouchPressed) {
    if (player.state === "crouched") {
      if (canStand()) player.state = "standing";
    } else if (player.state === "standing" && player.grounded) {
      // Verb_Crouch 0x0044BCB0 requires the current grounded byte.
      player.state = "crouched";
    }
  }

  const climbing = player.state === "climbing";
  const spectator = player.state === "spectator";
  const cap = spectator ? FDNY.spectatorSpeed : player.state === "crouched" ? FDNY.crouchedSpeed : FDNY.standingSpeed;
  const cy = Math.cos(input.yaw), sy = Math.sin(input.yaw);
  const cp = climbing || spectator ? Math.cos(input.pitch) : 1;
  const sp = climbing || spectator ? Math.sin(input.pitch) : 0;
  // Genesis3D is Y-up. Positive local forward is the camera's -Z axis.
  let wx = -sy * cp * input.forward + cy * input.strafe;
  let wy = sp * input.forward;
  let wz = -cy * cp * input.forward - sy * input.strafe;
  const length = Math.hypot(wx, wy, wz);
  // Think only normalizes input magnitudes GREATER THAN one. A lone strafe
  // has magnitude 0.75 and must retain it (target speed 300, not 400).
  if (length > 0) {
    const scale = cap / Math.max(1, length);
    wx *= scale; wy *= scale; wz *= scale;
  }

  if (length > 0) {
    const groundedAuthority = player.grounded || climbing || spectator;
    const fraction = groundedAuthority ? FDNY.groundedBlendFraction : FDNY.airborneBlendFraction;
    const savedY = player.velocity.y;
    player.velocity.x = dampToward(player.velocity.x, wx, fraction, dt);
    player.velocity.y = dampToward(player.velocity.y, wy, fraction, dt);
    player.velocity.z = dampToward(player.velocity.z, wz, fraction, dt);
    if (!climbing && !spectator) player.velocity.y = savedY;
  }

  const drag = climbing || spectator ? FDNY.climbDrag : player.grounded ? FDNY.groundDrag : 0;
  const ay = climbing || spectator ? 0 : FDNY.gravity;
  // Think chooses wish velocity, drag and gravity BEFORE the JUMP verb.
  if (input.jumpHeld) {
    if (spectator) player.state = "standing";
    if (player.state === "crouched") {
      if (input.jumpPressed && canStand()) player.state = "standing";
      // Even a blocked/held crouch-jump returns before integration and commit.
      player.displacement = { x: 0, y: 0, z: 0 };
      return false;
    }
    // 0x0044E990 checks +0x12c only, not previous grounded at +0x12d.
    if ((player.grounded || climbing) && player.jumpCooldown === 0) {
      if (climbing) { player.state = "standing"; player.velocity.y = FDNY.jumpVelocity; }
      else player.velocity.y += FDNY.jumpVelocity;
      player.jumpCooldown = FDNY.jumpCooldownSeconds;
    }
  }
  integrateVelocity(player, { x: 0, y: ay, z: 0 }, { x: drag, y: climbing || spectator ? drag : 0, z: drag }, dt);
  return true;
}

/** 0x0049DBC0 and its original vector helpers: float32 stores occur at
 * each multiplication/addition boundary. Verified against 1,000 executions
 * of the original integrator machine code, not a translated reference formula.
 */
export function integrateVelocity(player: PlayerPhysics, acceleration: Vec3, drag: Vec3, rawDt: number): void {
  const f = Math.fround, dt = f(rawDt), dtSquared = f(dt * dt);
  for (const axis of ["x", "y", "z"] as const) {
    const afterDrag = f(moveTowardZero(f(player.velocity[axis]), f(f(drag[axis]) * dt)));
    player.velocity[axis] = f(afterDrag + f(f(acceleration[axis]) * dt));
    player.displacement[axis] = f(f(player.velocity[axis] * dt) + f(f(0.5 * f(acceleration[axis])) * dtSquared));
  }
  player.previousGrounded = player.grounded;
  player.grounded = false;
}

export function commitDisplacement(player: PlayerPhysics): void {
  player.position.x = Math.fround(player.position.x + player.displacement.x);
  player.position.y = Math.fround(player.position.y + player.displacement.y);
  player.position.z = Math.fround(player.position.z + player.displacement.z);
}

/** 0x0049E040: last-resort response after five collision callbacks. */
export function applyCollisionResponse(player: PlayerPhysics, normal: Vec3): void {
  if (normal.y >= 0.5) { player.grounded = true; return; }
  player.displacement = { x: 0, y: 0, z: 0 };
  player.velocity = { x: 0, y: 0, z: 0 };
}
