import * as THREE from 'three';

export function buildCollisionOverlay(data) {
  const group = new THREE.Group();
  const buckets = {wall: {triangles: [], edges: []}, floor: {triangles: [], edges: []}, roof: {triangles: [], edges: []}};
  for (const polygon of data.polygons) {
    const points = polygon.p;
    const bucket = buckets[polygon.n[1] > .6 ? 'floor' : polygon.n[1] < -.6 ? 'roof' : 'wall'];
    for (let i = 1; i < points.length - 1; i++) bucket.triangles.push(...points[0], ...points[i], ...points[i + 1]);
    for (let i = 0; i < points.length; i++) bucket.edges.push(...points[i], ...points[(i + 1) % points.length]);
  }
  for (const [type, bucket] of Object.entries(buckets)) {
    const geometry = new THREE.BufferGeometry();
    geometry.setAttribute('position', new THREE.Float32BufferAttribute(bucket.triangles, 3));
    const mesh = new THREE.Mesh(geometry, new THREE.MeshBasicMaterial({
      color: 0xe33c3c, transparent: true, opacity: .16, side: THREE.DoubleSide,
      depthWrite: false, polygonOffset: true, polygonOffsetFactor: -2, polygonOffsetUnits: -2
    }));
    mesh.userData.type = type;
    mesh.renderOrder = 2;
    group.add(mesh);
    const edgeGeometry = new THREE.BufferGeometry();
    edgeGeometry.setAttribute('position', new THREE.Float32BufferAttribute(bucket.edges, 3));
    const edges = new THREE.LineSegments(edgeGeometry, new THREE.LineBasicMaterial({
      color: 0xc92323, transparent: true, opacity: .24, depthWrite: false
    }));
    edges.userData.type = type;
    edges.renderOrder = 3;
    group.add(edges);
  }
  group.userData.key = data.key;
  return group;
}
