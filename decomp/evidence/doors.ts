import type { Vec3 } from "./physics.ts";
import type { AuthoredModelMotion } from "./model-motion.ts";

export interface DoorEntity {
  index: number; class: string; model?: number;
  bounds?: { mins: number[]; maxs: number[] };
  properties: Record<string, string>;
}

export type ApproachDoorSnapshot = {
  angle: number; direction: number; fraction: number; touching: boolean;
};

/** Training's authored motion has identity and +90-degree Y quaternion keys
 * at 0 and 1 seconds. *Door applies timescale 3 and openaway may reverse it.
 */
export class ApproachDoor {
  readonly entity: DoorEntity;
  readonly pivot: Vec3;
  readonly reach: number;
  readonly timeScale: number;
  /** Optional authored `ifclear Zone (...)` gate for approach opening. */
  readonly openIfClear?: string;
  angle = 0;
  private direction = 1;
  private fraction = 0;
  private touching = false;

  constructor(entity: DoorEntity, pivot: Vec3, timeScale = 3, openIfClear?: string) {
    if (!entity.bounds || entity.model === undefined) throw new Error("Door requires a brush and bounds.");
    this.entity = entity; this.pivot = pivot;
    this.reach = Number(entity.properties.E_Reach);
    if (!Number.isFinite(this.reach) || this.reach < 0) throw new Error("Invalid door reach.");
    this.timeScale = Number.isFinite(timeScale) && timeScale > 0 ? timeScale : 3;
    this.openIfClear = openIfClear;
  }

  reset(): void {
    this.direction = 1;
    this.fraction = 0;
    this.touching = false;
    this.angle = 0;
  }

  snapshot(): ApproachDoorSnapshot {
    return { angle: this.angle, direction: this.direction, fraction: this.fraction, touching: this.touching };
  }

  restore(snapshot: ApproachDoorSnapshot): void {
    if (![snapshot.angle, snapshot.direction, snapshot.fraction].every(Number.isFinite)
      || Math.abs(snapshot.direction) !== 1 || snapshot.fraction < 0 || snapshot.fraction > 1) {
      throw new RangeError("Invalid approach-door snapshot");
    }
    this.angle = snapshot.angle; this.direction = snapshot.direction;
    this.fraction = snapshot.fraction; this.touching = Boolean(snapshot.touching);
  }

  get isTouching(): boolean { return this.touching; }

  script(command: "open" | "close"): void {
    this.fraction = command === "open" ? 1 : 0;
    this.touching = command === "open";
    this.angle = command === "open" ? this.direction * Math.PI / 2 : 0;
  }

  update(position: Vec3, dt: number, allowOpen = true): { angle: number; solid: boolean } {
    const bounds = this.entity.bounds!;
    const distance = [position.x, position.y, position.z].reduce((sum, value, axis) => {
      const delta = value - Math.max(bounds.mins[axis], Math.min(bounds.maxs[axis], value));
      return sum + delta * delta;
    }, 0);
    const touching = distance <= this.reach * this.reach;
    const wantsOpen = touching && allowOpen;
    if (wantsOpen && !this.touching && this.fraction === 0) {
      const rx = (bounds.mins[0] + bounds.maxs[0]) / 2 - this.pivot.x;
      const rz = (bounds.mins[2] + bounds.maxs[2]) / 2 - this.pivot.z;
      // Positive Y rotation moves the panel center along (rz, 0, -rx).
      this.direction = rz * (position.x - this.pivot.x) - rx * (position.z - this.pivot.z) > 0 ? -1 : 1;
    }
    this.touching = wantsOpen;
    const advance = Math.fround(Math.fround(Math.max(0, dt)) * this.timeScale);
    // `untouch: ifclear Zone (*DoorClose)` leaves an already-open door in
    // place while that zone still contains live fire. The same gate therefore
    // controls closure when the player departs.
    const mayClose = !touching && (allowOpen || !this.openIfClear);
    const delta = wantsOpen ? advance : mayClose ? -advance : 0;
    this.fraction = Math.max(0, Math.min(1, Math.fround(this.fraction + delta)));
    // geQuaternion_SlerpNotShortest is the authored motion interpolator. For
    // the Training tracks (identity -> quarter-turn about Y), retain its
    // float32 stores and recover the matrix angle from the resulting quaternion
    // instead of assuming a linear Euler interpolation.
    const t = this.fraction;
    const c = Math.fround(0.707107); // exact extracted motion key
    const s = Math.fround(0.707107 * this.direction);
    const omega = Math.acos(c), sinOmega = Math.sin(omega);
    const scale0 = Math.fround(Math.sin((1 - t) * omega) / sinOmega);
    const scale1 = Math.fround(Math.sin(t * omega) / sinOmega);
    const qw = Math.fround(scale0 + Math.fround(scale1 * c));
    const qy = Math.fround(scale1 * s);
    this.angle = 2 * Math.atan2(qy, qw);
    return { angle: this.angle, solid: true };
  }
}

export function createApproachDoors(entities: DoorEntity[], motions?: AuthoredModelMotion[]): ApproachDoor[] {
  return entities.filter((entity) => entity.class === "RCS_BModel" && entity.bounds && entity.model !== undefined
    && Object.entries(entity.properties).some(([key, value]) => /^S_Script\d+$/.test(key) && (/^\*(?:Door(?:Open|Close)?|subdoor)$/i.test(value.trim()) || /\btouch:\s*(?:open|\*DoorOpen|ifclear\s+[^\s(]+\s*\(\s*\*DoorOpen)/i.test(value))))
    .filter((entity) => {
      if (!motions) return true;
      const motion = motions.find((candidate) => candidate.model === entity.model);
      const rotationKeys = motion?.rotation?.keys ?? [];
      if (!rotationKeys.length) return true;
      return rotationKeys.some((key) => Math.abs(key[2]) + Math.abs(key[3]) + Math.abs(key[4]) > 1e-4);
    })
    .map((entity) => {
      const model = entities.find((candidate) => candidate.class === "%Model%" && Number(candidate.properties.Model) === entity.model);
      const origin = model?.properties.origin?.trim().split(/\s+/).map(Number);
      if (!origin || origin.length !== 3 || !origin.every(Number.isFinite)) throw new Error(`Missing door hinge for model ${entity.model}.`);
      const authoredScale = Object.values(entity.properties).map((value) => value.match(/settimescale\s+([\d.]+)/i)?.[1]).find(Boolean);
      const isSubdoor = Object.values(entity.properties).some((value) => /^\*subdoor$/i.test(value.trim()));
      const timeScale = authoredScale ? Number(authoredScale) : isSubdoor ? 4 : 3;
      const clearMatch = Object.values(entity.properties).map((value) => value.match(/\bifclear\s+([^\s(]+)\s*\(\s*\*DoorOpen/i)).find(Boolean);
      return new ApproachDoor(entity, { x: origin[0], y: origin[1], z: origin[2] }, timeScale, clearMatch?.[1]);
    });
}
