import * as THREE from "three";
import "./style.css";
import { GenesisBspCollision, type CollisionData } from "./collision";
import { FdnyRandom } from "./fdny-random";
import { readSettings } from "./settings";
import { TrainingScripts } from "./training-scripts";
import { RetailFrameClock } from "./frame-clock";
import { FDNY, eyeHeight, makePlayer } from "./physics";
import { stepMovement, enterClimb } from "./movement";
import { consumeExtinguisher } from "./extinguisher";
import { applyFireSpray } from "./fire-suppression";
import { fireParticleInfluence, RETAIL_EXTINGUISHER_ORIGIN, RETAIL_HOSE_PARTICLE_RADIUS, RETAIL_TOOL_DIRECTION, sprayLinePoints } from "./fire-particle";
import { MovementRecorder } from "./movement-trace";
import { MouseLook } from "./look";
import { createApproachDoors, type ApproachDoor } from "./doors";
import { touchesVictim } from "./victim-contact";
import { AXE, AxeMechanics, axeSweep, type AxeContact } from "./axe";
import { degreesToRadians, headingToRadians, INSPECTION_APPROXIMATIONS as APPROX } from "./units";
import { snapVictimToFloor } from "./training-spawns";
import { createAnimatedActor, type ActorBody, type AnimatedActor } from "./actor";
import { playerTouchesTrigger } from "./trigger-contact";
import { chooseVictimEscape, RETAIL_ROAM_SPEED, type VictimRescueState } from "./victim-ai";
import { motionDuration, sampleModelMotion, type AuthoredModelMotion } from "./model-motion";

interface MeshGroup { model: number; texture: string; positions: number[]; uvs: number[] }
interface WorldMesh { groups: MeshGroup[] }
interface Bounds { mins: number[]; maxs: number[] }
interface Entity { index: number; class: string; properties: Record<string, string>; model?: number; bounds?: Bounds }
type Tool = "hands" | "axe" | "extinguisher" | "hose" | "goggles";
type Interactive = { entity: Entity; marker: THREE.Object3D; alive: boolean; progress: number; valve?: boolean; animationRate?: number; state?: VictimRescueState; roamRadius?: number; roamSpeed?: number; roamAnchor?: THREE.Vector3; roamTarget?: THREE.Vector3; motionName?: string; roamingMotion?: string; fireRadius?: number; baseFireRadius?: number; firePosition?: THREE.Vector3; authoredFirePosition?: THREE.Vector3; emitterSize?: number; baseEmitterSize?: number; emitterStrength?: number; authoredEmitterActive?: boolean };

const AVAILABLE_LEVELS = ["training", "mbase", "bank", "bar_r", "gas_r", "boat_a", "boat_b", "subway_a", "subway_b", "oil_rig_a", "oil_rig_b", "oil_rig_c", "oil_rig_d"] as const;
const rawLevel = new URLSearchParams(location.search).get("level")?.toLowerCase() || "";
const requestedLevel = (AVAILABLE_LEVELS as readonly string[]).includes(rawLevel) ? rawLevel : null;
const levelName = requestedLevel || "training";
// Keep asset URLs relative to the page hosting the chooser. A leading slash
// breaks when the viewer is hosted below the domain root: Vite (or another
// SPA host) then returns index.html for `/gas_r/*.json`, which later surfaces
// as an opaque JSON "Unexpected token <" error.
const levelRoot = encodeURIComponent(levelName);
const SPAWN = new THREE.Vector3(-1268, -172, 4692);
const { difficulty, seed: initialSeed } = readSettings(location.search);
let mouseLook = new MouseLook();
const renderer = new THREE.WebGLRenderer({ antialias: true });
renderer.setPixelRatio(Math.min(devicePixelRatio, 2));
renderer.setSize(innerWidth, innerHeight);
renderer.outputColorSpace = THREE.SRGBColorSpace;
document.body.append(renderer.domElement);
const scene = new THREE.Scene();
scene.background = new THREE.Color(0x7396bd);
scene.fog = new THREE.Fog(0x8c9aa8, 900, 4200);
const camera = new THREE.PerspectiveCamera(75, innerWidth / innerHeight, 2, 8000);
camera.rotation.order = "YXZ";
scene.add(camera);
scene.add(new THREE.HemisphereLight(0xddeeff, 0x303020, 1.5));
const sun = new THREE.DirectionalLight(0xffffff, 1.2);
sun.position.set(300, 900, 500);
scene.add(sun);

const keys = new Set<string>();
const modelMeshes = new Map<number, THREE.Mesh[]>();
const movingModels = new Map<number, { pivot: THREE.Group; motion: AuthoredModelMotion; origin: [number, number, number]; duration: number; progress: number; target: number; timeScale: number; loop: boolean }>();
const triggers: Entity[] = [];
const insideTriggers = new Set<number>();
const insideProps = new Set<number>();
const breakables: Interactive[] = [], fires: Interactive[] = [], victims: Interactive[] = [], nearbyProps: Interactive[] = [];
const emitters: Interactive[] = [];
const victimActors: AnimatedActor[] = [];
const ladders: Entity[] = [];
const doors: { controller: ApproachDoor; pivot: THREE.Group; models: number[] }[] = [];
const slidingDoors: { entity: Entity; model: number; state: { duration: number; target: number; progress: number; timeScale: number }; touching: boolean }[] = [];
const loopModels: { entity: Entity; state: { pivot: THREE.Group; motion: AuthoredModelMotion; origin: [number, number, number]; duration: number; progress: number; target: number; timeScale: number; loop: boolean } }[] = [];
let entities: Entity[] = [];
// C_Heading=3 becomes 3*30+180 = 270 degrees in the retail executable.
let yaw = degreesToRadians(270), pitch = 0;
let spawnHeading = 3;
let player = makePlayer({ x: SPAWN.x, y: SPAWN.y, z: SPAWN.z });
let collision: GenesisBspCollision;
let victimActor: ActorBody;
const victimActorBodies = new Map<string, ActorBody>();
let viewAxe: AnimatedActor | null = null;
let crouchPressedThisFrame = false, mouseHeld = false, jumpHeld = false, jumpPressedThisFrame = false;
let onLadder = false;
let mouseCaptured = false, pointerFallback = false, ready = false;
// training.bsp starts the player with `equip axe`.
let tool: Tool = "axe";
let extinguisherCharge = 0, hasExtinguisher = false, hasHose = false, hoseAttached = false, hasGoggles = false;
let oxygenOn = true;
let gasIsOn = true;
let rescueBotEnabled = true;
let ashDensity = 0;
let carryingVictim = false, rescuedVictims = 0, bonusScore = 0;
let playerStartupCommands: string[] = [];
let levelStartupCommands: { command: string; source: string }[] = [];
let selectAxeAfterStartup = false;
let zone = levelName === "training" ? "Crouching and Jumping Exercise" : levelName, hintUntil = Infinity;
const random = new FdnyRandom(initialSeed);
let axeMechanics = new AxeMechanics();
let simulationMilliseconds = 0;
let lastFrameDelta = 0;
const movementRecorder = new MovementRecorder();

function geometryFor(group: MeshGroup): THREE.BufferGeometry {
  const geometry = new THREE.BufferGeometry();
  geometry.setAttribute("position", new THREE.Float32BufferAttribute(group.positions, 3));
  geometry.setAttribute("uv", new THREE.Float32BufferAttribute(group.uvs, 2));
  geometry.computeVertexNormals(); geometry.computeBoundingSphere();
  return geometry;
}

function positionOf(entity: Entity): THREE.Vector3 | null {
  const value = entity.properties.location ?? entity.properties.ZZ_location ?? entity.properties.origin;
  if (!value) return null;
  const parts = value.replaceAll(",", " ").trim().split(/\s+/).map(Number);
  return parts.length === 3 && parts.every(Number.isFinite) ? new THREE.Vector3(...parts as [number, number, number]) : null;
}

function marker(position: THREE.Vector3, color: number, radius: number, opacity = 0.9): THREE.Mesh {
  const object = new THREE.Mesh(new THREE.SphereGeometry(radius, 14, 10), new THREE.MeshBasicMaterial({ color, transparent: true, opacity, wireframe: true }));
  object.position.copy(position); scene.add(object); return object;
}

function showHint(text: string, seconds = 5): void {
  const hint = document.querySelector<HTMLElement>("#hint");
  if (hint) hint.textContent = text;
  hintUntil = simulationMilliseconds + seconds * 1000;
}

