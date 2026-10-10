import * as THREE from 'three';
import {animatedModels,localPoint,tagModel} from './animation.js';

export async function buildTexturedWorld(data, polygons, anisotropy, mapData) {
  const loader = new THREE.TextureLoader();
  const textures = new Map();
  try {
    const results = await Promise.allSettled(Object.entries(data.textures).map(async ([id, info]) => {
      const texture = await loader.loadAsync('data/textures/images/' + info.file);
      texture.colorSpace = THREE.SRGBColorSpace;
      texture.wrapS = texture.wrapT = THREE.RepeatWrapping;
      texture.anisotropy = Math.min(anisotropy, 8);
      textures.set(id, texture);
    }));
    const failed = results.find(r => r.status === 'rejected');
    if (failed) throw failed.reason;
    const buckets = new Map(), dynamic=animatedModels(mapData);
    polygons.forEach((polygon, id) => {
      const info = data.polygons[id];
      const type = polygon.n[1] > .6 ? 'floor' : polygon.n[1] < -.6 ? 'roof' : 'wall';
      const model=dynamic.has(polygon.model)?polygon.model:null;
      const key = type + ':' + info.texture+':'+model;
      if (!buckets.has(key)) buckets.set(key, {type, model, texture: info.texture, positions: [], uvs: [], ids: []});
      const bucket = buckets.get(key);
      for (let i = 1; i < polygon.p.length - 1; i++) {
        for (const j of [0, i, i + 1]) {
          bucket.positions.push(...localPoint(polygon.p[j],model,mapData));
          bucket.uvs.push(...info.uv[j]);
        }
        bucket.ids.push(id);
      }
    });
    const group = new THREE.Group();
    group.userData.key = data.key;
    for (const bucket of buckets.values()) {
      const geometry = new THREE.BufferGeometry();
      geometry.setAttribute('position', new THREE.Float32BufferAttribute(bucket.positions, 3));
      geometry.computeVertexNormals();
      geometry.setAttribute('uv', new THREE.Float32BufferAttribute(bucket.uvs, 2));
      const mesh = new THREE.Mesh(geometry, new THREE.MeshLambertMaterial({map: textures.get(String(bucket.texture)), color: 0xffffff, side: THREE.DoubleSide}));
      mesh.userData = {type: bucket.type, textured: true, ids: bucket.ids};
      tagModel(mesh,bucket.model,mapData);
      group.add(mesh);
    }
    return group;
  } catch (error) {
    textures.forEach(texture => texture.dispose());
    throw error;
  }
}
