import * as T from './vendor/three.module.min.js';
import { OrbitControls } from './vendor/OrbitControls.js';

const $ = id => document.getElementById(id);
const scene = new T.Scene();
scene.background = new T.Color('#202c35');
const camera = new T.PerspectiveCamera(48, 1, 1, 50000);
const renderer = new T.WebGLRenderer({ antialias: true });
renderer.setPixelRatio(Math.min(devicePixelRatio, 2));
renderer.outputColorSpace = T.SRGBColorSpace;
$('view').append(renderer.domElement);

const orbit = new OrbitControls(camera, renderer.domElement);
orbit.enableDamping = true;
orbit.enableRotate = true;
orbit.enablePan = true;
orbit.enableZoom = true;
orbit.target.set(0, 40, 0);
camera.position.set(1400, 1500, 1800);
orbit.update();
scene.add(new T.HemisphereLight(0xffffff, 0x526577, 2));
const sun = new T.DirectionalLight(0xffffff, 2.4);
sun.position.set(600, 1800, 900);
scene.add(sun);
const grid = new T.GridHelper(8192, 256, 0x718b98, 0x394f5b);
scene.add(grid);
scene.add(new T.AxesHelper(120));
const placed = new T.Group();
scene.add(placed);

const previewScene = new T.Scene();
previewScene.background = new T.Color('#1c2a34');
previewScene.add(new T.HemisphereLight(0xffffff, 0x485564, 2));
const previewSun = new T.DirectionalLight(0xffffff, 2.5);
previewSun.position.set(400, 800, 600);
previewScene.add(previewSun);
const previewCamera = new T.PerspectiveCamera(40, 1, 0.1, 10000);
const previewRenderer = new T.WebGLRenderer({ antialias: true, alpha: false });
previewRenderer.setPixelRatio(Math.min(devicePixelRatio, 1.5));
previewRenderer.outputColorSpace = T.SRGBColorSpace;
$('preview').append(previewRenderer.domElement);
const previewOrbit = new OrbitControls(previewCamera, previewRenderer.domElement);
previewOrbit.enableDamping = true;
const previewRoot = new T.Group();
previewScene.add(previewRoot);

const ray = new T.Raycaster();
const mouse = new T.Vector2();
const texLoader = new T.TextureLoader();
const textureCache = new Map();
const fireTextureCache = new Map();
const modelCache = new Map();
const actorCache = new Map();
const thumbImageCache = new Map();
const thumbModelCache = new Map();
const shapeKinds = ['floor', 'wall', 'object', 'ramp', 'fire', 'survivor', 'spawn'];
const sizes = {
  floor: [256, 32, 256], wall: [256, 256, 32], object: [96, 96, 96],
  ramp: [256, 128, 384], fire: [64, 96, 64], survivor: [40, 100, 40], spawn: [60, 115, 60]
};
const defaultMaterials = { floor: 'training:0', wall: 'training:8', object: 'training:3', ramp: 'training:0' };
let inventory = null;
let doc = null;
let selected = -1;
let selectedAsset = null;
let activeTab = 'shapes';
let tool = 'select';
let history = [];
let future = [];
let dirty = false;
let showRoof = false;
let drag = null;
let mouseDown = null;
let rebuildVersion = 0;
let textureTarget = -1;
let thumbObserver = null;

function say(message, error = false) {
  $('status').textContent = message;
  $('status').classList.toggle('error', error);
}

function markDirty() {
  dirty = true;
  $('dirtyMark').classList.add('on');
}

function snapshot() {
  history.push(JSON.stringify(doc));
  if (history.length > 120) history.shift();
  future = [];
  markDirty();
}

async function getJSON(url) {
  const response = await fetch(url);
  if (!response.ok) {
    let body = {};
    try { body = await response.json(); } catch { /* non-JSON response */ }
    throw Error(body.error || `${response.status} ${response.statusText}`);
  }
  return response.json();
}

function texture(ref) {
  if (!ref) return null;
  if (!textureCache.has(ref)) {
    const [map, id] = ref.split(':');
    const loaded = texLoader.load(`/api/texture?map=${encodeURIComponent(map)}&id=${encodeURIComponent(id)}`);
    loaded.colorSpace = T.SRGBColorSpace;
    loaded.wrapS = loaded.wrapT = T.RepeatWrapping;
    textureCache.set(ref, loaded);
  }
  return textureCache.get(ref);
}

function modelData(map, id) {
  const key = `${map}:${id}`;
  if (!modelCache.has(key)) {
    modelCache.set(key, getJSON(`/api/model?map=${encodeURIComponent(map)}&id=${encodeURIComponent(id)}`));
  }
  return modelCache.get(key);
}

function canonicalActor(type) {
  if (type === 'fatman') return 'man';
  if (type === 'boy1') return 'boy';
  return type || 'man';
}

function actorData(type) {
  type = canonicalActor(type);
  if (!actorCache.has(type)) actorCache.set(type, getJSON(`/api/actor?type=${encodeURIComponent(type)}`));
  return actorCache.get(type);
}

function actorTexture(type, index) {
  type = canonicalActor(type);
  const key = `actor:${type}:${index}`;
  if (!textureCache.has(key)) {
    const loaded = texLoader.load(`/api/actor-texture?type=${encodeURIComponent(type)}&id=${index}`);
    loaded.colorSpace = T.SRGBColorSpace;
    textureCache.set(key, loaded);
  }
  return textureCache.get(key);
}

function fireTexture(name) {
  if (!fireTextureCache.has(name)) {
    const loaded = texLoader.load(`/api/fire-sprite?name=${encodeURIComponent(name)}`);
    loaded.colorSpace = T.SRGBColorSpace;
    fireTextureCache.set(name, loaded);
  }
  return fireTextureCache.get(name);
}

function makeSourceRoot(item, index) {
  const root = new T.Group();
  root.position.fromArray(item.position || [0, 0, 0]);
  root.rotation.y = T.MathUtils.degToRad(item.rotation || 0);
  root.scale.fromArray(item.scale || [1, 1, 1]);
  root.userData.index = index;
  return root;
}

async function renderSource(item, parent, index, version = rebuildVersion) {
  const source = await modelData(item.sourceMap, item.sourceModel);
  if (version !== rebuildVersion) return null;
  const root = makeSourceRoot(item, index);
  for (const group of source.groups) {
    if (group.positions.length > 900000) continue;
    const geometry = new T.BufferGeometry();
    geometry.setAttribute('position', new T.Float32BufferAttribute(group.positions, 3));
    geometry.setAttribute('uv', new T.Float32BufferAttribute(group.uvs, 2));
    geometry.computeVertexNormals();
    const material = new T.MeshStandardMaterial({
      color: 0xffffff,
      map: texture(item.texture || `${item.sourceMap}:${group.texture}`),
      side: T.DoubleSide,
      roughness: 0.78,
      metalness: 0.04,
      emissive: index === selected ? new T.Color(0x171007) : new T.Color(0)
    });
    const mesh = new T.Mesh(geometry, material);
    mesh.userData.index = index;
    root.add(mesh);
  }
  root.userData.asset = true;
  parent.add(root);
  return root;
}

function brushGeometry(item) {
  const [width, height, depth] = item.size;
  const vertices = [
    [-width / 2, 0, -depth / 2], [width / 2, 0, -depth / 2],
    [width / 2, 0, depth / 2], [-width / 2, 0, depth / 2],
    [-width / 2, height, -depth / 2], [width / 2, height, -depth / 2],
    [width / 2, height, depth / 2], [-width / 2, height, depth / 2]
  ];
  let faces = [[0, 3, 2, 1], [0, 1, 5, 4], [1, 2, 6, 5], [2, 3, 7, 6], [3, 0, 4, 7], [4, 5, 6, 7]];
  if (item.kind === 'ramp') {
    vertices[4][1] = vertices[5][1] = 0;
    faces = [[0, 3, 2, 1], [1, 2, 6], [2, 3, 7, 6], [3, 0, 7], [0, 1, 6, 7]];
  }
  const angle = T.MathUtils.degToRad(item.rotation || 0);
  const c = Math.cos(angle), s = Math.sin(angle);
  const positions = [], uvs = [];
  for (const face of faces) {
    for (let corner = 1; corner < face.length - 1; corner++) {
      for (const index of [face[0], face[corner], face[corner + 1]]) {
        const [x, y, z] = vertices[index];
        const rotatedX = c * x + s * z;
        const rotatedZ = -s * x + c * z;
        positions.push(rotatedX, y, rotatedZ);
        const [originX, originY, originZ] = item.position || [0, 0, 0];
        uvs.push((originX + rotatedX) / 128 + (originZ + rotatedZ) / 256,
          (originY + y) / 128 + (originZ + rotatedZ) / 128);
      }
    }
  }
  const geometry = new T.BufferGeometry();
  geometry.setAttribute('position', new T.Float32BufferAttribute(positions, 3));
  geometry.setAttribute('uv', new T.Float32BufferAttribute(uvs, 2));
  geometry.computeVertexNormals();
  return geometry;
}

