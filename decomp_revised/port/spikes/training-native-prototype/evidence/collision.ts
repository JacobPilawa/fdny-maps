import type { Vec3 } from "./physics";

type Plane = { normal: [number, number, number]; distance: number; type: number };
type Node = { children: [number, number]; plane: number };
type Leaf = { contents: number; first_side: number; num_sides: number };
type LeafSide = { plane: number; side: number };
type Model = { index: number; root: number; mins: [number, number, number]; maxs: [number, number, number]; origin?: [number, number, number] };
export interface CollisionData { contents_mask: number; models: Model[]; nodes: Node[]; leafs: Leaf[]; leaf_sides: LeafSide[]; planes: Plane[] }
export interface TraceHit { fraction: number; point: Vec3; normal: Vec3; model: number; leaf: number }

const EPSILON = 0.1;
function component(v: Vec3 | [number, number, number], i: number): number {
  if (Array.isArray(v)) return v[i];
  return i === 0 ? v.x : i === 1 ? v.y : v.z;
}

export class GenesisBspCollision {
  // Genesis keeps the last committed contact even across unsuccessful queries.
  private retainedContact = { point: { x: 0, y: 0, z: 0 }, normal: { x: 0, y: 0, z: 0 }, leaf: 0 };
  private readonly data: CollisionData;
  private readonly retainContact: boolean;
  private readonly disabledModels: Set<number>;
  private readonly modelYRotations = new Map<number, number>();
  private readonly modelQuaternions = new Map<number, [number, number, number, number]>();
  private readonly modelTranslations = new Map<number, Vec3>();

  constructor(data: CollisionData, disabledModels: Iterable<number> = [], retainContact = false) {
    this.data = data;
    this.retainContact = retainContact;
    this.disabledModels = new Set(disabledModels);
  }

  /** Inspection object removal must update both rendering and collision. */
  setModelEnabled(model: number, enabled: boolean): void {
    if (enabled) this.disabledModels.delete(model);
    else this.disabledModels.add(model);
  }

  getDisabledModels(): number[] { return [...this.disabledModels].sort((a, b) => a - b); }

  /** Genesis3D transforms each moving model's sweep into model space while
   * retaining the caller's axis-aligned hull. */
  setModelRotationY(model: number, radians: number): void {
    if (radians === 0) this.modelYRotations.delete(model);
    else this.modelYRotations.set(model, Math.fround(radians));
  }

  /** Apply an authored moving-model translation in world units. */
  setModelTranslation(model: number, translation: Vec3): void {
    if (translation.x === 0 && translation.y === 0 && translation.z === 0) this.modelTranslations.delete(model);
    else this.modelTranslations.set(model, { x: Math.fround(translation.x), y: Math.fround(translation.y), z: Math.fround(translation.z) });
  }

  setModelRotationQuaternion(model: number, quaternion: [number, number, number, number]): void {
    const [x, y, z, w] = quaternion;
    if (Math.abs(x) + Math.abs(y) + Math.abs(z) < 1e-7 && Math.abs(w - 1) < 1e-7) this.modelQuaternions.delete(model);
    else this.modelQuaternions.set(model, [Math.fround(x), Math.fround(y), Math.fround(z), Math.fround(w)]);
  }

  isBoxInSolid(position: Vec3, mins: Vec3, maxs: Vec3): boolean {
    const boxMins = { x: position.x + mins.x, y: position.y + mins.y, z: position.z + mins.z };
    const boxMaxs = { x: position.x + maxs.x, y: position.y + maxs.y, z: position.z + maxs.z };
    for (const model of this.data.models) {
      if (this.disabledModels.has(model.index)) continue;
      const angle = this.modelYRotations.get(model.index) ?? 0, quaternion = this.modelQuaternions.get(model.index);
      const translation = this.modelTranslations.get(model.index);
      const worldBounds = transformedBounds(model, angle, translation, quaternion);
      if (!overlaps(boxMins, boxMaxs, worldBounds.mins, worldBounds.maxs)) continue;
      const localPosition = transformModelPoint({ x: position.x - (translation?.x ?? 0), y: position.y - (translation?.y ?? 0), z: position.z - (translation?.z ?? 0) }, model.origin, quaternion, angle, true);
      const localMins = { x: localPosition.x + mins.x, y: localPosition.y + mins.y, z: localPosition.z + mins.z };
      const localMaxs = { x: localPosition.x + maxs.x, y: localPosition.y + maxs.y, z: localPosition.z + maxs.z };
      const leaves = new Set<number>();
      this.visitLeaves(model.root, localMins, localMaxs, leaves);
      for (const leafIndex of leaves) if (this.pointInsideExpandedLeaf(leafIndex, localPosition, mins, maxs)) return true;
    }
    return false;
  }