const scripts = new TrainingScripts({
  hint: showHint,
  player(command) {
    // PlayerRescueEnt command handlers used by Training's trigger scripts.
    // `escape` releases the current interaction state; in the authored
    // Training triggers this is a no-op while standing, but it must still be
    // consumed instead of being reported as an unsupported EVS command.
    if (command === "escape") {
      if (carryingVictim) {
        carryingVictim = false; rescuedVictims++;
        showHint(rescuedVictims >= victims.length ? `${levelName} complete.` : "Survivor safe. Locate and extract the next survivor.", 6);
        if (rescuedVictims >= victims.length) document.body.classList.add("complete");
      }
      return true;
    }
    if (command === "finalexit") {
      if (rescuedVictims >= victims.length) document.body.classList.add("complete");
      else showHint(`Rescue all ${victims.length} survivors before leaving.`, 5);
      return true;
    }
    if (/^climb(?:\s+\d+)?$/i.test(command)) { if (player.state !== "climbing") enterClimb(player); onLadder = true; return true; }
    if (command === "unclimb") { if (player.state === "climbing") player.state = "standing"; onLadder = false; return true; }
    const ash = command.match(/^setashdensity\s+((?:[0-9]+(?:\.[0-9]+)?)|(?:\.[0-9]+))$/);
    if (ash) { ashDensity = Number(ash[1]); return true; }
    if (command === "equip goggles") { hasGoggles = true; tool = "goggles"; return true; }
    if (command === "equip extinguisher") { hasExtinguisher = true; tool = "extinguisher"; return true; }
    if (/^(?:select|equip)\s+axe$/i.test(command)) { tool = "axe"; return true; }
    if (/^select\s+extinguisher$/i.test(command)) { if (hasExtinguisher) tool = "extinguisher"; return true; }
    if (command.toLowerCase() === "oxyon") { oxygenOn = true; return true; }
    if (command.toLowerCase() === "oxyoff") { oxygenOn = false; return true; }
    const rescueBot = command.match(/^setrescuebot\s+([01])$/i);
    if (rescueBot) { rescueBotEnabled = rescueBot[1] === "1"; return true; }
    const bonus = command.match(/^addbonus\s+(\d+)$/i);
    if (bonus) { bonusScore += Number(bonus[1]); return true; }
    if (/^(?:settimescale\s+\d+(?:\.\d+)?|sethitpoints\s+-?\d+(?:\.\d+)?|break|gotvictim|hide|openaway|flush|open|close|loop|makevalve|fogdisable|fogenable(?:\s+.+)?|playsnd\s+\S+)$/i.test(command)) return true;
    if (command === "toolcmd extinguisher refill") { extinguisherCharge = 1; return true; }
    if (/^touchhose(?:\s+\d+(?:\.\d+)?)?$/i.test(command)) { hasHose = true; tool = "hose"; return true; }
    const nextMap = command.match(/^finalexitloadmap\s+(.+)$/i);
    if (nextMap) { window.location.href = `?level=${encodeURIComponent(nextMap[1])}`; return true; }
    return false;
  },
  entity(target, command) {
    // EVS name lookup in the retail VM is case-insensitive (several shipped
    // maps mix `ExplodingCar3` and `Explodingcar3`).
    const wanted = target.toLowerCase();
    // EVS names may exist on both a render-only %Model% record and its
    // gameplay controller. Resolve the controller first so commands such as
    // `tell vault open` reach the animated RCS_BModel rather than becoming a
    // no-op on the earlier model record.
    const matching = entities.filter((candidate) =>
      candidate.properties["%name%"]?.toLowerCase() === wanted || candidate.properties.A_Name?.toLowerCase() === wanted);
    const entity = matching.find((candidate) => candidate.class !== "%Model%") ?? matching[0];
    if (!entity) return false;
    const model = entity.model;
    const movingTargets = new Set<number>();
    if (model !== undefined) movingTargets.add(model);
    for (const animatedModel of modelNumbersFor(entity)) movingTargets.add(animatedModel);
    const moving = [...movingTargets].map((number) => movingModels.get(number)).filter((state): state is NonNullable<typeof state> => Boolean(state));
    if (moving.length && /^(open|close)$/i.test(command)) {
      const target = /^open$/i.test(command);
      moving.forEach((state) => { state.target = target ? state.duration : 0; });
      return true;
    }
    const door = doors.find((entry) => entry.controller.entity.index === entity.index);
    if (door && /^(open|close)$/i.test(command)) {
      door.controller.script(command.toLowerCase() as "open" | "close");
      door.pivot.rotation.y = door.controller.angle;
      for (const model of door.models) { collision?.setModelRotationY(model, door.controller.angle); collision?.setModelEnabled(model, true); }
      return true;
    }
    const fire = fires.find((entry) => entry.entity.index === entity.index);
    const move = command.match(/^moveto\s+([A-Za-z0-9_]+)\s+/i);
    if (fire && move) {
      const wantedPoint = move[1].toLowerCase();
      const destination = entities.find((candidate) => candidate.properties["%name%"]?.toLowerCase() === wantedPoint || candidate.properties.A_Name?.toLowerCase() === wantedPoint);
      const point = destination && positionOf(destination);
      if (point) { fire.firePosition?.copy(point); fire.marker.position.copy(point).add(new THREE.Vector3(0, Math.min(fire.fireRadius ?? 0, 120) * 0.25, 0)); return true; }
    }
    const size = command.match(/^setsize\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)$/i);
    if (fire && (size || /^(turnon|turnoff)$/i.test(command))) {
      if (size) {
        const x = Math.abs(Number(size[1])), z = Math.abs(Number(size[3]));
        if (Number.isFinite(x) && Number.isFinite(z)) {
          fire.fireRadius = Math.max(x, z);
          fire.marker.scale.setScalar(Math.max(0.05, fire.fireRadius / (fire.baseFireRadius ?? fire.fireRadius)));
        }
      }
      if (/turnoff/i.test(command)) fire.alive = false;
      if (/turnon/i.test(command)) fire.alive = true;
      fire.marker.visible = fire.alive;
      return true;
    }
    const emitter = emitters.find((entry) => entry.entity.index === entity.index);
    if (emitter) {
      const verb = command.match(/^(turnon|turnoff|setsize|setstrength)\b(?:\s+([\d.]+))?/i);
      if (verb) {
        if (verb[1].toLowerCase() === "turnon") emitter.alive = true;
        else if (verb[1].toLowerCase() === "turnoff") emitter.alive = false;
        else if (verb[2] !== undefined && Number.isFinite(Number(verb[2]))) {
          if (verb[1].toLowerCase() === "setsize") emitter.emitterSize = Number(verb[2]);
          else emitter.emitterStrength = Number(verb[2]);
        }
        emitter.marker.visible = emitter.alive;
        const scale = emitter.emitterSize ?? 1;
        emitter.marker.scale.setScalar(Math.max(0.1, scale));
        return true;
      }
    }
    if (/^(turnon|turnoff|setsize|setskin|playanim|moveto|flush|quake|playsnd)$/i.test(command.split(/\s+/, 1)[0])) return true;
    return false;
  },
  clear(zoneName) {
    return zoneIsClear(zoneName);
  },
  environment(command) {
    if (/^fogdisable$/i.test(command.trim())) {
      scene.fog = new THREE.Fog(0x8c9aa8, 900, 4200);
      return true;
    }
    const match = command.match(/^fogenable\s+(-?[\d.]+)\s+(-?[\d.]+)\s+([\d.]+)\s+([\d.]+)\s+([\d.]+)$/i);
    if (!match) return false;
    const [near, far, red, green, blue] = match.slice(1).map(Number);
    if (![near, far, red, green, blue].every(Number.isFinite)) return false;
    scene.fog = new THREE.Fog(new THREE.Color(Math.min(255, red) / 255, Math.min(255, green) / 255, Math.min(255, blue) / 255), Math.max(0, near), Math.max(near, far));
    return true;
  },
  unsupported(command, source) { console.warn(`Unimplemented EVS [${source}]: ${command}`); },
});

function modelNumbersFor(entity: Entity): number[] {
  const names = Object.entries(entity.properties).filter(([key]) => key.startsWith("C_AnimModel")).map(([, value]) => value);
  return entities.filter((candidate) => candidate.class === "%Model%" && names.includes(candidate.properties["%name%"]))
    .map((candidate) => Number(candidate.properties.Model));
}

function setEntityModelsVisible(entity: Entity, visible: boolean): void {
  const numbers = modelNumbersFor(entity);
  if (entity.model !== undefined && !numbers.includes(entity.model)) numbers.push(entity.model);
  for (const number of numbers) {
    collision.setModelEnabled(number, visible);
    for (const mesh of modelMeshes.get(number) ?? []) mesh.visible = visible;
  }
}