async function renderActor(item, parent, index, version = rebuildVersion) {
  const type = item.survivorType || 'man';
  const data = await actorData(type);
  if (version !== rebuildVersion) return null;
  const root = new T.Group();
  root.position.fromArray(item.position || [0, 0, 0]);
  root.rotation.y = T.MathUtils.degToRad(item.rotation || 0);
  root.userData.index = index;
  const buckets = new Map();
  for (const face of data.faces) {
    const matIndex = Number(face.material) || 0;
    const positions = buckets.get(matIndex) || [];
    for (const vertexIndex of face.vertices) {
      const vertex = data.vertices[vertexIndex];
      const [x, sourceY, sourceZ] = vertex.position;
      // ACT coordinates are Z-up. The native renderer maps them to game X/Y/Z.
      positions.push(x, sourceZ, -sourceY);
    }
    buckets.set(matIndex, positions);
  }
  for (const [matIndex, positions] of buckets) {
    const uvs = [];
    for (const face of data.faces) {
      if ((Number(face.material) || 0) !== matIndex) continue;
      for (const vertexIndex of face.vertices) uvs.push(...data.vertices[vertexIndex].uv);
    }
    const geometry = new T.BufferGeometry();
    geometry.setAttribute('position', new T.Float32BufferAttribute(positions, 3));
    geometry.setAttribute('uv', new T.Float32BufferAttribute(uvs, 2));
    geometry.computeVertexNormals();
    const material = new T.MeshStandardMaterial({
      map: actorTexture(type, matIndex), color: 0xffffff, side: T.DoubleSide,
      roughness: 0.88, emissive: index === selected ? new T.Color(0x171007) : new T.Color(0)
    });
    const mesh = new T.Mesh(geometry, material);
    mesh.userData.index = index;
    root.add(mesh);
  }
  root.scale.setScalar(2);
  root.userData.kind = 'survivor';
  parent.add(root);
  return root;
}

function makeFire(item, index = -1) {
  const root = new T.Group();
  const authoredRadius = Number(item.radius || item.size?.[0] / 2 || 32);
  const authoredHeight = Number(item.fireHeight || item.size?.[1] || 96);
  // FireEntity initializes its sprite bounds to (H_Radius*.75, H_Height*.25, H_Radius*.75).
  const radius = authoredRadius * 0.75;
  const height = authoredHeight * 0.25;
  const strength = T.MathUtils.clamp(Number(item.strength ?? 1), 0, 1);
  const opacity = strength >= 0.5 ? 1 : strength * 2;
  const tintMix = T.MathUtils.clamp(strength / 0.8, 0, 1);
  const flameTint = new T.Color(0.9, 0.3, 0.1).lerp(new T.Color(1, 1, 1), tintMix);
  const flameMeshes = [];
  const material = new T.MeshBasicMaterial({
    map: fireTexture('lflame-00.bmp'),
    color: flameTint,
    transparent: true,
    opacity,
    depthWrite: false,
    side: T.DoubleSide
  });
  material.map.colorSpace = T.SRGBColorSpace;
  for (const degrees of [0, 60, 120]) {
    const plane = new T.Mesh(new T.PlaneGeometry(radius * 0.8, height), material.clone());
    plane.position.y = height / 2;
    plane.rotation.y = T.MathUtils.degToRad(degrees);
    flameMeshes.push(plane);
    root.add(plane);
  }
  const glowMaterial = new T.MeshBasicMaterial({ map: fireTexture('fflame0000.bmp'), color: flameTint, transparent: true, opacity: Math.min(opacity * 1.5, 1), depthWrite: false, side: T.DoubleSide });
  const glow = new T.Mesh(new T.PlaneGeometry(radius, radius), glowMaterial);
  glow.rotation.x = -Math.PI / 2; glow.position.y = 0.5; root.add(glow);
  root.position.fromArray(item.position || [0, 0, 0]);
  root.rotation.y = T.MathUtils.degToRad(item.rotation || 0);
  root.userData.index = index;
  root.userData.fireMeshes = flameMeshes;
  root.userData.glowMesh = glow;
  root.userData.lastFrame = 0;
  return root;
}

function animateFires(now) {
  const frame = Math.floor(now / 70) % 19;
  for (const root of [...placed.children, ...previewRoot.children]) {
    if (!root.userData.fireMeshes || root.userData.lastFrame === frame) continue;
    root.userData.lastFrame = frame;
    const flame = fireTexture(`lflame-${String(frame).padStart(2, '0')}.bmp`);
    const glow = fireTexture(`fflame${String(frame).padStart(4, '0')}.bmp`);
    for (const mesh of root.userData.fireMeshes) { mesh.material.map = flame; mesh.material.needsUpdate = true; }
    root.userData.glowMesh.material.map = glow; root.userData.glowMesh.material.needsUpdate = true;
  }
}

function makeBrush(item, index) {
  const geometry = brushGeometry(item);
  const material = new T.MeshStandardMaterial({
    color: 0xffffff,
    map: texture(item.texture || defaultMaterials[item.kind]),
    side: T.DoubleSide,
    roughness: 0.9,
    emissive: index === selected ? new T.Color(0x171007) : new T.Color(0)
  });
  const mesh = new T.Mesh(geometry, material);
  mesh.position.fromArray(item.position || [0, 0, 0]);
  mesh.userData.index = index;
  mesh.userData.kind = item.kind;
  return mesh;
}

function makeSpawn(item, index) {
  const root = new T.Group();
  const box = new T.Mesh(
    new T.BoxGeometry(...item.size),
    new T.MeshBasicMaterial({ color: 0x56d5b4, transparent: true, opacity: 0.16, wireframe: true })
  );
  box.position.y = item.size[1] / 2;
  root.add(box);
  root.position.fromArray(item.position || [0, 0, 0]);
  root.rotation.y = T.MathUtils.degToRad(item.rotation || 0);
  root.userData.index = index;
  return root;
}

function isRoofItem(item) {
  if (typeof item.roof === 'boolean') return item.roof;
  return item.kind === 'wall' && item.size[1] < Math.min(item.size[0], item.size[2]) && item.position[1] > 100;
}

function applyRoofVisibility() {
  for (const root of placed.children) {
    const item = doc?.items[root.userData.index];
    root.visible = !item || showRoof || !isRoofItem(item);
  }
  $('roof').textContent = showRoof ? 'Hide roof' : 'Show roof';
}

function disposeGroup(group) {
  while (group.children.length) {
    const child = group.children[group.children.length - 1];
    group.remove(child);
    child.traverse(object => {
      object.geometry?.dispose();
      if (object.material) {
        for (const material of Array.isArray(object.material) ? object.material : [object.material]) {
          // Cached textures are shared by thumbnails and repeated level objects.
          material.dispose();
        }
      }
    });
  }
}