  traceBox(start: Vec3, delta: Vec3, mins: Vec3, maxs: Vec3, ignoreWalkable = false): TraceHit | null {
    const end = { x: Math.fround(start.x + delta.x), y: Math.fround(start.y + delta.y), z: Math.fround(start.z + delta.z) };
    const moveMins: Vec3 = {
      x: Math.fround(Math.min(start.x + mins.x, end.x + mins.x) - 1),
      y: Math.fround(Math.min(start.y + mins.y, end.y + mins.y) - 1),
      z: Math.fround(Math.min(start.z + mins.z, end.z + mins.z) - 1),
    };
    const moveMaxs: Vec3 = {
      x: Math.fround(Math.max(start.x + maxs.x, end.x + maxs.x) + 1),
      y: Math.fround(Math.max(start.y + maxs.y, end.y + maxs.y) + 1),
      z: Math.fround(Math.max(start.z + maxs.z, end.z + maxs.z) + 1),
    };
    let best: TraceHit | null = null;
    let bestWorldDistance = 99999;
    for (const model of this.data.models) {
      if (this.disabledModels.has(model.index)) continue;
      const angle = this.modelYRotations.get(model.index) ?? 0, quaternion = this.modelQuaternions.get(model.index);
      const translation = this.modelTranslations.get(model.index);
      const worldBounds = transformedBounds(model, angle, translation, quaternion);
      if (!overlaps(moveMins, moveMaxs, worldBounds.mins, worldBounds.maxs)) continue;
      const localStart = transformModelPoint({ x: start.x - (translation?.x ?? 0), y: start.y - (translation?.y ?? 0), z: start.z - (translation?.z ?? 0) }, model.origin, quaternion, angle, true);
      const localEnd = transformModelPoint({ x: end.x - (translation?.x ?? 0), y: end.y - (translation?.y ?? 0), z: end.z - (translation?.z ?? 0) }, model.origin, quaternion, angle, true);
      const localMoveMins: Vec3 = {
        x: Math.fround(Math.min(localStart.x + mins.x, localEnd.x + mins.x) - 1),
        y: Math.fround(Math.min(localStart.y + mins.y, localEnd.y + mins.y) - 1),
        z: Math.fround(Math.min(localStart.z + mins.z, localEnd.z + mins.z) - 1),
      };
      const localMoveMaxs: Vec3 = {
        x: Math.fround(Math.max(localStart.x + maxs.x, localEnd.x + maxs.x) + 1),
        y: Math.fround(Math.max(localStart.y + maxs.y, localEnd.y + maxs.y) + 1),
        z: Math.fround(Math.max(localStart.z + maxs.z, localEnd.z + maxs.z) + 1),
      };
      const visited = new Set<number>();
      this.visitLeaves(model.root, localMoveMins, localMoveMaxs, visited);
      let modelBest: TraceHit | null = null;
      let modelBestDistance = 9999;
      let modelLeafHit = false;
      for (const leafIndex of visited) {
        const result = this.intersectLeaf(leafIndex, localStart, localEnd, mins, maxs, model.index, modelBestDistance);
        modelLeafHit ||= result.leafHit;
        const hit = result.hit;
        if (hit) {
          modelBestDistance = collisionDistanceF32(hit.point, localStart);
          modelBest = hit;
          this.retainedContact = { point: { ...hit.point }, normal: { ...hit.normal }, leaf: hit.leaf };
        }
      }
      if (modelLeafHit && this.retainContact) {
        const retained = this.retainedContact;
        modelBest = { fraction: 0, point: { ...retained.point }, normal: { ...retained.normal }, leaf: retained.leaf, model: model.index };
      }
      if (modelBest && (angle !== 0 || quaternion)) {
        modelBest.point = transformModelPoint(modelBest.point, model.origin, quaternion, angle, false);
        modelBest.normal = quaternion ? rotateQuaternion(modelBest.normal, quaternion) : rotateY(modelBest.normal, angle);
      }
      if (modelBest && translation) {
        modelBest.point.x += translation.x; modelBest.point.y += translation.y; modelBest.point.z += translation.z;
      }
      if (modelBest) {
        this.retainedContact = { point: { ...modelBest.point }, normal: { ...modelBest.normal }, leaf: modelBest.leaf };
        const distance = collisionDistanceExtended(modelBest.point, start);
        const length = collisionDistanceExtended(end, start);
        modelBest.fraction = length === 0 ? 0 : distance / length;
        if ((!ignoreWalkable || modelBest.normal.y < 0.5) && distance < bestWorldDistance) {
          bestWorldDistance = Math.fround(distance);
          best = modelBest;
        }
      }
    }
    return best;
  }