function createGameplayEntities(): void {
  const authoredDoorMotions = [...movingModels.values()].map((state) => state.motion);
  for (const controller of createApproachDoors(entities, authoredDoorMotions)) {
    const pivot = new THREE.Group();
    pivot.position.set(controller.pivot.x, controller.pivot.y, controller.pivot.z);
    scene.add(pivot);
    const models = modelNumbersFor(controller.entity);
    if (!models.includes(controller.entity.model!)) models.push(controller.entity.model!);
    for (const model of models) for (const mesh of modelMeshes.get(model) ?? []) {
      // Mesh vertices are world coordinates; keep them in place around the
      // original %Model% hinge before applying an opening rotation.
      // Door meshes are authored as visible world geometry. Reassert that
      // state while moving them under the runtime pivot so a prior scripted
      // hide on a shared model cannot make an otherwise valid door vanish.
      mesh.visible = true;
      mesh.position.copy(pivot.position).negate();
      pivot.add(mesh);
    }
    doors.push({ controller, pivot, models });
  }
  // Garage doors in the oil-rig missions use translation-only tracks with an
  // identity quaternion. They share the DoorOpen/DoorClose EVS touch contract
  // but must not be rotated like hinged doors.
  for (const entity of entities.filter((candidate) => candidate.class === "RCS_BModel" && candidate.model !== undefined
    && candidate.bounds && Object.values(candidate.properties).some((value) => /\btouch:\s*(?:\*DoorOpen|open)\b/i.test(value)))) {
    if (doors.some((door) => door.controller.entity.index === entity.index)) continue;
    const state = movingModels.get(entity.model!);
    if (!state) continue;
    const rotates = (state.motion.rotation?.keys ?? []).some((key) => Math.abs(key[2]) + Math.abs(key[3]) + Math.abs(key[4]) > 1e-4);
    if (!rotates) slidingDoors.push({ entity, model: entity.model!, state, touching: false });
  }
  for (const entity of entities.filter((candidate) => candidate.class === "RCS_BModel" && candidate.model !== undefined
    && Object.values(candidate.properties).some((value) => /^touch:\s*loop$/i.test(value)))) {
    const state = movingModels.get(entity.model!);
    if (state) loopModels.push({ entity, state });
  }
  triggers.push(...entities.filter((entity) => entity.class === "RCS_BoundsTrigger" && entity.bounds));
  ladders.push(...entities.filter((entity) => entity.class === "RCS_BModel" && entity.properties.S_Script01?.includes("climb") && entity.bounds));
  for (const entity of entities.filter((candidate) => candidate.class === "RCS_BModel")) {
    const hp = Number(entity.properties.F_HitPoints ?? 0), liftable = entity.properties.G_Liftable === "1";
    if ((!hp && !liftable) || !entity.bounds) continue;
    const object = marker(positionOf(entity)!, liftable ? 0x45b8ff : 0xffb12e, 13, 0.65);
    breakables.push({ entity, marker: object, alive: true, progress: hp });
  }
  for (const entity of entities.filter((candidate) => candidate.class === "RCS_FireRescue")) {
    const p = positionOf(entity)!, radius = Number(entity.properties.H_Radius);
    const object = marker(p.clone().add(new THREE.Vector3(0, Math.min(radius, 120) * 0.25, 0)), 0xff351c, Math.min(radius * 0.18, 55));
    const alive = entity.properties.N_IsAlive !== "0";
    object.visible = alive;
    fires.push({ entity, marker: object, alive, progress: alive ? 1 : 0, fireRadius: radius, baseFireRadius: radius, firePosition: p.clone(), authoredFirePosition: p.clone() });
  }
  for (const entity of entities.filter((candidate) => candidate.class === "RCS_MiscEmitter")) {
    const p = positionOf(entity);
    if (!p) continue;
    const type = Number(entity.properties.C_Type ?? 0);
    const color = type === 6 || type === 5 ? 0xff6b26 : type === 2 || type === 3 ? 0xb9d3df : 0xffd36b;
    const active = !Object.values(entity.properties).some((value) => /^turnoff$/i.test(value.trim()));
    const object = marker(p, color, type === 6 ? 18 : 10, 0.28);
    const item: Interactive = { entity, marker: object, alive: active, progress: 1, emitterSize: type === 6 ? 1.4 : 1, baseEmitterSize: type === 6 ? 1.4 : 1, authoredEmitterActive: active };
    object.visible = active;
    emitters.push(item);
    for (const [key, value] of Object.entries(entity.properties)) {
      if (!/^S_Script\d+$/.test(key) || !value || value === "<null>") continue;
      const startup = value.trim().match(/^(turnon|turnoff|setsize|setstrength)\b(?:\s+([\d.]+))?/i);
      if (!startup) continue;
      if (startup[1].toLowerCase() === "turnon") item.alive = true;
      else if (startup[1].toLowerCase() === "turnoff") item.alive = false;
      else if (startup[2] !== undefined && Number.isFinite(Number(startup[2]))) {
        if (startup[1].toLowerCase() === "setsize") item.emitterSize = Number(startup[2]);
        else item.emitterStrength = Number(startup[2]);
      }
    }
    item.baseEmitterSize = item.emitterSize;
    item.authoredEmitterActive = item.alive;
    item.marker.visible = item.alive; item.marker.scale.setScalar(Math.max(0.1, item.emitterSize ?? 1));
  }
  const altByVictim = new Map<string, Entity[]>();
  for (const alt of entities.filter((candidate) => candidate.class === "RCS_VictimAltSpot")) {
    const list = altByVictim.get(alt.properties.A_Victim) ?? []; list.push(alt); altByVictim.set(alt.properties.A_Victim, list);
  }
  for (const entity of entities.filter((candidate) => candidate.class === "RCS_VictimRescue")) {
    const candidates = [entity, ...(altByVictim.get(entity.properties["%name%"]) ?? [])];
    const choice = random.spotIndex(candidates.length), p = positionOf(candidates[choice])!;
    snapVictimToFloor(collision, p);
    if (collision.isBoxInSolid(p, { x: -24, y: -24, z: -24 }, { x: 24, y: 24, z: 24 })) {
      const escape = chooseVictimEscape((offset) => !collision.isBoxInSolid(
        { x: p.x + offset.x, y: p.y + offset.y, z: p.z + offset.z },
        { x: -24, y: -24, z: -24 }, { x: 24, y: 24, z: 24 },
      ));
      if (escape) { p.x += escape.x; p.y += escape.y; p.z += escape.z; snapVictimToFloor(collision, p); }
    }
    // VictimRescueEnt enters its downed rescue state at creation. The retail
    // male actor's corresponding clip is `onback`; keep it animated on the
    // simulation clock rather than leaving the bind pose frozen in place.
    const rawType = (entity.properties.D_Type ?? "man").toLowerCase();
    const type = rawType.startsWith("fat") ? "fatman" : rawType.startsWith("boy") ? "boy" : rawType.startsWith("girl") ? "girl" : rawType.startsWith("woman") ? "woman" : rawType.startsWith("dog") ? "dog" : "man";
    const actorStem = type === "fatman" ? "vic_fatty" : `vic_${type}`;
    const actorBody = victimActorBodies.get(type) ?? victimActor;
    const animated = createAnimatedActor(actorBody, actorStem, false, levelRoot);
    victimActors.push(animated);
    const object = animated.group;
    // VictimRescueEnt.cpp creates the retail actor at uniform scale 2.
    object.scale.setScalar(2);
    object.position.copy(p);
    object.rotation.y = headingToRadians(Number(candidates[choice].properties.B_Heading ?? candidates[choice].properties.G_Heading));
    scene.add(object);
    // Training victims have no autonomous locomotion. Their native state is
    // downed (state 4) until pickup transitions them to carried (state 5).
    const roamValue = Object.values(entity.properties).find((value) => /^roam\s+\d+(?:\.\d+)?$/i.test(value));
    const roam = roamValue?.match(/^roam\s+(\d+(?:\.\d+)?)$/i);
    const roaming = Boolean(roam);
    const downMotion = actorBody.motions.find((motion) => /^(onback|down)$/i.test(motion.name))?.name ?? actorBody.motions[0]?.name ?? "onback";
    let roamingMotion = actorBody.motions.some((motion) => motion.name.toLowerCase() === "slowwalk") ? "slowwalk" : actorBody.motions.some((motion) => motion.name.toLowerCase() === "wag1") ? "Wag1" : "onback";
    const playAnim = Object.values(entity.properties).find((value) => /^playanim\s+\d+$/i.test(value));
    const authoredMotion = playAnim ? actorBody.motions[Number(playAnim.split(/\s+/)[1])]?.name : undefined;
    // Native state 7 immediately selects one of motion slots 3..5. The
    // retail chooser uses Random_Range(0, 2.99): slot 3 (limp) travels at
    // 50.4 units/s, while slots 4/5 travel at 19.2 units/s.
    let roamSpeed = RETAIL_ROAM_SPEED;
    if (roaming && actorBody.motions.length > 5) {
      const gait = random.spotIndex(3);
      roamingMotion = actorBody.motions[3 + gait].name;
      roamSpeed = gait === 0 ? 50.4 : 19.2;
    } else if (roaming && actorBody.motions.length > 0) {
      // Fatty and dog ACTs expose their native three-state locomotion set
      // instead of the man/woman six-clip layout.  The retail chooser still
      // consumes one random slot; their movement speed remains the 36-unit
      // type-6 value for fatty and the normal 19.2-unit value for dog.
      const gait = random.spotIndex(Math.min(3, actorBody.motions.length));
      roamingMotion = actorBody.motions[gait].name;
      roamSpeed = actorBody.motions.length === 4 ? RETAIL_ROAM_SPEED : 19.2;
    }
    const animationRate = actorBody.motions.length === 6 ? 1 : 0.2;
    if (roaming) animated.loop(roamingMotion, 0, animationRate); else animated.play(authoredMotion ?? downMotion, 0, animationRate);
    const victimState: VictimRescueState = roaming ? "roaming" : "downed";
    victims.push({ entity, marker: object, alive: true, progress: choice, state: victimState,
      // VictimRescueEnt::TryRoam writes 36.0f (0x42100000) to the native
      // movement-speed field before selecting its first target.
      roamRadius: roaming ? Number(roam![1]) : undefined, roamSpeed: roaming ? roamSpeed : undefined,
      roamAnchor: roaming ? p.clone() : undefined,
      motionName: authoredMotion ?? downMotion, roamingMotion, animationRate });
  }
  for (const entity of entities.filter((candidate) => candidate.class === "RCS_PropModel")) {
    const name = entity.properties.A_Name, p = positionOf(entity)!;
    const lowerName = name.toLowerCase();
    const color = lowerName.startsWith("valve") ? 0x82b7d9 : lowerName.startsWith("hose") ? 0xffd43b : 0xe9f1f5;
    const valve = Object.values(entity.properties).some((value) => /^makevalve$/i.test(value.trim()));
    nearbyProps.push({ entity, marker: marker(p, color, 12, 0.8), alive: true, progress: 0, valve });
  }
  // Some authored consoles and controls are brush models rather than
  // RCS_PropModel entities (the Bank vault console is one example). They
  // still receive the same touch/use events in the retail dispatcher.
  for (const entity of entities.filter((candidate) => candidate.class !== "RCS_PropModel" && candidate.bounds
    && Object.values(candidate.properties).some((value) => /\buse:/i.test(value)))) {
    const p = positionOf(entity);
    if (p) nearbyProps.push({ entity, marker: marker(p, 0xe9f1f5, 12, 0.8), alive: true, progress: 0 });
  }
  // Event-only brush models use a direct macro reference instead of an
  // explicit `touch:` prefix (for example Bank's axe case and vault door).
  for (const entity of entities.filter((candidate) => candidate.class === "RCS_BModel" && candidate.bounds
    && Object.values(candidate.properties).some((value) => /^\s*\*(?:GrabAxe|VaultDoor|VaultConsole|gasoff)\s*$/i.test(value)))) {
    const p = positionOf(entity);
    if (p) nearbyProps.push({ entity, marker: marker(p, 0xe9f1f5, 12, 0.35), alive: true, progress: Number(entity.properties.F_HitPoints ?? 1) || 1 });
  }
}