async function rebuild() {
  const version = ++rebuildVersion;
  disposeGroup(placed);
  for (let index = 0; index < doc.items.length; index++) {
    const item = doc.items[index];
    if (item.kind === 'source_model') {
      try { await renderSource(item, placed, index, version); }
      catch (error) { if (version === rebuildVersion) say(`Could not load BSP model preview: ${error.message}`, true); }
    } else if (item.kind === 'survivor') {
      try { await renderActor(item, placed, index, version); }
      catch (error) { if (version === rebuildVersion) say(`Could not load extracted ACT model: ${error.message}`, true); }
    } else if (item.kind === 'fire') {
      placed.add(makeFire(item, index));
    } else if (item.kind === 'spawn') {
      placed.add(makeSpawn(item, index));
    } else {
      placed.add(makeBrush(item, index));
    }
    if (version !== rebuildVersion) return;
  }
  updateTree();
  updateInspector();
  updateMode();
  applyRoofVisibility();
}

function visibleList() {
  if (activeTab === 'shapes') {
    return shapeKinds.map(kind => ({ id: `shape:${kind}`, kind: `shape:${kind}`, label: kind[0].toUpperCase() + kind.slice(1), mapName: 'Built-in shapes', category: 'Shape' }));
  }
  if (activeTab === 'brushes') return inventory.models;
  if (activeTab === 'actors') return inventory.actors;
  return inventory.textures;
}

function selectAsset(asset) {
  selectedAsset = asset;
  $('placeSelected').disabled = false;
  updateAssets();
  updateMode();
  renderPreview();
}

function updateFilters() {
  $('mapFilter').replaceChildren(new Option('All source maps', ''), ...inventory.maps.map(map => new Option(map.name, map.key)));
  const categories = activeTab === 'brushes' ? ['BRUSH', ...inventory.types]
    : activeTab === 'actors' ? ['survivor', 'fire']
      : activeTab === 'materials' ? ['Texture'] : ['Shape'];
  $('categoryFilter').replaceChildren(new Option('All types', ''), ...categories.map(category => new Option(category, category)));
  $('assetCount').textContent = `${visibleList().length} assets`;
}

function imageForThumb(ref) {
  if (!thumbImageCache.has(ref)) {
    const img = new Image();
    img.crossOrigin = 'anonymous';
    img.src = ref.startsWith('actor:')
      ? `/api/actor-texture?type=${encodeURIComponent(ref.split(':')[1])}&id=${encodeURIComponent(ref.split(':')[2])}`
      : ref === 'fire'
        ? '/api/fire-sprite?name=lflame-00.bmp'
        : `/api/texture?map=${encodeURIComponent(ref.split(':')[0])}&id=${encodeURIComponent(ref.split(':')[1])}`;
    thumbImageCache.set(ref, img);
  }
  return thumbImageCache.get(ref);
}

function waitForImage(image) {
  if (image.complete) return Promise.resolve();
  return new Promise(resolve => {
    image.addEventListener('load', resolve, { once: true });
    image.addEventListener('error', resolve, { once: true });
  });
}

function solveTextureTransform(source, target) {
  const [s0, s1, s2] = source;
  const [p0, p1, p2] = target;
  const det = s0.x * (s1.y - s2.y) + s1.x * (s2.y - s0.y) + s2.x * (s0.y - s1.y);
  if (Math.abs(det) < 1e-7) return null;
  const solve = (k) => {
    const v0 = p0[k], v1 = p1[k], v2 = p2[k];
    return [
      (v0 * (s1.y - s2.y) + v1 * (s2.y - s0.y) + v2 * (s0.y - s1.y)) / det,
      (v0 * (s2.x - s1.x) + v1 * (s0.x - s2.x) + v2 * (s1.x - s0.x)) / det,
      (v0 * (s1.x * s2.y - s2.x * s1.y) + v1 * (s2.x * s0.y - s0.x * s2.y) + v2 * (s0.x * s1.y - s1.x * s0.y)) / det
    ];
  };
  const x = solve('x'), y = solve('y');
  return [x[0], y[0], x[1], y[1], x[2], y[2]];
}

function drawProjectedTriangles(canvas, triangles) {
  // Keep the immediate icon in place if an extracted model has no usable faces.
  if (!triangles.length) return;
  const ctx = canvas.getContext('2d');
  const width = canvas.width, height = canvas.height;
  ctx.clearRect(0, 0, width, height);
  ctx.fillStyle = '#1c2932';
  ctx.fillRect(0, 0, width, height);
  const points = triangles.flatMap(triangle => triangle.points);
  let minX = Infinity, maxX = -Infinity, minY = Infinity, maxY = -Infinity;
  for (const point of points) { minX = Math.min(minX, point.x); maxX = Math.max(maxX, point.x); minY = Math.min(minY, point.y); maxY = Math.max(maxY, point.y); }
  const scale = Math.min((width - 18) / Math.max(1, maxX - minX), (height - 14) / Math.max(1, maxY - minY));
  const xOffset = (width - (maxX - minX) * scale) / 2 - minX * scale;
  const yOffset = (height - (maxY - minY) * scale) / 2 - minY * scale;
  triangles.sort((a, b) => a.depth - b.depth);
  for (const triangle of triangles) {
    const dest = triangle.points.map(point => ({ x: point.x * scale + xOffset, y: point.y * scale + yOffset }));
    const img = triangle.image;
    if (img?.complete && img.naturalWidth) {
      const source = triangle.uv.map(uv => ({ x: uv[0] * img.naturalWidth, y: (1 - uv[1]) * img.naturalHeight }));
      const transform = solveTextureTransform(source, dest);
      if (transform) {
        ctx.save();
        ctx.beginPath(); ctx.moveTo(dest[0].x, dest[0].y); ctx.lineTo(dest[1].x, dest[1].y); ctx.lineTo(dest[2].x, dest[2].y); ctx.closePath(); ctx.clip();
        ctx.transform(...transform); ctx.drawImage(img, 0, 0); ctx.restore();
        ctx.strokeStyle = '#10182055'; ctx.lineWidth = 0.45;
        ctx.beginPath(); ctx.moveTo(dest[0].x, dest[0].y); ctx.lineTo(dest[1].x, dest[1].y); ctx.lineTo(dest[2].x, dest[2].y); ctx.closePath(); ctx.stroke();
        continue;
      }
    }
    ctx.fillStyle = triangle.color || '#71828d';
    ctx.beginPath(); ctx.moveTo(dest[0].x, dest[0].y); ctx.lineTo(dest[1].x, dest[1].y); ctx.lineTo(dest[2].x, dest[2].y); ctx.closePath(); ctx.fill();
  }
}

function projectGamePoint([x, y, z]) {
  return { x: (x - z) * 0.72, y: -(x + z) * 0.30 - y * 0.78 };
}

async function renderModelThumb(canvas, asset) {
  const key = `${asset.map}:${asset.model}`;
  if (!thumbModelCache.has(key)) thumbModelCache.set(key, modelData(asset.map, asset.model));
  const source = await thumbModelCache.get(key);
  const triangles = [];
  const triangleCount = source.groups.reduce((sum, group) => sum + Math.floor(group.positions.length / 9), 0);
  const stride = Math.max(1, Math.ceil(triangleCount / 900));
  let ordinal = 0;
  for (const group of source.groups) {
    const image = imageForThumb(`${asset.map}:${group.texture}`);
    for (let i = 0; i + 8 < group.positions.length; i += 9) {
      if (ordinal++ % stride !== 0) continue;
      const coords = [0, 1, 2].map(corner => [group.positions[i + corner * 3], group.positions[i + corner * 3 + 1], group.positions[i + corner * 3 + 2]]);
      const points = coords.map(projectGamePoint);
      const uvs = [0, 1, 2].map(corner => group.uvs.slice((i / 3 + corner) * 2, (i / 3 + corner) * 2 + 2));
      triangles.push({ points, uv: uvs, image, depth: coords.reduce((sum, point) => sum + point[0] + point[1] + point[2], 0) / 3 });
    }
  }
  await Promise.all([...new Set(triangles.map(triangle => triangle.image))].map(waitForImage));
  drawProjectedTriangles(canvas, triangles);
}

