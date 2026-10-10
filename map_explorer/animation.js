import * as THREE from 'three';

export function sampleChannel(channel,time,rotation=false){
 const values=channel?.values,times=channel?.times;if(!values?.length)return rotation?new THREE.Quaternion():new THREE.Vector3();
 let a=0;while(a+1<times.length&&times[a+1]<=time)a++;
 const b=time<=times[0]?0:Math.min(a+1,times.length-1),t=a===b?0:THREE.MathUtils.clamp((time-times[a])/(times[b]-times[a]),0,1);
 if(rotation)return new THREE.Quaternion(...values[a]).slerp(new THREE.Quaternion(...values[b]),t).normalize();
 if(a===b)return new THREE.Vector3(...values[a]);
 if(channel.interpolation===1||channel.interpolation===2){
  const tangent=(i,out)=>{if(channel.interpolation===2)return [0,0,0];if(i===0)return values[1].map((v,j)=>v-values[0][j]);if(i===values.length-1)return values[i].map((v,j)=>v-values[i-1][j]);const n0=times[i]-times[i-1],n1=times[i+1]-times[i];return values[i+1].map((v,j)=>(v-values[i-1][j])*(out?n1:n0)/(n0+n1));};
  const ta=tangent(a,true),tb=tangent(b,false),t2=t*t,t3=t2*t;
  return new THREE.Vector3(...values[a].map((v,j)=>(2*t3-3*t2+1)*v+(-2*t3+3*t2)*values[b][j]+(t3-2*t2+t)*ta[j]+(t3-t2)*tb[j]));
 }
 return new THREE.Vector3(...values[a]).lerp(new THREE.Vector3(...values[b]),t);
}
export function animatedModels(data){return new Set(data.features.clips.flatMap(c=>c.parts.map(p=>p.model)));}
export function localPoint(point,model,data){const origin=model==null?[0,0,0]:data.models[model].origin;return point.map((v,i)=>v-origin[i]);}
export function tagModel(object,model,data){if(model==null)return;object.userData.model=model;object.userData.origin=data.models[model].origin;object.position.set(...object.userData.origin);}
export function applyMotion(object,states){const id=object.userData.model;if(id==null)return;const origin=object.userData.origin,pose=states.get(id);object.position.set(...origin);object.quaternion.identity();if(pose){object.position.add(pose.translation);object.quaternion.copy(pose.rotation);}if(object.userData.localCenter)object.position.add(new THREE.Vector3(...object.userData.localCenter).applyQuaternion(object.quaternion));}
export function motionStates(clip,time,tracks){const states=new Map();if(!clip)return states;for(const part of clip.parts){const track=tracks[String(part.model)];if(!track)continue;const t=Math.max(0,time-part.delay)*part.rate;states.set(part.model,{translation:sampleChannel(track.translation,t),rotation:sampleChannel(track.rotation,t,true)});}return states;}
export function buildBrushWorld(data){const dynamic=animatedModels(data),buckets=new Map(),group=new THREE.Group();for(const [id,p] of data.polygons.entries()){const type=p.n[1]>.6?'floor':p.n[1]<-.6?'roof':'wall',model=dynamic.has(p.model)?p.model:null,key=type+':'+model;if(!buckets.has(key))buckets.set(key,{type,model,positions:[],ids:[]});const b=buckets.get(key);for(let i=1;i<p.p.length-1;i++){for(const j of [0,i,i+1])b.positions.push(...localPoint(p.p[j],model,data));b.ids.push(id);}}
 for(const b of buckets.values()){const g=new THREE.BufferGeometry();g.setAttribute('position',new THREE.Float32BufferAttribute(b.positions,3));g.computeVertexNormals();const mesh=new THREE.Mesh(g,new THREE.MeshLambertMaterial({color:b.type==='floor'?0xd4dbcb:b.type==='wall'?0xa7b5ad:0xbac3bb,side:THREE.DoubleSide}));mesh.userData={type:b.type,ids:b.ids};tagModel(mesh,b.model,data);group.add(mesh);}return group;
}