async function loadWorld(): Promise<void> {
  const isJsonResponse = (response: Response): boolean =>
    response.ok && (response.headers.get("content-type")?.toLowerCase().includes("json") ?? false);
  const [meshResponse, entitiesResponse, collisionResponse, axeResponse, victimResponse, scriptsResponse, motionsResponse] = await Promise.all([
    fetch(`${levelRoot}/world_mesh.json`), fetch(`${levelRoot}/entities.json`), fetch(`${levelRoot}/collision.json`),
    fetch(`${levelRoot}/actors/axe.json`).then((response) => isJsonResponse(response) ? response : fetch("training/actors/axe.json")),
    fetch(`${levelRoot}/actors/vic_man.json`).then((response) => isJsonResponse(response) ? response : fetch("training/actors/vic_man.json")),
    fetch(`${levelRoot}/scripts.json`),
    fetch(`${levelRoot}/motions.json`).catch(() => new Response("[]", { status: 200 })),
  ]);
  for (const response of [meshResponse, entitiesResponse, collisionResponse, axeResponse, victimResponse, scriptsResponse, motionsResponse]) {
    if (!response.ok) throw new Error(`${response.url}: HTTP ${response.status}`);
    const contentType = response.headers.get("content-type")?.toLowerCase() ?? "";
    if (!contentType.includes("json")) throw new Error(`${response.url}: expected JSON, received ${contentType || "unknown content"}`);
  }
  const scriptBundle = await scriptsResponse.json() as { sources: { path: string; text: string }[] };
  for (const source of scriptBundle.sources) scripts.loadDefinitions(source.text, source.path);
  const mapStartupCommands: { command: string; source: string }[] = [];
  // Preserve top-level map actions as well as `action Player` hooks.  These
  // run after entities/collision exist below so tells can affect authored
  // fires and moving brushes (for example mbase's mortar relocation).
  for (const source of scriptBundle.sources) {
    if (!source.path.startsWith("Maps/")) continue;
    for (const [lineNumber, rawLine] of source.text.split(/\r?\n/).entries()) {
      const line = rawLine.trim();
      if (!line || line.startsWith("//") || /^exec\s+/i.test(line) || /^set\s+/i.test(line)) continue;
      const action = line.match(/^action\s+Player\s+\((.*)\)$/i);
      if (action) mapStartupCommands.push({ command: action[1], source: `${source.path}:${lineNumber + 1}` });
      else if (/^(?:tell|fogenable|fogdisable|quake|playsnd)\b/i.test(line)) mapStartupCommands.push({ command: line, source: `${source.path}:${lineNumber + 1}` });
    }
  }
  levelStartupCommands = mapStartupCommands.slice();
  // rescue.evs ends its common startup pass by selecting the axe after any
  // mission-specific equipment has been granted.
  const rescueSource = scriptBundle.sources.find((source) => source.path.endsWith("Scripts/rescue.evs"));
  // Execute rescue.evs' common difficulty gate. The retail script grants
  // goggles on every non-Training mission at difficulty 0/1 before selecting
  // the axe; leaving this line definition-only changes the initial tool state
  // on routes whose player entity has no explicit goggles command.
  if (rescueSource && levelName !== "training" && difficulty <= 1) {
    scripts.run("equip goggles", "Scripts/rescue.evs:startup/difficulty");
  }
  selectAxeAfterStartup = Boolean(rescueSource?.text.match(/^\s*tell\s+player\s+\(select\s+axe\)/im));
  if (selectAxeAfterStartup) scripts.run("select axe", "Scripts/rescue.evs:startup");
  const world = await meshResponse.json() as WorldMesh;
  entities = await entitiesResponse.json() as Entity[];
  const authoredMotions = await motionsResponse.json() as AuthoredModelMotion[];
  const authoredPlayer = entities.find((entity) => entity.class === "RCS_PlayerRescue");
  const authoredSpawn = authoredPlayer && positionOf(authoredPlayer);
  if (authoredSpawn) {
    SPAWN.copy(authoredSpawn);
    player = makePlayer({ x: SPAWN.x, y: SPAWN.y, z: SPAWN.z });
    spawnHeading = Number(authoredPlayer.properties.C_Heading ?? 3);
    yaw = headingToRadians(spawnHeading);
  }
  const axeActor = await axeResponse.json() as ActorBody;
  // The actor body and its material textures must come from the same mission
  // root. Most maps share Training's axe body; retaining the current level
  // texture root here silently produces an untextured weapon on those maps.
  const axeAssetRoot = axeResponse.url.includes("/training/") ? "/training" : levelRoot;
  victimActor = await victimResponse.json() as ActorBody;
  victimActorBodies.clear(); victimActorBodies.set("man", victimActor);
  const normalizeVictimType = (raw: string): string => raw.toLowerCase().startsWith("fat") ? "fatman" : raw.toLowerCase().startsWith("boy") ? "boy" : raw.toLowerCase().startsWith("girl") ? "girl" : raw.toLowerCase().startsWith("woman") ? "woman" : raw.toLowerCase().startsWith("dog") ? "dog" : "man";
  const victimTypes = [...new Set(entities.filter((entity) => entity.class === "RCS_VictimRescue").map((entity) => normalizeVictimType(entity.properties.D_Type ?? "man")))];
  for (const type of victimTypes) {
    if (type === "man") continue;
    const stem = type === "fatman" ? "vic_fatty" : `vic_${type}`;
    const response = await fetch(`${levelRoot}/actors/${stem}.json`).catch(() => null);
    if (response && isJsonResponse(response)) victimActorBodies.set(normalizeVictimType(type), await response.json() as ActorBody);
  }
  // Entity startup hooks run during the retail spawn pass. In particular the
  // gas station grants the extinguisher and applies its outside ash state.
  if (authoredPlayer) for (const [key, value] of Object.entries(authoredPlayer.properties)) {
    if (/^S_Script\d+$/.test(key) && value && value !== "<null>") { playerStartupCommands.push(value); scripts.run(value, `player:${key}`); }
  }
  // BoundsTrigger brush models are invisible, non-solid event volumes. Treating
  // them as ordinary BSP solids produces the red boxes and phantom floors.
  const triggerModels = entities.filter((entity) => entity.class === "RCS_BoundsTrigger" && entity.model !== undefined)
    .map((entity) => entity.model as number);
  const collisionData = await collisionResponse.json() as CollisionData;
  collision = new GenesisBspCollision(collisionData, triggerModels);
  const motionByModel = new Map(authoredMotions.map((motion) => [motion.model, motion]));
  const textureLoader = new THREE.TextureLoader(), materials = new Map<string, THREE.MeshLambertMaterial>();
  for (const group of world.groups) {
    // col_red/col_black are editor-only trigger/collision volume materials.
    if (group.texture.toLowerCase().startsWith("col_")) continue;
    let material = materials.get(group.texture);
    if (!material) {
      const texture = textureLoader.load(`${levelRoot}/textures/${encodeURIComponent(group.texture)}.png`);
      texture.colorSpace = THREE.SRGBColorSpace; texture.wrapS = texture.wrapT = THREE.RepeatWrapping; texture.magFilter = THREE.NearestFilter;
      material = new THREE.MeshLambertMaterial({ map: texture, side: THREE.DoubleSide }); materials.set(group.texture, material);
    }
    const motion = motionByModel.get(group.model), collisionModel = collisionData.models.find((candidate) => candidate.index === group.model);
    let geometry = geometryFor(group), parent: THREE.Object3D = scene;
    if (motion && collisionModel?.origin) {
      const origin = collisionModel.origin;
      const positions = geometry.getAttribute("position") as THREE.BufferAttribute;
      for (let i = 0; i < positions.count; i++) positions.setXYZ(i, positions.getX(i) - origin[0], positions.getY(i) - origin[1], positions.getZ(i) - origin[2]);
      positions.needsUpdate = true;
      let state = movingModels.get(group.model);
      if (!state) {
        const pivot = new THREE.Group(); pivot.position.set(...origin); scene.add(pivot);
        const authoredScale = entities.find((entity) => entity.model === group.model)?.properties.S_Script01?.match(/^settimescale\s+([0-9]+(?:\.[0-9]+)?)$/i);
        const timeScale = authoredScale ? Number(authoredScale[1]) : 1;
        const duration = motionDuration(motion);
        state = { pivot, motion, origin, duration, progress: 0, target: 0, timeScale: Number.isFinite(timeScale) && timeScale > 0 ? timeScale : 1, loop: false };
        movingModels.set(group.model, state);
      }
      parent = state.pivot;
    }
    const mesh = new THREE.Mesh(geometry, material);
    mesh.name = `model_${group.model}_${group.texture}`; scene.add(mesh);
    if (parent !== scene) { scene.remove(mesh); parent.add(mesh); }
    const list = modelMeshes.get(group.model) ?? []; list.push(mesh); modelMeshes.set(group.model, list);
  }
  createGameplayEntities();
  for (const startup of mapStartupCommands) scripts.run(startup.command, startup.source);
  viewAxe = createAnimatedActor(axeActor, "axe", true, axeAssetRoot);
  // The mesh and bind pose are retail data. Placement remains isolated here
  // until the view-weapon transform is recovered from the executable.
  viewAxe.group.position.set(48, -43, -95);
  viewAxe.group.rotation.y = 1.2;
  viewAxe.group.scale.setScalar(0.8);
  viewAxe.group.renderOrder = 20;
  camera.add(viewAxe.group);
  syncCamera();
  const zoneEl = document.querySelector<HTMLElement>("#zone");
  if (zoneEl && levelName !== "training") zoneEl.textContent = `${levelName} — reconstruction build`;
  ready = true;
  document.querySelector("#loading")?.remove();
  showHint("Click to capture the mouse. Arrow keys move, left-click uses the tool, right-click jumps, C/Ctrl crouches, and Space/Shift interacts.", 8);
}