async function renderActorThumb(canvas, actorType) {
  const data = await actorData(actorType || 'man');
  const triangles = [];
  for (const face of data.faces) {
    const points = [], uv = [];
    let depth = 0;
    for (const vertexIndex of face.vertices) {
      const vertex = data.vertices[vertexIndex];
      const gamePoint = [vertex.position[0] * 2, vertex.position[2] * 2, -vertex.position[1] * 2];
      points.push(projectGamePoint(gamePoint));
      uv.push(vertex.uv);
      depth += gamePoint[0] + gamePoint[1] + gamePoint[2];
    }
    triangles.push({ points, uv, image: imageForThumb(`actor:${actorType || 'man'}:${Number(face.material) || 0}`), depth: depth / 3 });
  }
  await Promise.all([...new Set(triangles.map(triangle => triangle.image))].map(waitForImage));
  drawProjectedTriangles(canvas, triangles);
}

function drawShapeThumb(canvas, kind) {
  const ctx = canvas.getContext('2d');
  const scale = Math.min(canvas.width, canvas.height) / 88;
  const p = value => value * scale;
  ctx.fillStyle = '#1c2932'; ctx.fillRect(0, 0, canvas.width, canvas.height);
  const cx = canvas.width / 2, cy = canvas.height / 2 + p(2);
  ctx.lineWidth = Math.max(1, p(1));
  ctx.strokeStyle = '#637987';
  ctx.fillStyle = '#5c7884';
  if (kind === 'shape:floor') { ctx.beginPath(); ctx.moveTo(cx - p(36), cy); ctx.lineTo(cx, cy - p(16)); ctx.lineTo(cx + p(36), cy); ctx.lineTo(cx, cy + p(17)); ctx.closePath(); ctx.fill(); ctx.stroke(); }
  else if (kind === 'shape:ramp') { ctx.beginPath(); ctx.moveTo(cx - p(32), cy + p(16)); ctx.lineTo(cx - p(32), cy - p(5)); ctx.lineTo(cx + p(32), cy - p(17)); ctx.lineTo(cx + p(32), cy + p(16)); ctx.closePath(); ctx.fill(); ctx.stroke(); }
  else if (kind === 'shape:fire') {
    const img = imageForThumb('fire');
    const paint = () => { if (img.naturalWidth) { ctx.clearRect(0, 0, canvas.width, canvas.height); ctx.fillStyle = '#1c2932'; ctx.fillRect(0, 0, canvas.width, canvas.height); ctx.drawImage(img, cx - p(28), cy - p(34), p(56), p(62)); } };
    img.addEventListener('load', paint, { once: true });
    if (img.complete && img.naturalWidth) paint(); else drawFallbackThumb(canvas, { kind: 'fire' });
  }
  else if (kind === 'shape:survivor') { drawFallbackThumb(canvas, { kind: 'survivor', actor: 'man' }); renderActorThumb(canvas, 'man').catch(() => {}); }
  else if (kind === 'shape:spawn') { ctx.strokeStyle = '#66d6bc'; ctx.strokeRect(cx - p(20), cy - p(25), p(40), p(50)); ctx.beginPath(); ctx.moveTo(cx - p(26), cy); ctx.lineTo(cx + p(26), cy); ctx.moveTo(cx, cy - p(31)); ctx.lineTo(cx, cy + p(31)); ctx.stroke(); }
  else { ctx.fillStyle = '#617f8b'; ctx.fillRect(cx - p(28), cy - p(21), p(56), p(42)); ctx.strokeRect(cx - p(28), cy - p(21), p(56), p(42)); }
}

function drawFallbackThumb(canvas, asset) {
  const ctx = canvas.getContext('2d');
  const w = canvas.width, h = canvas.height;
  const unit = Math.min(w, h) / 64;
  const p = value => value * unit;
  ctx.clearRect(0, 0, w, h);
  ctx.fillStyle = '#1c2932'; ctx.fillRect(0, 0, w, h);
  ctx.strokeStyle = '#40515c'; ctx.lineWidth = Math.max(1, unit);
  ctx.strokeRect(unit / 2, unit / 2, w - unit, h - unit);
  const kind = asset.kind || '';
  if (kind === 'fire' || kind === 'shape:fire') {
    ctx.fillStyle = '#ff9a43'; ctx.beginPath(); ctx.moveTo(w / 2, h - p(10));
    ctx.bezierCurveTo(w / 2 - p(23), h - p(21), w / 2 - p(8), h - p(35), w / 2 - p(5), h - p(47));
    ctx.bezierCurveTo(w / 2 + p(11), h - p(35), w / 2 + p(3), h - p(29), w / 2 + p(19), h - p(20));
    ctx.bezierCurveTo(w / 2 + p(23), h - p(15), w / 2 + p(12), h - p(10), w / 2, h - p(10)); ctx.fill();
    ctx.fillStyle = '#ffe08a'; ctx.beginPath(); ctx.ellipse(w / 2, h - p(18), p(6), p(11), 0, 0, Math.PI * 2); ctx.fill();
  } else if (kind === 'survivor' || kind === 'shape:survivor') {
    ctx.fillStyle = '#80c8d5'; ctx.beginPath(); ctx.arc(w / 2, h / 2 - p(15), p(8), 0, Math.PI * 2); ctx.fill();
    ctx.strokeStyle = '#d5e1df'; ctx.lineWidth = p(6); ctx.lineCap = 'round';
    ctx.beginPath(); ctx.moveTo(w / 2, h / 2 - p(5)); ctx.lineTo(w / 2, h / 2 + p(14));
    ctx.moveTo(w / 2, h / 2); ctx.lineTo(w / 2 - p(13), h / 2 + p(8));
    ctx.moveTo(w / 2, h / 2); ctx.lineTo(w / 2 + p(13), h / 2 + p(8));
    ctx.moveTo(w / 2, h / 2 + p(14)); ctx.lineTo(w / 2 - p(10), h - p(7));
    ctx.moveTo(w / 2, h / 2 + p(14)); ctx.lineTo(w / 2 + p(10), h - p(7)); ctx.stroke();
  } else if (kind === 'texture') {
    const hash = String(asset.id || asset.label || 'texture').split('').reduce((sum, char) => (sum * 31 + char.charCodeAt(0)) >>> 0, 7);
    const colors = ['#8f6b50', '#777d76', '#77705f', '#526c78', '#817565', '#8a6558'];
    ctx.fillStyle = colors[hash % colors.length]; ctx.fillRect(p(7), p(7), w - p(14), h - p(14));
    ctx.strokeStyle = '#d8c5a0'; ctx.lineWidth = p(1);
    for (let i = p(12); i < w - p(6); i += p(12)) { ctx.beginPath(); ctx.moveTo(i, p(7)); ctx.lineTo(i, h - p(7)); ctx.stroke(); }
    for (let i = p(12); i < h - p(6); i += p(12)) { ctx.beginPath(); ctx.moveTo(p(7), i); ctx.lineTo(w - p(7), i); ctx.stroke(); }
  } else {
    const label = `${asset.label || asset.category || asset.model || ''}`.toLowerCase();
    const color = kind.startsWith('shape:') ? '#7897a4' : '#b6936a';
    ctx.fillStyle = color; ctx.strokeStyle = '#d9c8ad'; ctx.lineWidth = p(1.2);
    if (kind === 'shape:floor' || label.includes('floor')) {
      ctx.beginPath(); ctx.moveTo(w / 2, h / 2 - p(18)); ctx.lineTo(w / 2 + p(24), h / 2 - p(6)); ctx.lineTo(w / 2, h / 2 + p(7)); ctx.lineTo(w / 2 - p(24), h / 2 - p(6)); ctx.closePath(); ctx.fill(); ctx.stroke();
    } else if (kind === 'shape:ramp' || label.includes('ramp') || label.includes('stair')) {
      ctx.beginPath(); ctx.moveTo(p(8), h - p(12)); ctx.lineTo(p(8), h - p(28)); ctx.lineTo(w - p(8), p(14)); ctx.lineTo(w - p(8), h - p(12)); ctx.closePath(); ctx.fill(); ctx.stroke();
    } else if (kind === 'shape:spawn') {
      ctx.strokeStyle = '#66d6bc'; ctx.beginPath(); ctx.arc(w / 2, h / 2, p(20), 0, Math.PI * 2); ctx.moveTo(w / 2 - p(25), h / 2); ctx.lineTo(w / 2 + p(25), h / 2); ctx.moveTo(w / 2, h / 2 - p(25)); ctx.lineTo(w / 2, h / 2 + p(25)); ctx.stroke();
    } else {
      // Isometric brush icon, useful as an immediate fallback for BSP assets.
      ctx.beginPath(); ctx.moveTo(w / 2, p(9)); ctx.lineTo(w - p(8), h / 2 - p(8)); ctx.lineTo(w / 2, h / 2 + p(4)); ctx.lineTo(p(8), h / 2 - p(8)); ctx.closePath(); ctx.fill(); ctx.stroke();
      ctx.fillStyle = '#85684b'; ctx.beginPath(); ctx.moveTo(p(8), h / 2 - p(8)); ctx.lineTo(w / 2, h / 2 + p(4)); ctx.lineTo(w / 2, h - p(8)); ctx.lineTo(p(8), h - p(20)); ctx.closePath(); ctx.fill(); ctx.stroke();
      ctx.fillStyle = '#a0805e'; ctx.beginPath(); ctx.moveTo(w / 2, h / 2 + p(4)); ctx.lineTo(w - p(8), h / 2 - p(8)); ctx.lineTo(w - p(8), h - p(20)); ctx.lineTo(w / 2, h - p(8)); ctx.closePath(); ctx.fill(); ctx.stroke();
    }
  }
}