  private visitLeaves(index: number, mins: Vec3, maxs: Vec3, output: Set<number>): void {
    if (index < 0) { output.add(-(index + 1)); return; }
    const node = this.data.nodes[index];
    if (!node) return;
    const side = boxOnPlaneSide(mins, maxs, this.data.planes[node.plane]);
    if (side & 1) this.visitLeaves(node.children[0], mins, maxs, output);
    if (side & 2) this.visitLeaves(node.children[1], mins, maxs, output);
  }

  private intersectLeaf(leafIndex: number, start: Vec3, end: Vec3, mins: Vec3, maxs: Vec3, model: number, bestDistance: number): { hit: TraceHit | null; leafHit: boolean } {
    const leaf = this.data.leafs[leafIndex];
    if (!leaf || !(leaf.contents & this.data.contents_mask) || leaf.num_sides === 0) return { hit: null, leafHit: false };
    let hit: TraceHit | null = null;
    let leafHit = false;
    const recurse = (frontPoint: Vec3, backPoint: Vec3, sideIndex: number, possibleSide: boolean): boolean => {
      if (!possibleSide) return false;
      if (sideIndex >= leaf.num_sides) return true;
      const i = sideIndex;
      const side = this.data.leaf_sides[leaf.first_side + i];
      const source = this.data.planes[side.plane];
      const sign = side.side ? -1 : 1;
      const normal: Vec3 = {
        x: Math.fround(source.normal[0] * sign),
        y: Math.fround(source.normal[1] * sign),
        z: Math.fround(source.normal[2] * sign),
      };
      let distance = Math.fround(source.distance * sign);
      distance = Math.fround(distance - normal.x * (normal.x > 0 ? mins.x : maxs.x));
      distance = Math.fround(distance - normal.y * (normal.y > 0 ? mins.y : maxs.y));
      distance = Math.fround(distance - normal.z * (normal.z > 0 ? mins.z : maxs.z));
      // Retail's PLANE_ANY path evaluates Z*Z + Y*Y + X*X in x87 registers;
      // these inputs are float32, so the products and this sum are exact in a
      // JavaScript double for Training's coordinate range.
      const planeDistance = (point: Vec3) => normal.z * point.z + normal.y * point.y + normal.x * point.x - distance;
      const front = planeDistance(frontPoint), back = planeDistance(backPoint);
      if (front >= 0 && back >= 0) return recurse(frontPoint, backPoint, i + 1, false);
      if (front < 0 && back < 0) return recurse(frontPoint, backPoint, i + 1, true);
      const frontIsBackSide = front < 0;
      let ratio = (frontIsBackSide ? front + EPSILON : front - EPSILON) / (front - back);
      ratio = Math.max(0, Math.min(1, ratio));
      const intersection = {
        x: Math.fround(frontPoint.x + ratio * (backPoint.x - frontPoint.x)),
        y: Math.fround(frontPoint.y + ratio * (backPoint.y - frontPoint.y)),
        z: Math.fround(frontPoint.z + ratio * (backPoint.z - frontPoint.z)),
      };
      if (recurse(frontPoint, intersection, i + 1, frontIsBackSide)) {
        leafHit = true;
        return true;
      }
      if (recurse(intersection, backPoint, i + 1, !frontIsBackSide)) {
        if (!hit) {
          const distanceFromStart = collisionDistanceExtended(intersection, start);
          if (distanceFromStart < bestDistance) {
            const fullDistance = Math.hypot(end.x - start.x, end.y - start.y, end.z - start.z);
            hit = { fraction: fullDistance === 0 ? 0 : distanceFromStart / fullDistance, point: intersection, normal, model, leaf: leafIndex };
          }
        }
        leafHit = true;
        return true;
      }
      return false;
    };
    recurse(start, end, 0, true);
    return { hit, leafHit };
  }

  private pointInsideExpandedLeaf(leafIndex: number, point: Vec3, mins: Vec3, maxs: Vec3): boolean {
    const leaf = this.data.leafs[leafIndex];
    if (!leaf || !(leaf.contents & this.data.contents_mask) || leaf.num_sides === 0) return false;
    for (let i = 0; i < leaf.num_sides; i++) {
      const side = this.data.leaf_sides[leaf.first_side + i];
      const source = this.data.planes[side.plane];
      const sign = side.side ? -1 : 1;
      const normal = { x: source.normal[0] * sign, y: source.normal[1] * sign, z: source.normal[2] * sign };
      let distance = source.distance * sign;
      distance -= normal.x > 0 ? normal.x * mins.x : normal.x * maxs.x;
      distance -= normal.y > 0 ? normal.y * mins.y : normal.y * maxs.y;
      distance -= normal.z > 0 ? normal.z * mins.z : normal.z * maxs.z;
      if (normal.x * point.x + normal.y * point.y + normal.z * point.z - distance >= 0) return false;
    }
    return true;
  }
}