function syncCamera(): void {
  camera.position.set(player.position.x, player.position.y + eyeHeight(player.state), player.position.z);
  camera.rotation.set(pitch, yaw, 0);
}
function toolWorldPoint(local: { x: number; y: number; z: number }): THREE.Vector3 {
  const point = new THREE.Vector3(local.x, local.y, local.z).applyQuaternion(camera.quaternion).add(camera.position);
  point.set(Math.fround(point.x), Math.fround(point.y), Math.fround(point.z));
  return point;
}
function toolWorldDirection(local: { x: number; y: number; z: number }): THREE.Vector3 {
  const direction = new THREE.Vector3(local.x, local.y, local.z).applyQuaternion(camera.quaternion).normalize();
  direction.set(Math.fround(direction.x), Math.fround(direction.y), Math.fround(direction.z));
  return direction;
}
function feetPosition(): THREE.Vector3 { return new THREE.Vector3(player.position.x, player.position.y + FDNY.hullMins.y, player.position.z); }
function inBounds(point: THREE.Vector3, bounds: Bounds, margin = 0): boolean {
  return point.x >= bounds.mins[0] - margin && point.x <= bounds.maxs[0] + margin
    && point.y >= bounds.mins[1] - margin && point.y <= bounds.maxs[1] + margin
    && point.z >= bounds.mins[2] - margin && point.z <= bounds.maxs[2] + margin;
}

/** Evaluates the authored EVS `ifclear Zone` predicate used by gated doors. */
function zoneIsClear(zoneName: string): boolean {
  const wanted = zoneName.toLowerCase();
  const zoneTrigger = triggers.find((trigger) => {
    const match = Object.values(trigger.properties).find((value) => /^makezone\s*\(/i.test(value));
    return match?.match(/^makezone\s*\(\s*([^\)]+)\)/i)?.[1].trim().toLowerCase() === wanted;
  });
  if (!zoneTrigger?.bounds) return false;
  return fires.filter((fire) => {
    const position = fire.firePosition ?? positionOf(fire.entity);
    return position && inBounds(position, zoneTrigger.bounds!);
  }).every((fire) => !fire.alive);
}

function updateTriggers(): void {
  for (const trigger of triggers) {
    const inside = trigger.bounds !== undefined && playerTouchesTrigger(player, trigger.bounds);
    if (!inside) {
      if (insideTriggers.has(trigger.index)) scripts.dispatch(trigger, "untouch");
      insideTriggers.delete(trigger.index);
      continue;
    }
    const zoneScript = Object.values(trigger.properties).find((value) => value.startsWith("makezone"));
    if (zoneScript) zone = zoneScript.match(/makezone\s*\((.*)\)/)?.[1] ?? zone;
    if (!insideTriggers.has(trigger.index)) {
      insideTriggers.add(trigger.index); scripts.dispatch(trigger, "touch"); dispatchDirectTouch(trigger);
    } else {
      scripts.dispatch(trigger, "touching");
    }
    if (trigger.properties.A_Name === "hinttrigger17" && carryingVictim) {
      carryingVictim = false; rescuedVictims++;
      if (rescuedVictims >= victims.length) { showHint(`${levelName} complete — all survivors rescued.`, 60); document.body.classList.add("complete"); }
      else showHint("Survivor safe. Locate and extract the remaining survivor.", 6);
    }
  }
}

/** Shipped maps also use direct macro references as the default touch action. */
function dispatchDirectTouch(entity: Entity): void {
  if (entity.class !== "RCS_BoundsTrigger" && entity.class !== "RCS_PropModel" && entity.class !== "RCS_BModel") return;
  for (const [key, value] of Object.entries(entity.properties).sort(([a], [b]) => a.localeCompare(b))) {
    if (!/^S_Script\d+$/.test(key) || !value || value === "<null>" || /^\s*(touch|touching|untouch|use|pickup|break|chop)\s*:/i.test(value)) continue;
    if (/^\s*\*(?:bonus\d+|vaultdoor|outside|inside|explosion\d*|brokendoor|quake|warehousecatwalks|towerstairs)\s*$/i.test(value)) scripts.run(value, `${entity.index}:${key}:touch`);
  }
}

function dispatchDirectUse(entity: Entity): void {
  if (entity.class !== "RCS_BModel" && entity.class !== "RCS_PropModel") return;
  for (const [key, value] of Object.entries(entity.properties).sort(([a], [b]) => a.localeCompare(b))) {
    if (!/^S_Script\d+$/.test(key) || !value || value === "<null>" || /^\s*(touch|touching|untouch|use|pickup|break|chop)\s*:/i.test(value)) continue;
    if (/^\s*\*(?:gasoff|vaultdoor|vaultconsole)\s*$/i.test(value)) scripts.run(value, `${entity.index}:${key}:use`);
  }
}

function updatePropTouches(): void {
  for (const prop of nearbyProps) {
    if (!prop.alive) { insideProps.delete(prop.entity.index); continue; }
    // The retail touch dispatcher receives entity contacts from the player
    // hull.  Prop brushes carry authored bounds in the extracted entity set;
    // use those bounds when present and retain the radius only for model-only
    // props that have no collision brush.
    const inside = prop.entity.bounds
      ? playerTouchesTrigger(player, prop.entity.bounds)
      : prop.marker.position.distanceTo(camera.position) < Number(prop.entity.properties.F_TouchRadius ?? 70) + APPROX.propTouchPadding;
    if (!inside) {
      if (insideProps.has(prop.entity.index)) scripts.dispatch(prop.entity, "untouch");
      insideProps.delete(prop.entity.index);
      continue;
    }
    if (!insideProps.has(prop.entity.index)) {
      insideProps.add(prop.entity.index); scripts.dispatch(prop.entity, "touch"); dispatchDirectTouch(prop.entity);
      if (Object.values(prop.entity.properties).some((value) => /^\s*\*bonus\d+\s*$/i.test(value))) {
        prop.alive = false; prop.marker.visible = false;
      }
    } else {
      scripts.dispatch(prop.entity, "touching");
    }
  }
}

function updateVictimContact(): void {
  if (carryingVictim) return;
  const victim = victims.find((item) => item.alive && touchesVictim(player, item.marker.position, Number(item.entity.properties.J_Radius)));
  if (!victim) return;
  victim.alive = false; victim.state = "carried"; victim.marker.visible = false; carryingVictim = true;
  scripts.dispatch(victim.entity, "pickup");
}

/** Keep the rendered ACT instance on the same transform as the native
 * VictimRescueEnt collision/interaction position.  The retail actor is an
 * attached presentation object, so roaming, alternate spawn selection,
 * pickup, and reset all move/hide it together with the gameplay entity. */
function syncVictimActors(): void {
  for (let index = 0; index < victims.length; index++) {
    const actor = victimActors[index];
    const victim = victims[index];
    if (!actor || !victim) continue;
    actor.group.position.copy(victim.marker.position);
    actor.group.rotation.y = victim.marker.rotation.y;
    actor.group.visible = victim.alive && victim.state !== "carried";
  }
}

/** Reproduces the native state-7 bounded wander used by authored `roam N`
 * victims. The retail routine chooses an obstacle-aware target inside the
 * fixed spawn anchor/leash; this keeps that same bounded contract and uses the
 * already recovered BSP hull sweep for each movement step. */