function thumbCard(asset) {
  const button = document.createElement('button');
  button.className = `asset-card${selectedAsset?.id === asset.id ? ' selected' : ''}`;
  button.onclick = () => selectAsset(asset);
  const thumb = document.createElement('div');
  thumb.className = 'thumb';
  const canvas = document.createElement('canvas');
  canvas.width = 96; canvas.height = 96;
  drawFallbackThumb(canvas, asset);
  thumb.append(canvas);
  button.append(thumb);
  const copy = document.createElement('div'); copy.className = 'card-copy';
  const title = document.createElement('span'); title.className = 'card-title'; title.textContent = asset.label || asset.name;
  const sub = document.createElement('div'); sub.className = 'card-sub';
  const source = document.createElement('span'); source.textContent = asset.mapName || asset.category || 'Built in';
  const detail = document.createElement('span'); detail.textContent = asset.triangles ? `${asset.triangles} △` : asset.actor || '';
  sub.append(source, detail); copy.append(title, sub); button.append(copy);
  if (asset.kind === 'source_model') {
    thumbObserver?.observe(canvas);
    canvas.dataset.thumbType = 'model'; canvas.dataset.map = asset.map; canvas.dataset.model = asset.model;
  } else if (asset.kind === 'survivor') {
    canvas.dataset.thumbType = 'actor'; canvas.dataset.actor = canonicalActor(asset.actor);
    thumbObserver?.observe(canvas);
  } else if (asset.kind === 'fire') {
    const img = imageForThumb('fire');
    const draw = () => { if (!img.naturalWidth) return; const ctx = canvas.getContext('2d'); ctx.clearRect(0, 0, canvas.width, canvas.height); ctx.fillStyle = '#1c2932'; ctx.fillRect(0, 0, canvas.width, canvas.height); ctx.drawImage(img, 13, 7, 70, 82); };
    img.addEventListener('load', draw, { once: true }); if (img.complete) draw();
  } else if (asset.kind === 'texture') {
    const img = imageForThumb(asset.id);
    const draw = () => { if (!img.naturalWidth) return; const ctx = canvas.getContext('2d'); ctx.clearRect(0, 0, canvas.width, canvas.height); ctx.drawImage(img, 0, 0, canvas.width, canvas.height); };
    img.addEventListener('load', draw, { once: true }); if (img.complete) draw();
  } else if (asset.kind?.startsWith('shape:')) {
    drawShapeThumb(canvas, asset.kind);
  }
  return button;
}

function updateAssets() {
  if (!inventory) return;
  const query = $('assetSearch').value.toLowerCase().trim();
  const map = $('mapFilter').value;
  const category = $('categoryFilter').value;
  const rows = visibleList().filter(asset =>
    (!map || asset.map === map) &&
    (!category || asset.category === category || asset.kind === category || (activeTab === 'materials' && category === 'Texture')) &&
    (!query || `${asset.label || asset.name} ${asset.mapName || ''} ${asset.category || ''} ${asset.actor || ''} ${asset.model || ''}`.toLowerCase().includes(query))
  );
  thumbObserver?.disconnect();
  thumbObserver = new IntersectionObserver(entries => {
    for (const entry of entries) {
      if (!entry.isIntersecting) continue;
      const canvas = entry.target;
      thumbObserver.unobserve(canvas);
      if (canvas.dataset.thumbType === 'model') {
        renderModelThumb(canvas, { map: canvas.dataset.map, model: Number(canvas.dataset.model) }).catch(() => drawFallbackThumb(canvas, { kind: 'source_model' }));
      } else if (canvas.dataset.thumbType === 'actor') {
        renderActorThumb(canvas, canvas.dataset.actor).catch(() => drawFallbackThumb(canvas, { kind: 'survivor', actor: canvas.dataset.actor }));
      }
    }
  }, { root: $('assetList'), rootMargin: '120px' });
  $('assetList').replaceChildren(...rows.slice(0, 900).map(thumbCard));
  $('assetCount').textContent = `${rows.length} assets${rows.length > 900 ? ' · narrow search' : ''}`;
  $('assetHint').textContent = activeTab === 'brushes'
    ? 'Each card is a textured preview of an extracted BSP brush model. Click to inspect; use Place to add it.'
    : activeTab === 'materials'
      ? 'Choose a game texture, then Apply selected texture to the selected terrain or BSP object.'
      : activeTab === 'actors'
        ? 'Extracted in-game ACT survivors and FireRescue setups used by existing maps.'
        : 'Choose terrain, a ramp, player start, fire or survivor. The card images come from actual game assets where available.';
}

function clearPreview() {
  disposeGroup(previewRoot);
}

async function renderPreview() {
  clearPreview();
  if (!selectedAsset) {
    previewCamera.position.set(260, 240, 310);
    previewCamera.lookAt(0, 40, 0);
    previewOrbit.target.set(0, 40, 0);
    return;
  }
  const asset = selectedAsset;
  const geometryItem = { position: [0, 0, 0], rotation: 0, size: sizes.object, kind: 'object' };
  try {
    if (asset.kind === 'source_model') {
      const item = { sourceMap: asset.map, sourceModel: asset.model, position: [0, 0, 0], rotation: 0, scale: [1, 1, 1] };
      const root = await renderSource(item, previewRoot, -1);
      if (!root || selectedAsset !== asset) return;
      const bounds = new T.Box3().setFromObject(root);
      const center = bounds.getCenter(new T.Vector3());
      root.position.sub(center);
      framePreview(root);
      return;
    }
    if (asset.kind === 'survivor') {
      await renderActor({ kind: 'survivor', survivorType: canonicalActor(asset.actor), position: [0, 0, 0], size: [40, asset.height || 100, 40] }, previewRoot, -1);
      if (selectedAsset !== asset) return;
    } else if (asset.kind === 'fire') {
      const fire = makeFire({ kind: 'fire', position: [0, 0, 0], size: [2 * (asset.radius || 50), asset.height || 100, 2 * (asset.radius || 50)], radius: asset.radius || 50, fireHeight: asset.height || 100 });
      previewRoot.add(fire);
    } else if (asset.kind === 'texture') {
      const material = new T.MeshStandardMaterial({ map: texture(asset.id), side: T.DoubleSide });
      const plane = new T.Mesh(new T.BoxGeometry(150, 10, 150), material);
      previewRoot.add(plane);
    } else if (asset.kind === 'source_model') {
      return;
    } else if (asset.kind?.startsWith('shape:')) {
      const kind = asset.kind.slice(6);
      if (kind === 'survivor') {
        await renderActor({ kind, survivorType: 'man', position: [0, 0, 0], size: sizes.survivor }, previewRoot, -1);
      } else if (kind === 'fire') {
        previewRoot.add(makeFire({ kind, position: [0, 0, 0], size: sizes.fire, radius: 32, fireHeight: 96 }));
      } else if (kind === 'spawn') {
        previewRoot.add(makeSpawn({ size: sizes.spawn, position: [0, 0, 0] }, -1));
      } else {
        const item = { ...geometryItem, kind, size: sizes[kind], texture: null };
        previewRoot.add(makeBrush(item, -1));
      }
    }
    if (selectedAsset !== asset) return;
    const root = previewRoot.children[0];
    if (root) framePreview(root);
  } catch (error) {
    say(`Preview unavailable: ${error.message}`, true);
  }
}