function rotateY(value: Vec3, angle: number): Vec3 {
  if (angle === 0) return { ...value };
  const c = Math.cos(angle), s = Math.sin(angle);
  return { x: c * value.x + s * value.z, y: value.y, z: -s * value.x + c * value.z };
}

function transformAboutY(value: Vec3, origin: [number, number, number] | undefined, angle: number): Vec3 {
  if (!origin || angle === 0) return { ...value };
  const rotated = rotateY({ x: value.x - origin[0], y: value.y - origin[1], z: value.z - origin[2] }, angle);
  return { x: rotated.x + origin[0], y: rotated.y + origin[1], z: rotated.z + origin[2] };
}

function rotateQuaternion(value: Vec3, q: [number, number, number, number]): Vec3 {
  const [x, y, z, w] = q;
  const tx = 2 * (y * value.z - z * value.y), ty = 2 * (z * value.x - x * value.z), tz = 2 * (x * value.y - y * value.x);
  return { x: value.x + w * tx + y * tz - z * ty, y: value.y + w * ty + z * tx - x * tz, z: value.z + w * tz + x * ty - y * tx };
}

function transformModelPoint(value: Vec3, origin: [number, number, number] | undefined, quaternion: [number, number, number, number] | undefined, angle: number, inverse: boolean): Vec3 {
  if (!origin) return { ...value };
  const local = { x: value.x - origin[0], y: value.y - origin[1], z: value.z - origin[2] };
  if (quaternion) {
    const q: [number, number, number, number] = inverse ? [-quaternion[0], -quaternion[1], -quaternion[2], quaternion[3]] : quaternion;
    const rotated = rotateQuaternion(local, q);
    return { x: rotated.x + origin[0], y: rotated.y + origin[1], z: rotated.z + origin[2] };
  }
  return transformAboutY(value, origin, inverse ? -angle : angle);
}

function transformedBounds(model: Model, angle: number, translation?: Vec3, quaternion?: [number, number, number, number]): { mins: [number, number, number]; maxs: [number, number, number] } {
  // A non-Y authored track can legitimately have a zero derived yaw. Keep
  // the quaternion path active in that case so bounds follow the actual
  // moving brush rather than silently falling back to its unrotated AABB.
  if (!model.origin || (angle === 0 && !quaternion)) {
    const t = translation ?? { x: 0, y: 0, z: 0 };
    return { mins: [model.mins[0] + t.x, model.mins[1] + t.y, model.mins[2] + t.z], maxs: [model.maxs[0] + t.x, model.maxs[1] + t.y, model.maxs[2] + t.z] };
  }
  const points: Vec3[] = [];
  for (const x of [model.mins[0], model.maxs[0]]) for (const y of [model.mins[1], model.maxs[1]]) for (const z of [model.mins[2], model.maxs[2]]) {
    const point = transformModelPoint({ x, y, z }, model.origin, quaternion, angle, false), t = translation ?? { x: 0, y: 0, z: 0 };
    points.push({ x: point.x + t.x, y: point.y + t.y, z: point.z + t.z });
  }
  return {
    mins: [Math.min(...points.map((p) => p.x)), Math.min(...points.map((p) => p.y)), Math.min(...points.map((p) => p.z))],
    maxs: [Math.max(...points.map((p) => p.x)), Math.max(...points.map((p) => p.y)), Math.max(...points.map((p) => p.z))],
  };
}

function overlaps(aMin: Vec3, aMax: Vec3, bMin: [number, number, number], bMax: [number, number, number]): boolean {
  return aMax.x >= bMin[0] && aMin.x <= bMax[0] && aMax.y >= bMin[1] && aMin.y <= bMax[1] && aMax.z >= bMin[2] && aMin.z <= bMax[2];
}

/** geVec3d_Length stores the Z+Y+X x87 dot/sqrt result to float32. */
function collisionDistanceF32(a: Vec3, b: Vec3): number {
  return Math.fround(collisionDistanceExtended(a, b));
}

function collisionDistanceExtended(a: Vec3, b: Vec3): number {
  const x = Math.fround(a.x - b.x), y = Math.fround(a.y - b.y), z = Math.fround(a.z - b.z);
  return Math.sqrt(z * z + y * y + x * x);
}

function boxOnPlaneSide(mins: Vec3, maxs: Vec3, plane: Plane): number {
  if (plane.type < 3) {
    let side = 0;
    if (component(maxs, plane.type) >= plane.distance) side |= 1;
    if (component(mins, plane.type) < plane.distance) side |= 2;
    return side;
  }
  let front = 0, back = 0;
  for (let i = 0; i < 3; i++) {
    const n = plane.normal[i];
    front += n * component(n < 0 ? mins : maxs, i);
    back += n * component(n < 0 ? maxs : mins, i);
  }
  let side = 0;
  if (front - plane.distance >= 0) side |= 1;
  if (back - plane.distance < 0) side |= 2;
  return side;
}