function updateRoamingVictims(dt: number): void {
  for (const victim of victims) {
    if (!victim.alive || victim.state !== "roaming" || !victim.roamAnchor || !victim.roamRadius) continue;
    const position = victim.marker.position;
    if (!victim.roamTarget || position.distanceTo(victim.roamTarget) < 6) {
      const theta = random.nextFloat() * Math.PI * 2;
      const radius = Math.sqrt(random.nextFloat()) * victim.roamRadius;
      const candidate = victim.roamAnchor.clone().add(new THREE.Vector3(Math.cos(theta) * radius, 0, Math.sin(theta) * radius));
      const floor = { x: candidate.x, y: candidate.y, z: candidate.z };
      snapVictimToFloor(collision, floor);
      const candidateTarget = new THREE.Vector3(floor.x, floor.y, floor.z);
      const candidateDelta = candidateTarget.clone().sub(position);
      const blocked = collision.isBoxInSolid(floor, { x: -24, y: -24, z: -24 }, { x: 24, y: 24, z: 24 })
        || Boolean(collision.traceBox({ x: position.x, y: position.y, z: position.z },
          { x: candidateDelta.x, y: candidateDelta.y, z: candidateDelta.z },
          { x: -24, y: -24, z: -24 }, { x: 24, y: 24, z: 24 }));
      // Native VictimRescueEnt probes its obstacle list before committing to
      // a roam target. Rejecting a blocked sweep avoids repeatedly selecting a
      // point behind a wall and matches the four-corner escape philosophy.
      victim.roamTarget = blocked ? position.clone() : candidateTarget;
    }
    const delta = victim.roamTarget.clone().sub(position);
    const distance = delta.length();
    if (distance <= 0) continue;
    delta.multiplyScalar(Math.min(1, (victim.roamSpeed ?? 60) * dt / distance));
    const start = { x: position.x, y: position.y, z: position.z };
    const hit = collision.traceBox(start, { x: delta.x, y: delta.y, z: delta.z }, { x: -24, y: -24, z: -24 }, { x: 24, y: 24, z: 24 });
    if (hit) {
      const escape = chooseVictimEscape((offset) => {
        const probe = { x: position.x + offset.x, y: position.y + offset.y, z: position.z + offset.z };
        return !collision.isBoxInSolid(probe, { x: -24, y: -24, z: -24 }, { x: 24, y: 24, z: 24 })
          && !collision.traceBox({ x: position.x, y: position.y, z: position.z }, offset,
            { x: -24, y: -24, z: -24 }, { x: 24, y: 24, z: 24 });
      });
      if (escape) {
        position.x += escape.x; position.y += escape.y; position.z += escape.z;
        victim.roamTarget = undefined;
      } else victim.roamTarget = position.clone();
      continue;
    }
    position.add(delta);
    victim.marker.rotation.y = Math.atan2(delta.x, delta.z);
  }
}

function updateMovingModels(dt: number): void {
  for (const [model, state] of movingModels) {
    // ApproachDoor owns these tracks: it computes the touch-side direction,
    // quarter-turn interpolation, and collision state.  Letting the generic
    // scripted-motion pass run afterward would reset their brush rotation.
    if (doors.some((door) => door.models.includes(model))) continue;
    if (state.loop && state.duration > 0) {
      state.progress = (state.progress + dt * state.timeScale) % state.duration;
    } else {
      state.progress = Math.max(0, Math.min(state.duration, state.progress + (state.target > state.progress ? dt : -dt) * state.timeScale));
    }
    const sampled = sampleModelMotion(state.motion, state.progress);
    state.pivot.position.set(state.origin[0] + sampled.translation.x, state.origin[1] + sampled.translation.y, state.origin[2] + sampled.translation.z);
    state.pivot.quaternion.copy(sampled.rotation);
    collision.setModelTranslation(model, sampled.translation);
    collision.setModelRotationQuaternion(model, [sampled.rotation.x, sampled.rotation.y, sampled.rotation.z, sampled.rotation.w]);
    // Genesis collision currently exposes Y rotation for brush models; all
    // shipped rescue moving tracks use a Y-axis rotation component.
    const yawAngle = 2 * Math.atan2(sampled.rotation.y, sampled.rotation.w);
    collision.setModelRotationY(model, yawAngle);
  }
}

function aimDistance(object: THREE.Object3D): { along: number; off: number } {
  const direction = new THREE.Vector3(); camera.getWorldDirection(direction);
  const delta = object.position.clone().sub(camera.position), along = delta.dot(direction);
  return { along, off: delta.sub(direction.multiplyScalar(along)).length() };
}
function closest(items: Interactive[], range: number, cone: number): Interactive | null {
  return items.filter((item) => item.alive).map((item) => ({ item, metric: aimDistance(item.marker) }))
    .filter(({ metric }) => metric.along > 0 && metric.along < range && metric.off < cone)
    .sort((a, b) => a.metric.along - b.metric.along)[0]?.item ?? null;
}

function axeContact(): AxeContact {
  const forward = new THREE.Vector3(); camera.getWorldDirection(forward);
  const up = new THREE.Vector3(0, 1, 0).applyQuaternion(camera.quaternion);
  const right = new THREE.Vector3(1, 0, 0).applyQuaternion(camera.quaternion);
  const sweep = axeSweep(camera.position, forward, up, right);
  const radius = AXE.hullRadius;
  const hit = collision.traceBox(sweep.start, sweep.delta, { x: -radius, y: -radius, z: -radius }, { x: radius, y: radius, z: radius });
  if (!hit) return "miss";
  const point = new THREE.Vector3(hit.point.x, hit.point.y, hit.point.z).addScaledVector(forward, radius);
  for (const target of breakables) {
    if (!target.alive || Number(target.entity.properties.F_HitPoints) <= 0) continue;
    // A BModel's runtime bounds include every brush piece, not just AnimModel0.
    const box = new THREE.Box3();
    for (const model of modelNumbersFor(target.entity)) {
      for (const mesh of modelMeshes.get(model) ?? []) box.expandByObject(mesh);
    }
    box.expandByScalar(AXE.contactPadding);
    if (!box.containsPoint(point)) continue;
      target.progress--;
      scripts.dispatch(target.entity, "chop");
      if (target.progress <= 0) {
        target.alive = false; target.marker.visible = false;
        for (const prop of nearbyProps.filter((item) => item.entity.index === target.entity.index)) { prop.alive = false; prop.marker.visible = false; }
      setEntityModelsVisible(target.entity, false); scripts.dispatch(target.entity, "break");
      for (const [key, value] of Object.entries(target.entity.properties)) {
        if (!/^S_Script\d+$/.test(key) || !value || value === "<null>" || /^\s*(touch|touching|untouch|use|pickup|break|chop)\s*:/i.test(value)) continue;
        if (/^\s*\*(?:GrabAxe)\s*$/i.test(value) || /^\s*addtimecmd\b/i.test(value)) scripts.run(value, `${target.entity.index}:${key}:break`);
      }
        showHint("Obstacle chopped clear.", 2); return "broken";
    }
    return "stuck";
  }
  return "stuck";
}

function useTool(dt = 0): void {
  if (tool === "goggles") {
    for (const victim of victims) if (victim.alive) victim.marker.visible = true;
    showHint("Thermal scan active. Press 1, then approach a survivor to carry them.", 5); return;
  }
  if (tool === "axe") return; // Driven by the recovered swing state machine.
  if (tool === "extinguisher" || tool === "hose") {
    if (dt <= 0) return;
    if (tool === "hose" && !hoseAttached) { showHint("Attach the hose at a valve with E first.", 3); return; }
    if (tool === "extinguisher" && (!hasExtinguisher || extinguisherCharge <= 0)) { showHint("Find the extinguisher stand, or return there to refill.", 3); return; }
    if (tool === "extinguisher") {
      // PlayerExtinguisherEnt::Think emits twelve particles at 60-unit
      // spacing. The camera is the recreation's weapon/nozzle transform;
      // contact itself is the recovered 0x0044A7A0 ellipsoid test.
      const direction = toolWorldDirection(RETAIL_TOOL_DIRECTION);
      const particles = sprayLinePoints(toolWorldPoint(RETAIL_EXTINGUISHER_ORIGIN), direction);
      for (const target of fires.filter((item) => item.alive)) {
        const fireRadius = target.fireRadius ?? (Number(target.entity.properties.H_Radius) || 0);
        const firePosition = target.firePosition ?? positionOf(target.entity) ?? target.marker.position;
        for (const particle of particles) {
          const influence = fireParticleInfluence(particle, firePosition, 60, fireRadius);
          if (!Number.isFinite(influence)) continue;
          // Extinguisher ParticleTouch passes fVar7+fVar7 to Spray. Preserve
          // that native doubling and do not clamp before the nonlinear
          // 1-(1-influence)^2 evaluation.
          const result = applyFireSpray(target.progress, dt, Number(target.entity.properties.K_SprayTime), influence * 2, true);
          target.progress = result.health;
          target.marker.scale.setScalar(Math.max(0.05, target.progress));
          if (result.doused) { target.alive = false; target.marker.visible = false; showHint(`${target.entity.properties.A_Name} extinguished.`, 2); break; }
        }
      }
      return;
    }
    // PlayerHoseEnt::Think tests one nozzle particle against each fire. Keep
    // this separate from the extinguisher's 12-particle loop; the native hose
    // radius is a runtime value initialized to the 60-unit tool scale.
    const direction = toolWorldDirection(RETAIL_TOOL_DIRECTION);
    // PlayerHoseEnt::Think (0x004640F0) transforms local (0,0,8) through
    // the player view matrix before applying its attached-hose effect offset.
    // The view-space point is the suppression contact; the constructor origin
    // is retained for the rendered hose model only.
    const nozzle = toolWorldPoint({ x: 0, y: 0, z: 8 });
    for (const target of fires.filter((item) => item.alive)) {
      const firePosition = target.firePosition ?? positionOf(target.entity) ?? target.marker.position;
      const influence = fireParticleInfluence(nozzle, firePosition, RETAIL_HOSE_PARTICLE_RADIUS, target.fireRadius ?? (Number(target.entity.properties.H_Radius) || 0));
      if (!Number.isFinite(influence)) continue;
      const result = applyFireSpray(target.progress, dt, Number(target.entity.properties.K_SprayTime), influence, true);
      target.progress = result.health;
      target.marker.scale.setScalar(Math.max(0.05, target.progress));
      if (result.doused) { target.alive = false; target.marker.visible = false; showHint(`${target.entity.properties.A_Name} extinguished.`, 2); }
    }
  }
}

