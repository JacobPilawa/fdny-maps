import * as THREE from 'three';
import {sampleChannel} from './animation.js';
const bodies=new Map();
async function bodyFor(stem){if(!bodies.has(stem))bodies.set(stem,fetch(`data/actors/${stem}.json`).then(r=>{if(!r.ok)throw Error('Victim asset '+r.status);return r.json();}).catch(e=>{bodies.delete(stem);throw e;}));return bodies.get(stem);}
function stemFor(type){type=(type||'man').toLowerCase();return 'vic_'+(type.startsWith('fat')?'fatty':type.startsWith('hero')?'herodown':['woman','girl','boy','dog'].find(t=>type.startsWith(t))||'man');}
function matrix(v){return new THREE.Matrix4().set(v[0],v[1],v[2],v[9],v[3],v[4],v[5],v[10],v[6],v[7],v[8],v[11],0,0,0,1);}
export async function buildVictimModels(data){const group=new THREE.Group(),loader=new THREE.TextureLoader(),textureCache=new Map();try{
 const victims=data.markers.filter(m=>m.category==='VICTIM');await Promise.all(victims.map(async m=>{
  const stem=stemFor(m.raw.D_Type),body=await bodyFor(stem),script=m.expanded_scripts||m.scripts,skin=/\bsetskin\s+(\d+)/i.exec(script),pose=/\bplayanim\s+(\d+)/i.exec(script);
  const motion=pose?body.motions[Number(pose[1])]:body.motions.find(p=>/^(onback|down)$/i.test(p.name))||body.motions.find(p=>/idle/i.test(p.name))||body.motions[0];
  const skinIndex=skin?body.materials.findIndex(s=>s.name.toLowerCase()==='skin'+skin[1]):0,textureIndex=Math.max(0,skinIndex),textureKey=stem+':'+textureIndex;
  if(!textureCache.has(textureKey))textureCache.set(textureKey,loader.loadAsync(`data/actors/${stem}_${textureIndex}.png`).then(t=>{t.colorSpace=THREE.SRGBColorSpace;t.flipY=false;return t;}));
  const texture=await textureCache.get(textureKey),paths=new Map((motion?.paths||[]).map(p=>[p.name.toLowerCase(),p])),transforms=[];
  body.bones.forEach((bone,i)=>{const path=paths.get(bone.name.toLowerCase()),local=new THREE.Matrix4().compose(sampleChannel(path?.translation,motion?.start||0),sampleChannel(path?.rotation,motion?.start||0,true),new THREE.Vector3(1,1,1)),transform=matrix(bone.attachment).multiply(local);transforms[i]=bone.parent<0?transform:transforms[bone.parent].clone().multiply(transform);});
  const positions=[],uvs=[];for(const face of body.lods[0])for(const index of face.vertices){const v=body.vertices[index],p=new THREE.Vector3(...v.position).applyMatrix4(transforms[v.bone]);positions.push(p.x,p.z,-p.y);uvs.push(...v.uv);}
  const geometry=new THREE.BufferGeometry();geometry.setAttribute('position',new THREE.Float32BufferAttribute(positions,3));geometry.setAttribute('uv',new THREE.Float32BufferAttribute(uvs,2));geometry.computeVertexNormals();
  const mesh=new THREE.Mesh(geometry,new THREE.MeshLambertMaterial({map:texture,side:THREE.DoubleSide,alphaTest:.5}));mesh.scale.setScalar(2);mesh.userData={marker:m,pose:motion?.name,stem,skin:textureIndex};group.add(mesh);
 }));return group;
 }catch(e){group.traverse(o=>{o.geometry?.dispose();o.material?.dispose();});for(const p of textureCache.values())p.then(t=>t.dispose()).catch(()=>{});throw e;}}
