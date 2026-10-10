import * as THREE from 'three';
import {animatedModels,localPoint,tagModel} from './animation.js';

export function buildCollisionOverlay(data,mapData) {
  const group = new THREE.Group();
  const buckets=new Map(),dynamic=animatedModels(mapData);
  for (const polygon of data.polygons) {
    const model=dynamic.has(polygon.model)?polygon.model:null,points=polygon.p.map(p=>localPoint(p,model,mapData));
    const type=polygon.n[1]>.6?'floor':polygon.n[1]<-.6?'roof':'wall',key=type+':'+model;
    if(!buckets.has(key))buckets.set(key,{type,model,triangles:[],edges:[]});
    const bucket=buckets.get(key);
    for (let i = 1; i < points.length - 1; i++) bucket.triangles.push(...points[0], ...points[i], ...points[i + 1]);
    for (let i = 0; i < points.length; i++) bucket.edges.push(...points[i], ...points[(i + 1) % points.length]);
  }
  for (const bucket of buckets.values()) {
    const type=bucket.type;
    const geometry = new THREE.BufferGeometry();
    geometry.setAttribute('position', new THREE.Float32BufferAttribute(bucket.triangles, 3));
    const mesh = new THREE.Mesh(geometry, new THREE.MeshBasicMaterial({
      color: 0xe33c3c, transparent: true, opacity: .16, side: THREE.DoubleSide,
      depthWrite: false, polygonOffset: true, polygonOffsetFactor: -2, polygonOffsetUnits: -2
    }));
    mesh.userData.type = type;
    mesh.renderOrder = 2;
    tagModel(mesh,bucket.model,mapData);
    group.add(mesh);
    const edgeGeometry = new THREE.BufferGeometry();
    edgeGeometry.setAttribute('position', new THREE.Float32BufferAttribute(bucket.edges, 3));
    const edges = new THREE.LineSegments(edgeGeometry, new THREE.LineBasicMaterial({
      color: 0xc92323, transparent: true, opacity: .24, depthWrite: false
    }));
    edges.userData.type = type;
    edges.renderOrder = 3;
    tagModel(edges,bucket.model,mapData);
    group.add(edges);
  }
  group.userData.key = data.key;
  return group;
}