function interact(): void {
  const usable = breakables.filter((item) => item.alive && Object.values(item.entity.properties).some((value) => /\buse:/i.test(value)))
    .sort((a, b) => a.marker.position.distanceTo(camera.position) - b.marker.position.distanceTo(camera.position))[0];
  if (usable && usable.marker.position.distanceTo(camera.position) < APPROX.objectPickupRange) {
    scripts.dispatch(usable.entity, "use");
    if (levelName === "gas_r" && usable.entity.properties.A_Name === "button1") {
      scripts.run("*gasoff", "gas_r:button1");
      showHint("Gas main shut off.", 5);
    }
    return;
  }
  const prop = nearbyProps.filter((item) => item.alive && item.marker.position.distanceTo(camera.position) < Number(item.entity.properties.F_TouchRadius ?? 70) + APPROX.propUsePadding)
    .sort((a, b) => a.marker.position.distanceTo(camera.position) - b.marker.position.distanceTo(camera.position))[0];
  if (prop) {
    const name = prop.entity.properties.A_Name ?? "";
    if ((prop.valve || name.toLowerCase().startsWith("valve")) && hasHose) { hoseAttached = !hoseAttached; tool = "hose"; showHint(hoseAttached ? "Hose attached. Hold left mouse to spray." : "Hose detached.", 3); scripts.dispatch(prop.entity, "use"); return; }
    if (Object.values(prop.entity.properties).some((value) => /\buse:/i.test(value))) { scripts.dispatch(prop.entity, "use"); dispatchDirectUse(prop.entity); return; }
    dispatchDirectUse(prop.entity);
  }
  const liftable = breakables.find((item) => item.alive && item.entity.properties.G_Liftable === "1" && item.marker.position.distanceTo(camera.position) < APPROX.objectPickupRange);
  if (liftable) { liftable.alive = false; liftable.marker.visible = false; setEntityModelsVisible(liftable.entity, false); showHint("Object cleared from the path.", 2); }
}

function updatePlayer(dt: number, gameTimeMilliseconds: number, wallTimeMilliseconds: number): void {
  dt = Math.min(dt, FDNY.maxDeltaTime);
  scripts.advance(((gameTimeMilliseconds - simulationMilliseconds) >>> 0) / 1000);
  simulationMilliseconds = gameTimeMilliseconds;
  lastFrameDelta = dt;
  // Evaluate approach before movement so a closed brush cannot trap the
  // player at its collision boundary while waiting for a touch event.
  for (const door of doors) {
    const allowOpen = !door.controller.openIfClear || zoneIsClear(door.controller.openIfClear);
    const wasTouching = door.controller.isTouching;
    const state = door.controller.update(player.position, dt, allowOpen);
    if (!allowOpen && !wasTouching && door.controller.isTouching) {
      const warning = Object.values(door.controller.entity.properties).map((value) =>
        value.match(/ifnotclear\s+\w+\s*\(\s*hint\s+([\d.]+)\s+\(([^)]*)\)\s*\)/i)).find(Boolean);
      if (warning) showHint(warning[2], Number(warning[1]));
    }
    door.pivot.rotation.y = state.angle;
    for (const model of door.models) collision.setModelRotationY(model, state.angle);
  }
  for (const door of slidingDoors) {
    const bounds = door.entity.bounds!;
    const distance = [player.position.x, player.position.y, player.position.z].reduce((sum, value, axis) => {
      const delta = value - Math.max(bounds.mins[axis], Math.min(bounds.maxs[axis], value));
      return sum + delta * delta;
    }, 0);
    const touching = distance <= Math.pow(Number(door.entity.properties.E_Reach ?? 0), 2);
    if (touching !== door.touching) {
      door.touching = touching;
      door.state.target = touching ? door.state.duration : 0;
      scripts.dispatch(door.entity, touching ? "touch" : "untouch");
    }
  }
  for (const loop of loopModels) {
    const touching = Boolean(loop.entity.bounds && playerTouchesTrigger(player, loop.entity.bounds));
    loop.state.loop = touching;
    if (!touching) loop.state.target = 0;
  }
  updateMovingModels(dt);
  ({ yaw, pitch } = mouseLook.update(difficulty, player.state === "climbing" || player.state === "spectator"));
  const touchingLadder = ladders.some((ladder) => ladder.bounds && playerTouchesTrigger(player, ladder.bounds));
  if (touchingLadder && !onLadder) enterClimb(player);
  else if (!touchingLadder && onLadder && player.state === "climbing") player.state = "standing";
  onLadder = touchingLadder;
  const input = {
    forward: (keys.has("KeyW") || keys.has("ArrowUp") ? 1 : 0) - (keys.has("KeyS") || keys.has("ArrowDown") ? 1 : 0),
    strafe: ((keys.has("KeyD") || keys.has("ArrowRight") ? 1 : 0) - (keys.has("KeyA") || keys.has("ArrowLeft") ? 1 : 0)) * 0.75,
    jumpHeld, jumpPressed: jumpPressedThisFrame, crouchPressed: crouchPressedThisFrame, yaw, pitch,
  };
  const before = movementRecorder.active ? structuredClone(player) : null;
  const hits = stepMovement(player, input, dt, collision);
  if (before) movementRecorder.record({ dt, input, before, after: player, hits, disabledModels: collision.getDisabledModels(), timing: { wallTimeMilliseconds, gameTimeMilliseconds } });
  jumpPressedThisFrame = false; crouchPressedThisFrame = false;
  syncCamera();
  victimActors.forEach((actor) => actor.update(simulationMilliseconds / 1000));
  updateTriggers(); updatePropTouches(); updateRoamingVictims(dt); updateVictimContact(); syncVictimActors();
  if (tool === "axe") {
    const started = axeMechanics.tick(dt, simulationMilliseconds, mouseHeld && !carryingVictim, camera.position, axeContact);
    if (started) viewAxe?.play("swing", performance.now() / 1000);
  } else if (mouseHeld) {
    useTool(dt);
    if (tool === "extinguisher" && hasExtinguisher) extinguisherCharge = consumeExtinguisher(extinguisherCharge, dt);
  }
}

function selectTool(next: Tool): void {
  // Player::SelectTool 0x0044C2A0 asks the current tool's CanDeselect.
  if (next === tool) return;
  if (tool === "axe" && axeMechanics.state !== "idle") return;
  if ((tool === "extinguisher" || tool === "hose") && mouseHeld) return;
  tool = next;
}

function resetTraining(): void {
  player = makePlayer({ x: SPAWN.x, y: SPAWN.y, z: SPAWN.z });
  mouseLook = new MouseLook(); yaw = headingToRadians(spawnHeading); pitch = 0;
  onLadder = false; axeMechanics = new AxeMechanics(); clearInput();
  insideTriggers.clear(); insideProps.clear(); carryingVictim = false; rescuedVictims = 0; bonusScore = 0;
  zone = levelName === "training" ? "Crouching and Jumping Exercise" : levelName;
  hintUntil = Infinity;
  document.body.classList.remove("complete");
  const hint = document.querySelector<HTMLElement>("#hint"); if (hint) hint.textContent = "";
  random.seed(initialSeed);
  hasExtinguisher = false; extinguisherCharge = 0; hasHose = false; hoseAttached = false; hasGoggles = false; tool = "axe";
  // Establish native defaults before replaying the authored startup pass;
  // commands such as setrescuebot, oxyoff, and setashdensity must survive
  // the replay exactly as they do during the initial entity spawn.
  gasIsOn = true;
  oxygenOn = true;
  rescueBotEnabled = true;
  ashDensity = 0;
  scripts.reset(simulationMilliseconds / 1000);
  if (levelName !== "training" && difficulty <= 1) scripts.run("equip goggles", "Scripts/rescue.evs:reset/difficulty");
  for (const [index, command] of playerStartupCommands.entries()) scripts.run(command, `player:reset:${index}`);
  if (selectAxeAfterStartup) tool = "axe";
  for (const item of breakables) {
    item.alive = true; item.progress = Number(item.entity.properties.F_HitPoints ?? 0);
    item.marker.visible = true; setEntityModelsVisible(item.entity, true);
  }
  for (const item of nearbyProps) { item.alive = true; item.marker.visible = true; item.progress = 0; }
  for (const item of emitters) { item.alive = item.authoredEmitterActive ?? true; item.emitterSize = item.baseEmitterSize; item.marker.visible = item.alive; item.marker.scale.setScalar(Math.max(0.1, item.emitterSize ?? 1)); }
  for (const item of fires) {
    item.alive = item.entity.properties.N_IsAlive !== "0"; item.progress = item.alive ? 1 : 0; item.fireRadius = item.baseFireRadius; item.firePosition?.copy(item.authoredFirePosition ?? positionOf(item.entity) ?? item.firePosition); item.marker.position.copy(item.firePosition ?? item.marker.position).add(new THREE.Vector3(0, Math.min(item.fireRadius ?? 0, 120) * 0.25, 0)); item.marker.visible = item.alive; item.marker.scale.setScalar(1);
  }
  const altByVictim = new Map<string, Entity[]>();
  for (const alt of entities.filter((candidate) => candidate.class === "RCS_VictimAltSpot")) {
    const list = altByVictim.get(alt.properties.A_Victim) ?? []; list.push(alt); altByVictim.set(alt.properties.A_Victim, list);
  }
  for (let i = 0; i < victims.length; i++) {
    const victim = victims[i];
    const candidates = [victim.entity, ...(altByVictim.get(victim.entity.properties["%name%"] ?? "") ?? [])];
    const choice = random.spotIndex(candidates.length), position = positionOf(candidates[choice]);
    if (position) {
      snapVictimToFloor(collision, position);
      if (collision.isBoxInSolid(position, { x: -24, y: -24, z: -24 }, { x: 24, y: 24, z: 24 })) {
        const escape = chooseVictimEscape((offset) => !collision.isBoxInSolid(
          { x: position.x + offset.x, y: position.y + offset.y, z: position.z + offset.z },
          { x: -24, y: -24, z: -24 }, { x: 24, y: 24, z: 24 },
        ));
        if (escape) { position.x += escape.x; position.y += escape.y; position.z += escape.z; snapVictimToFloor(collision, position); }
      }
      victim.marker.position.copy(position);
      victim.marker.rotation.y = headingToRadians(Number(candidates[choice].properties.B_Heading ?? candidates[choice].properties.G_Heading));
    }
    victim.progress = choice; victim.state = victim.roamRadius ? "roaming" : "downed"; victim.roamTarget = undefined; victim.alive = true; victim.marker.visible = true;
    const motion = victimActors[i];
    if (victims[i].roamRadius) motion?.loop(victims[i].roamingMotion ?? "slowwalk", simulationMilliseconds / 1000, victims[i].animationRate);
    else motion?.play(victims[i].motionName ?? "onback", simulationMilliseconds / 1000, victims[i].animationRate);
  }
  syncVictimActors();
  for (const door of doors) {
    door.controller.reset(); door.pivot.rotation.y = 0;
    for (const model of door.models) collision.setModelRotationY(model, 0);
  }
  for (const door of slidingDoors) { door.state.progress = 0; door.state.target = 0; door.touching = false; }
  for (const [model, state] of movingModels) { state.progress = 0; state.target = 0; state.loop = false; state.pivot.position.set(...state.origin); state.pivot.quaternion.identity(); collision.setModelTranslation(model, { x: 0, y: 0, z: 0 }); collision.setModelRotationQuaternion(model, [0, 0, 0, 1]); collision.setModelRotationY(model, 0); }
  for (const startup of levelStartupCommands) scripts.run(startup.command, `${startup.source}:reset`);
  syncCamera();
}