function framePreview(root) {
  const bounds = new T.Box3().setFromObject(root);
  const size = bounds.getSize(new T.Vector3());
  const center = bounds.getCenter(new T.Vector3());
  root.position.sub(center);
  const distance = Math.max(...size.toArray(), 48) * 1.8;
  previewCamera.position.set(distance, distance * 0.72, distance);
  previewCamera.lookAt(0, 0, 0);
  previewOrbit.target.set(0, 0, 0);
  previewOrbit.update();
}

function setTab(tab) {
  activeTab = tab;
  document.querySelectorAll('#assetTabs button').forEach(button => button.classList.toggle('active', button.dataset.tab === tab));
  updateFilters(); updateAssets();
}

function setSelected(index) {
  selected = index;
  rebuild();
}

function objectIndexOf(object) {
  for (let current = object; current; current = current.parent) {
    if (Number.isInteger(current.userData?.index) && current.userData.index >= 0) return current.userData.index;
  }
  return null;
}

function setWorldPoint(event) {
  const rect = renderer.domElement.getBoundingClientRect();
  mouse.set((event.clientX - rect.left) / rect.width * 2 - 1, -(event.clientY - rect.top) / rect.height * 2 + 1);
  ray.setFromCamera(mouse, camera);
}

function intersectHorizontal(event, height) {
  setWorldPoint(event);
  const plane = new T.Plane(new T.Vector3(0, 1, 0), -height);
  const point = new T.Vector3();
  return ray.ray.intersectPlane(plane, point) ? point : null;
}

function makePlacedItem(point) {
  if (!selectedAsset) return null;
  const asset = selectedAsset;
  const step = $('snap').checked ? Number($('gridSize').value) : 1;
  const snap = value => Math.round(value / step) * step;
  const y = Number($('placeY').value) || 0;
  const position = [snap(point.x), y, snap(point.z)];
  if (asset.kind?.startsWith('shape:')) {
    const kind = asset.kind.slice(6);
    if (['floor', 'wall', 'object', 'ramp'].includes(kind)) {
      return { kind, position: [position[0], kind === 'floor' ? y - sizes.floor[1] : y, position[2]], size: [...sizes[kind]], rotation: 0, scale: [1, 1, 1], texture: null };
    }
    if (kind === 'fire') return { kind, position, size: [...sizes.fire], radius: 32, fireHeight: 96, strength: 1, sprayTime: 4, floorMe: true, smoking: true, spread: false, rotation: 0 };
    if (kind === 'survivor') return { kind, position, size: [...sizes.survivor], survivorType: 'man', roam: 0, pickupRadius: 60, rotation: 0 };
    if (kind === 'spawn') return { kind, position: [position[0], y + 32, position[2]], size: [...sizes.spawn], rotation: 0 };
  }
  if (asset.kind === 'source_model') {
    return { kind: 'source_model', label: asset.label, sourceMap: asset.map, sourceModel: asset.model,
      position, size: asset.max.map((value, axis) => value - asset.min[axis]), scale: [1, 1, 1], rotation: 0, texture: null };
  }
  if (asset.kind === 'survivor') {
    return { kind: 'survivor', label: asset.label, position, size: [40, asset.height || 100, 40],
      survivorType: asset.actor || 'man', roam: asset.roam || 0, pickupRadius: asset.radius || 60, rotation: 0 };
  }
  if (asset.kind === 'fire') {
    const radius = asset.radius || 50;
    return { kind: 'fire', label: asset.label, position, size: [radius * 2, asset.height || 100, radius * 2], radius,
      fireHeight: asset.height || 100, strength: 1, sprayTime: asset.sprayTime || 4, floorMe: true, smoking: asset.smoking !== false, spread: !!asset.spread, rotation: 0 };
  }
  return null;
}

function placeAt(point) {
  const item = makePlacedItem(point);
  if (!item) return;
  snapshot();
  if (item.kind === 'spawn') doc.items = doc.items.filter(existing => existing.kind !== 'spawn');
  doc.items.push(item);
  selected = doc.items.length - 1;
  rebuild();
}

function placeSelectedAtCursor() {
  if (selectedAsset?.kind === 'texture') {
    applySelectedTexture();
    return;
  }
  if (!selectedAsset) { say('Choose an asset in the library first.'); return; }
  setTool('place');
  say(`Place ${selectedAsset.label || selectedAsset.name || selectedAsset.kind}: click in the viewport; drag to orbit.`);
}

function applySelectedTexture() {
  if (selected < 0 || selectedAsset?.kind !== 'texture') { say('Select a level object and then choose a texture.'); return; }
  snapshot();
  doc.items[selected].texture = selectedAsset.id;
  rebuild();
  setTool('select');
  say(`Applied ${selectedAsset.name} to the selected object.`);
}

function setTool(next) {
  if (next === 'move' && selected < 0) {
    say('Select a placed object before choosing Move.');
    return;
  }
  if (next === 'place' && !selectedAsset) {
    say('Choose an asset in the library before choosing Place.');
    return;
  }
  tool = next;
  orbit.enabled = next !== 'move';
  $('selectTool').classList.toggle('active', next === 'select');
  $('moveTool').classList.toggle('active', next === 'move');
  $('placeTool').classList.toggle('active', next === 'place');
  renderer.domElement.style.cursor = next === 'place' ? 'crosshair' : next === 'move' ? 'move' : 'grab';
  updateMode();
}

function updateMode() {
  $('mode').textContent = tool === 'place'
    ? `Place ${selectedAsset?.label || selectedAsset?.name || selectedAsset?.kind || 'asset'} · click to add`
    : tool === 'move' ? 'Move · drag selected item on ground plane'
      : 'Select · orbit / pan / zoom';
  $('cancelPlacement').textContent = tool === 'select' ? 'Select / orbit · Esc' : 'Cancel tool · Esc';
  $('placeSelected').textContent = selectedAsset?.kind === 'texture' ? 'Apply selected texture' : 'Place selected asset';
  $('placeSelected').disabled = !selectedAsset;
}

function updateTree() {
  const options = doc.items.map((item, index) => {
    const name = item.label || (item.sourceMap ? `${item.label || `model ${item.sourceModel}`}` : item.kind);
    return new Option(`${String(index + 1).padStart(2, '0')} · ${name}`, String(index));
  });
  $('objects').replaceChildren(...options);
  $('objects').value = String(selected);
}

function makeLabel(text) {
  const label = document.createElement('label'); label.textContent = text; return label;
}

function inputNumber(value, step, callback, min = null, max = null) {
  const input = document.createElement('input');
  input.type = 'number'; input.step = step; input.value = Number(value).toFixed(String(step).includes('.') ? 2 : 0);
  if (min !== null) input.min = min;
  if (max !== null) input.max = max;
  input.onchange = () => {
    const next = Number(input.value);
    if (!Number.isFinite(next) || (min !== null && next < min) || (max !== null && next > max)) { input.value = value; return; }
    snapshot(); callback(next); rebuild();
  };
  return input;
}

function scalarField(labelText, value, callback, step = '1', min = null, max = null) {
  const label = makeLabel(labelText);
  label.append(inputNumber(value, step, callback, min, max));
  return label;
}

function rowField(labelText, values, callback, step = '8', limits = null) {
  const label = makeLabel(labelText); const row = document.createElement('div'); row.className = 'vector-row';
  values.forEach((value, axis) => row.append(inputNumber(value, step, next => callback(axis, next), limits?.[0], limits?.[1])));
  label.append(row); return label;
}

function chooseField(labelText, value, options, callback) {
  const label = makeLabel(labelText); const select = document.createElement('select');
  for (const [key, name] of options) select.add(new Option(name, key));
  select.value = String(value);
  select.onchange = () => { snapshot(); callback(select.value); rebuild(); };
  label.append(select); return label;
}

