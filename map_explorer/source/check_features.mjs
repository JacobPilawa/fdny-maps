// Small data/motion regression check; visual testing remains in the explorer.
import fs from 'node:fs';
import path from 'node:path';
import {fileURLToPath,pathToFileURL} from 'node:url';
import assert from 'node:assert/strict';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const source=fs.readFileSync(path.join(root,'animation.js'),'utf8').replace("from 'three'","from '"+pathToFileURL(path.join(root,'vendor/three.module.min.js')).href+"'");
const animation=await import('data:text/javascript;base64,'+Buffer.from(source).toString('base64'));
const info=await import(pathToFileURL(path.join(root,'script-info.js')));
const read=name=>JSON.parse(fs.readFileSync(path.join(root,'data',name),'utf8'));
let markers=0,clips=0;const categories={};
for(const map of read('manifest.json')){
 const d=read(map.key+'.json'),tracks=read('animations/'+map.key+'.json');
 assert.equal(new Set(d.markers.map(m=>m.id)).size,d.markers.length,'unique IDs '+map.key);
 for(const m of d.markers){categories[m.category]=(categories[m.category]||0)+1;assert([m.x,m.y,m.z].every(Number.isFinite));}
 for(const clip of d.features.clips){assert(clip.duration>0);for(const p of clip.parts){assert(tracks[p.model]);assert(d.models[p.model]);}for(const time of [0,clip.duration/2,clip.duration])for(const pose of animation.motionStates(clip,time,tracks).values())assert([...pose.translation.toArray(),...pose.rotation.toArray()].every(Number.isFinite));}
 for(const link of d.features.links)assert(d.markers.some(m=>m.id===link.from)&&d.markers.some(m=>m.id===link.to));
 for(const z of d.features.zones)assert(z.markers.every(id=>d.markers.some(m=>m.id===id&&m.bounds)));
 // Polygon identity/UV order must remain compatible with the existing texture dataset.
 const textures=read('textures/'+map.key+'.json');assert.equal(d.polygons.length,textures.polygons.length);
 if(['oil_rig_b','oil_rig_c','bank','bar_r','boat_b'].includes(map.key)){
  const world=animation.buildBrushWorld(d);for(const mesh of world.children){animation.applyMotion(mesh,new Map());const id=mesh.userData.ids[0],local=Array.from(mesh.geometry.attributes.position.array.slice(0,3));const pos=local.map((v,i)=>v+mesh.position.toArray()[i]);assert(pos.every((v,i)=>Math.abs(v-d.polygons[id].p[0][i])<.002),'rest geometry '+map.key);mesh.geometry.dispose();mesh.material.dispose();}
 }
 markers+=d.markers.length;clips+=d.features.clips.length;
}
const oil=read('oil_rig_b.json').features.clips.find(c=>c.id==='warehouse-catwalks');assert.deepEqual(oil.parts.map(p=>p.delay),[0,.5,1]);assert.equal(oil.duration,2);
assert.equal(categories.BONUS,78);assert.equal(categories.EMITTER,230);assert.equal(categories.REFILL,17);
assert(info.scriptRows('makezone (Break Room)').every(r=>r.name!=='break'));
assert(info.scriptRows('touch: ifclear Tower (cmd [(tell TowerStairs open), (wait 1)])').some(r=>r.name==='ifclear'));
console.log({maps:17,markers,clips,categories});