renderer.domElement.tabIndex = 0;
function clearInput(): void {
  mouseLook.clearCounts();
  keys.clear(); mouseHeld = false; jumpHeld = false; jumpPressedThisFrame = false; crouchPressedThisFrame = false;
}
renderer.domElement.addEventListener("click", () => {
  if (!ready) return;
  renderer.domElement.focus();
  if (document.pointerLockElement !== renderer.domElement) {
    renderer.domElement.requestPointerLock().catch(() => {
      // Embedded Chromium may reject native pointer lock. Keep this explicit
      // inspection fallback usable; Escape/blur always releases it.
      pointerFallback = true; mouseCaptured = true;
      renderer.domElement.classList.add("captured");
      showHint("Mouse capture unavailable: inspection mouse steering active. Escape releases controls. Use a standalone browser for pointer lock.", 8);
    });
  }
});
document.addEventListener("pointerlockchange", () => {
  mouseCaptured = document.pointerLockElement === renderer.domElement;
  pointerFallback = false;
  renderer.domElement.classList.toggle("captured", mouseCaptured);
  if (!mouseCaptured) clearInput();
});
addEventListener("blur", () => {
  clearInput();
  if (pointerFallback) { mouseCaptured = false; pointerFallback = false; renderer.domElement.classList.remove("captured"); }
});
document.addEventListener("visibilitychange", () => { if (document.hidden) clearInput(); });
document.addEventListener("mousemove", (event) => {
  if (!mouseCaptured) return;
  mouseLook.addCounts(event.movementX, event.movementY);
});
document.addEventListener("mousedown", (event) => {
  if (!ready || !mouseCaptured) return;
  if (event.button === 0) { mouseHeld = true; useTool(); }
  if (event.button === 2) { jumpHeld = true; jumpPressedThisFrame = true; }
});
document.addEventListener("mouseup", (event) => {
  if (event.button === 0) mouseHeld = false;
  if (event.button === 2) jumpHeld = false;
});
document.addEventListener("keydown", (event) => {
  if (event.code === "Escape" && pointerFallback) {
    mouseCaptured = false; pointerFallback = false; clearInput(); renderer.domElement.classList.remove("captured");
  }
  if (ready && !event.repeat && event.code === "F8") {
    event.preventDefault();
    if (movementRecorder.active) movementRecorder.active = false;
    else movementRecorder.start();
    showHint(movementRecorder.active ? "Recording movement steps in original units. F8 stops; F9 downloads." : "Movement recording stopped. F9 downloads.", 5);
    return;
  }
  if (ready && !event.repeat && event.code === "F9") {
    event.preventDefault(); movementRecorder.active = false;
    const url = URL.createObjectURL(new Blob([JSON.stringify(movementRecorder.trace)], { type: "application/json" }));
    const link = document.createElement("a"); link.href = url; link.download = "training-movement-trace.json";
    document.body.append(link); link.click(); link.remove(); setTimeout(() => URL.revokeObjectURL(url), 1000);
    return;
  }
  if (!ready || !mouseCaptured) return;
  if (["ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight", "Space"].includes(event.code)) event.preventDefault();
  keys.add(event.code);
  if (event.repeat) return;
  // Active executable registration at 0x004395C0 supersedes input.rrf.
  if (["KeyC", "ControlLeft", "ControlRight"].includes(event.code)) crouchPressedThisFrame = true;
  if (["Space", "ShiftLeft", "ShiftRight", "KeyE"].includes(event.code)) interact();
  if (event.code === "Digit1") selectTool("axe");
  if (event.code === "Digit2" && (hasHose || hasExtinguisher)) selectTool(hasHose ? "hose" : "extinguisher");
  if ((event.code === "Digit3" || event.code === "KeyG") && hasGoggles) selectTool("goggles");
  if (event.code === "KeyR") {
    resetTraining();
  }
});
document.addEventListener("keyup", (event) => { keys.delete(event.code); });
document.addEventListener("contextmenu", (event) => { if (mouseCaptured) event.preventDefault(); });
addEventListener("resize", () => { camera.aspect = innerWidth / innerHeight; camera.updateProjectionMatrix(); renderer.setSize(innerWidth, innerHeight); });

function animate(): void {
  if (simulationMilliseconds > hintUntil) { const hint = document.querySelector<HTMLElement>("#hint"); if (hint) hint.textContent = ""; }
  const status = document.querySelector<HTMLElement>("#status");
  if (viewAxe) {
    viewAxe.group.visible = tool === "axe";
    viewAxe.update(performance.now() / 1000);
  }
  if (status) {
    const p = player.position;
  status.textContent = [`body position (units)  ${p.x.toFixed(1)}  ${p.y.toFixed(1)}  ${p.z.toFixed(1)}`, `${player.state === "climbing" ? "climbing" : player.grounded ? "grounded" : "airborne"}${player.state === "crouched" ? " · crouched" : ""} · tool: ${tool}`, `velocity (units/s)  ${player.velocity.x.toFixed(1)}  ${player.velocity.y.toFixed(1)}  ${player.velocity.z.toFixed(1)}`, `extinguisher ${hasExtinguisher ? `${(extinguisherCharge * 100).toFixed(0)}%` : "not found"} · hose ${hasHose ? (hoseAttached ? "attached" : "carried") : "not found"} · oxygen ${oxygenOn ? "on" : "off"}`, `fires ${fires.filter((fire) => fire.alive).length}/${fires.length} · survivors ${rescuedVictims}/${victims.length}${carryingVictim ? " · carrying" : ""} · bonus ${bonusScore}`, `frame ${(lastFrameDelta * 1000).toFixed(3)} ms · ${(lastFrameDelta ? 1 / lastFrameDelta : 0).toFixed(2)} Hz · native cap 17 ms`, `movement recording ${movementRecorder.active ? "ON" : "off"} · ${movementRecorder.trace.samples.length} frames`, `retail MT seed ${initialSeed} · victim choices ${victims.map((victim) => victim.progress).join(",")}`].join("\n");
  }
  const zoneEl = document.querySelector<HTMLElement>("#zone"); if (zoneEl) zoneEl.textContent = zone;
  renderer.render(scene, camera);
}

function startSimulation(): void {
  const simulationClock = new RetailFrameClock(performance.now());
  function simulate(): void {
    const now = performance.now();
    const frame = simulationClock.next(now);
    if (frame.dt > 0) {
      updatePlayer(frame.dt, simulationClock.gameTimeMilliseconds, now);
      animate();
    }
    // Original main loop clocks, updates, then draws. Count update/render cost
    // toward the throttle, rather than adding another 17 ms after that work.
    setTimeout(simulate, simulationClock.delayUntilNextFrame(performance.now()));
  }
  simulate();
  animate();
}

if (requestedLevel) loadWorld().then(startSimulation).catch((error) => {
  const loading = document.querySelector<HTMLElement>("#loading"); if (loading) loading.textContent = `Failed to load ${levelName} map: ${String(error)}`;
  console.error(error);
});

if (!requestedLevel) {
  document.querySelector("#loading")?.remove();
  document.querySelector<HTMLElement>("#level-select")?.removeAttribute("hidden");
}