function checkField(labelText, value, callback) {
  const label = makeLabel(''); const input = document.createElement('input'); input.type = 'checkbox'; input.checked = !!value;
  input.onchange = () => { snapshot(); callback(input.checked); rebuild(); };
  label.append(input, document.createTextNode(` ${labelText}`)); return label;
}

function updateInspector() {
  const root = $('properties'); root.replaceChildren();
  const item = doc?.items[selected];
  $('selection').textContent = item ? (item.label || `${item.kind[0].toUpperCase()}${item.kind.slice(1)}`) : 'Nothing selected';
  $('emptyInspector').hidden = !!item;
  $('duplicate').disabled = $('frame').disabled = $('deleteItem').disabled = $('delete').disabled = !item;
  if (!item) return;

  root.append(rowField('Position · X / Y / Z', item.position, (axis, value) => item.position[axis] = value, '1'));
  root.append(scalarField('Rotation · degrees', item.rotation || 0, value => item.rotation = value, '1', -36000, 36000));
  const rotation = document.createElement('div'); rotation.className = 'mini-actions';
  for (const [label, step] of [['−90°', -90], ['−15°', -15], ['+15°', 15], ['+90°', 90]]) {
    const button = document.createElement('button'); button.textContent = label;
    button.onclick = () => { snapshot(); item.rotation = ((item.rotation || 0) + step + 360) % 360; rebuild(); };
    rotation.append(button);
  }
  root.append(rotation);

  if (item.kind === 'source_model') {
    root.append(rowField('Scale · X / Y / Z', item.scale || [1, 1, 1], (axis, value) => item.scale[axis] = value, '0.05', [0.05, 20]));
    root.append(checkField('Treat as ceiling / roof (hidden by default)', isRoofItem(item), value => item.roof = value));
    const bounds = makeLabel('Source bounds · width / height / depth');
    const readout = document.createElement('div'); readout.className = 'bounds-readout'; readout.textContent = item.size.map(value => Number(value).toFixed(0)).join(' × ');
    bounds.append(readout); root.append(bounds);
  } else if (['floor', 'wall', 'object', 'ramp'].includes(item.kind)) {
    root.append(rowField('Dimensions · width / height / depth', item.size, (axis, value) => item.size[axis] = value, '1', [1, 10000]));
    root.append(checkField('Treat as ceiling / roof (hidden by default)', isRoofItem(item), value => item.roof = value));
  } else if (item.kind === 'fire') {
    root.append(scalarField('Fire radius · gameplay / visual size', item.radius || item.size[0] / 2, value => { item.radius = value; item.size[0] = value * 2; item.size[2] = value * 2; }, '1', 1, 10000));
    root.append(scalarField('Fire height', item.fireHeight || item.size[1], value => { item.fireHeight = value; item.size[1] = value; }, '1', 1, 10000));
    root.append(scalarField('Initial fire strength · 0–1', item.strength ?? 1, value => item.strength = value, '0.05', 0, 1));
    root.append(scalarField('Spray time · seconds', item.sprayTime ?? 4, value => item.sprayTime = value, '0.1', 0, 1000));
    root.append(checkField('Floor-align fire', item.floorMe !== false, value => item.floorMe = value));
    root.append(checkField('Smoke', item.smoking !== false, value => item.smoking = value));
    root.append(checkField('Allow fire to spread', !!item.spread, value => item.spread = value));
  } else if (item.kind === 'survivor') {
    root.append(chooseField('ACT survivor type', item.survivorType || 'man', [['man', 'Man'], ['woman', 'Woman'], ['girl', 'Girl'], ['boy', 'Boy'], ['boy1', 'Boy variant'], ['fatman', 'Random larger survivor'], ['dog', 'Dog']], value => item.survivorType = value));
    root.append(scalarField('Roam radius · 0 disables roaming', item.roam || 0, value => item.roam = value, '10', 0, 10000));
    root.append(scalarField('Pickup radius', item.pickupRadius || 60, value => item.pickupRadius = value, '1', 1, 500));
  }
  root.append(makeMaterialEditor(item));
  if (item.sourceMap) {
    const note = document.createElement('p'); note.className = 'small-copy';
    note.textContent = `Imported from ${inventory.maps.find(map => map.key === item.sourceMap)?.name || item.sourceMap}, BSP model ${item.sourceModel}. Extracted geometry and source textures are retained.`;
    root.append(note);
  }
}

function makeMaterialEditor(item) {
  const wrap = document.createElement('div'); wrap.className = 'material-edit';
  const label = makeLabel('Surface material'); const row = document.createElement('div'); row.className = 'material-row';
  const button = document.createElement('button');
  const selectedTexture = item.texture ? inventory?.textures.find(textureItem => textureItem.id === item.texture) : null;
  button.textContent = selectedTexture ? `${selectedTexture.name} · ${selectedTexture.mapName}` : 'Original / default game material';
  button.onclick = () => { textureTarget = selected; $('textureDialog').hidden = false; $('textureSearch').value = ''; renderTextures(); };
  const clear = document.createElement('button'); clear.textContent = '↺'; clear.title = 'Use original/default material';
  clear.onclick = () => { snapshot(); delete item.texture; rebuild(); };
  row.append(button, clear); label.append(row); wrap.append(label); return wrap;
}

function renderTextures() {
  const query = $('textureSearch').value.toLowerCase().trim();
  const rows = inventory.textures.filter(item => !query || `${item.name} ${item.mapName}`.toLowerCase().includes(query)).slice(0, 1000);
  $('textureList').replaceChildren(...rows.map(item => {
    const button = document.createElement('button'); button.className = 'texture-card';
    button.onclick = () => {
      const target = doc.items[textureTarget];
      if (target) { snapshot(); target.texture = item.id; selected = textureTarget; rebuild(); }
      $('textureDialog').hidden = true; selectedAsset = item; renderPreview(); setTab('materials');
    };
    const image = document.createElement('img'); image.loading = 'lazy';
    image.src = `/api/texture?map=${encodeURIComponent(item.map)}&id=${item.texture}`; image.alt = '';
    const name = document.createElement('span'); name.textContent = item.name;
    const map = document.createElement('span'); map.textContent = item.mapName; map.style.color = '#899eac';
    button.append(image, name, map); return button;
  }));
}

function hitItem(event) {
  setWorldPoint(event);
  const hits = ray.intersectObjects(placed.children, true);
  for (const hit of hits) {
    const index = objectIndexOf(hit.object);
    if (Number.isInteger(index)) return { index, hit };
  }
  return null;
}

function drawPlacedTransform(index) {
  const root = placed.children.find(child => child.userData.index === index);
  if (root) {
    root.position.fromArray(doc.items[index].position);
    root.rotation.y = T.MathUtils.degToRad(doc.items[index].rotation || 0);
  }
}

renderer.domElement.addEventListener('pointerdown', event => {
  if (event.button !== 0) return;
  mouseDown = { x: event.clientX, y: event.clientY };
  if (tool === 'move') {
    const result = hitItem(event);
    if (!result) return;
    selected = result.index;
    const item = doc.items[selected];
    const point = intersectHorizontal(event, item.position[1]);
    if (!point) return;
    drag = { index: selected, before: JSON.stringify(doc), offset: [item.position[0] - point.x, item.position[2] - point.z], height: item.position[1] };
    updateInspector(); updateTree();
  }
});

renderer.domElement.addEventListener('pointermove', event => {
  if (!drag || !mouseDown) return;
  const point = intersectHorizontal(event, drag.height);
  if (!point) return;
  const step = $('snap').checked ? Number($('gridSize').value) : 1;
  const item = doc.items[drag.index];
  item.position[0] = Math.round((point.x + drag.offset[0]) / step) * step;
  item.position[2] = Math.round((point.z + drag.offset[1]) / step) * step;
  dirty = true; drawPlacedTransform(drag.index); updateInspector(); updateTree(); $('dirtyMark').classList.add('on');
});

renderer.domElement.addEventListener('pointerup', event => {
  if (event.button !== 0 || !mouseDown) return;
  const moved = Math.hypot(event.clientX - mouseDown.x, event.clientY - mouseDown.y) > 4;
  if (tool === 'move' && drag && moved) { history.push(drag.before); future = []; markDirty(); }
  if (!moved && tool === 'select') {
    const result = hitItem(event);
    setSelected(result?.index ?? -1);
  } else if (!moved && tool === 'place') {
    const point = intersectHorizontal(event, Number($('placeY').value) || 0);
    if (point) placeAt(point);
  }
  drag = null; mouseDown = null;
});

function nudge(dx, dy, dz, large = false) {
  if (selected < 0) return;
  snapshot(); const position = doc.items[selected].position; const unit = large ? 32 : 8;
  position[0] += dx * unit; position[1] += dy * unit; position[2] += dz * unit; rebuild();
}

function rotateSelected(amount) {
  if (selected < 0) return;
  snapshot(); const item = doc.items[selected]; item.rotation = ((item.rotation || 0) + amount + 360) % 360; rebuild();
}

function undo() {
  if (!history.length) return;
  future.push(JSON.stringify(doc)); doc = JSON.parse(history.pop()); dirty = true; fields(); rebuild();
}

function redo() {
  if (!future.length) return;
  history.push(JSON.stringify(doc)); doc = JSON.parse(future.pop()); dirty = true; fields(); rebuild();
}

function deleteSelected() {
  if (selected < 0) return;
  snapshot(); doc.items.splice(selected, 1); selected = -1; setTool('select'); rebuild();
}

window.addEventListener('keydown', event => {
  if (/input|select|textarea/i.test(event.target.tagName)) return;
  if ((event.metaKey || event.ctrlKey) && event.key.toLowerCase() === 'z') { event.preventDefault(); event.shiftKey ? redo() : undo(); return; }
  if (event.key === 'Escape') { setTool('select'); $('textureDialog').hidden = true; return; }
  if (event.key === 'Delete' || event.key === 'Backspace') { if (selected >= 0) { event.preventDefault(); deleteSelected(); } return; }
  if (event.key === 'q' || event.key === 'Q') rotateSelected(-15);
  if (event.key === 'e' || event.key === 'E') rotateSelected(15);
  const directions = { ArrowLeft: [-1, 0, 0], ArrowRight: [1, 0, 0], ArrowUp: [0, 0, -1], ArrowDown: [0, 0, 1] };
  if (directions[event.key]) { const direction = directions[event.key]; nudge(...direction, event.shiftKey); }
});

$('delete').onclick = $('deleteItem').onclick = deleteSelected;
$('duplicate').onclick = () => {
  if (selected < 0) return;
  if (doc.items[selected].kind === 'spawn') { say('A level can have one player spawn. Move the existing spawn instead.'); return; }
  snapshot(); const copy = structuredClone(doc.items[selected]); copy.position[0] += 32; copy.position[2] += 32;
  doc.items.push(copy); selected = doc.items.length - 1; rebuild();
};
$('undo').onclick = undo; $('redo').onclick = redo;
$('objects').onchange = () => setSelected(Number($('objects').value));
$('selectTool').onclick = () => setTool('select');
$('moveTool').onclick = () => setTool('move');
$('placeTool').onclick = placeSelectedAtCursor;
$('placeSelected').onclick = placeSelectedAtCursor;
$('cancelPlacement').onclick = () => setTool('select');
$('rotateMinus').onclick = () => rotateSelected(-15); $('rotatePlus').onclick = () => rotateSelected(15);
$('roof').onclick = () => { showRoof = !showRoof; applyRoofVisibility(); };
$('fit').onclick = () => {
  const bounds = new T.Box3().setFromObject(placed);
  if (bounds.isEmpty()) { orbit.target.set(0, 40, 0); camera.position.set(1400, 1400, 1800); }
  else { const center = bounds.getCenter(new T.Vector3()), size = bounds.getSize(new T.Vector3()); const distance = Math.max(...size.toArray(), 500) * 1.5; orbit.target.copy(center); camera.position.copy(center).add(new T.Vector3(distance, distance * 0.8, distance)); }
  orbit.update();
};
$('frame').onclick = () => {
  const object = placed.children.find(child => child.userData.index === selected);
  if (!object) return;
  const bounds = new T.Box3().setFromObject(object), center = bounds.getCenter(new T.Vector3());
  const distance = Math.max(...bounds.getSize(new T.Vector3()).toArray(), 100) * 2;
  orbit.target.copy(center); camera.position.copy(center).add(new T.Vector3(distance, distance * 0.7, distance)); orbit.update();
};

for (const button of document.querySelectorAll('#assetTabs button')) button.onclick = () => setTab(button.dataset.tab);
$('assetSearch').oninput = updateAssets; $('mapFilter').onchange = updateAssets; $('categoryFilter').onchange = updateAssets;
$('textureSearch').oninput = renderTextures; $('closeTextures').onclick = () => $('textureDialog').hidden = true;
$('title').onchange = () => { snapshot(); doc.title = $('title').value; };
$('key').onchange = () => { snapshot(); doc.key = $('key').value; };

function fields() {
  $('title').value = doc.title; $('key').value = doc.key; $('dirtyMark').classList.toggle('on', dirty);
}

function opened(level) {
  doc = structuredClone(level); selected = -1; history = []; future = []; dirty = false; fields(); rebuild(); say(`Opened ${doc.title}`);
}

let saved = [];
async function refreshProjects(keepKey = '') {
  saved = await getJSON('/api/levels');
  $('projects').replaceChildren(...saved.map(level => new Option(level.title, level.key)));
  if (keepKey && saved.some(level => level.key === keepKey)) $('projects').value = keepKey;
}

$('load').onclick = () => {
  if (dirty && !confirm('Discard unsaved changes?')) return;
  const level = saved.find(item => item.key === $('projects').value);
  if (level) opened(level);
};
$('new').onclick = async () => {
  if (dirty && !confirm('Discard unsaved changes?')) return;
  const level = await getJSON('./examples/empty_room.json');
  level.key = `custom_room_${Date.now().toString(36)}`; level.title = 'Untitled Level'; opened(level); markDirty();
};

async function save(test = false) {
  $('save').disabled = $('test').disabled = true;
  try {
    doc.title = $('title').value; doc.key = $('key').value;
    const response = await fetch(test ? '/api/test' : '/api/save', {
      method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(doc)
    });
    const result = await response.json();
    if (!response.ok) throw Error(result.error);
    dirty = false; fields(); say(result.message); await refreshProjects(doc.key);
  } catch (error) { say(error.message, true); }
  finally { $('save').disabled = $('test').disabled = false; }
}
$('save').onclick = () => save(false); $('test').onclick = () => save(true);
$('dirtyMark').setAttribute('aria-live', 'polite');
window.addEventListener('beforeunload', event => { if (dirty) { event.preventDefault(); event.returnValue = ''; } });

const resize = new ResizeObserver(() => {
  const width = $('view').clientWidth, height = $('view').clientHeight;
  if (width && height) { renderer.setSize(width, height); camera.aspect = width / height; camera.updateProjectionMatrix(); }
  const previewWidth = $('preview').clientWidth, previewHeight = $('preview').clientHeight;
  if (previewWidth && previewHeight) { previewRenderer.setSize(previewWidth, previewHeight); previewCamera.aspect = previewWidth / previewHeight; previewCamera.updateProjectionMatrix(); }
});
resize.observe($('view')); resize.observe($('preview'));
renderer.setAnimationLoop(() => {
  animateFires(performance.now());
  orbit.update(); renderer.render(scene, camera); previewOrbit.update(); previewRenderer.render(previewScene, previewCamera);
});

try {
  inventory = await getJSON('/api/inventory');
  updateFilters(); updateAssets(); await refreshProjects();
  const initial = saved.find(level => level.key === 'custom_test_room') || saved[0];
  if (initial) opened(initial); else opened(await getJSON('./examples/empty_room.json'));
  setTool('select');
  say('Asset browser ready · Select is camera navigation; use Move or Place when editing the scene.');
} catch (error) {
  say(`Asset library could not load: ${error.message}. Restart the editor server and refresh this page.`, true);
}
