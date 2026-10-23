// Renderer checks run against Three.js in Node; no browser or game controls.
import fs from 'node:fs';
import path from 'node:path';
import {pathToFileURL,fileURLToPath} from 'node:url';
import assert from 'node:assert/strict';
import {parseRecording} from '../recording-data.js';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const url=f=>pathToFileURL(path.join(root,f)).href;
const THREE=await import(url('vendor/three.module.min.js'));
const source=fs.readFileSync(path.join(root,'recording.js'),'utf8')
  .replace("from 'three'",`from '${url('vendor/three.module.min.js')}'`)
  .replace("from './recording-data.js'",`from '${url('recording-data.js')}'`);
const {RecordingMode}=await import('data:text/javascript;base64,'+Buffer.from(source).toString('base64'));
const start={type:'start',schema:'fdny-trajectory-v1',stage:'training'};
const samples=Array.from({length:30},(_,n)=>({type:'sample',n,t_ms:n*100,position:[n*10,-100,0],body_forward:[0,0,1],player_state_raw:n===10?4:3}));
const victim={type:'victim_sample',id:'v001',name:'victim1',t_ms:0,position:[0,0,0],bounds_min:[-24,-24,-24],bounds_max:[24,24,24]};
const fixture=[start,...samples,victim,{...victim,t_ms:400,position:[100,0,0]},{type:'victim_end',id:'v001',t_ms:2000}].map(r=>JSON.stringify(r)).join('\n')+'\n';
for(const input of [fixture,...process.argv.slice(2).map(p=>fs.readFileSync(p,'utf8'))]){
  const r=Object.create(RecordingMode.prototype);
  Object.assign(r,{trace:parseRecording(input),mode:'full',time:0,showPlayer:true,showVictims:true,xray:true,group:null,playerColor:'#ff0000',thickness:3});
  const state={data:{key:'training'},scene:new THREE.Scene(),mode:'3d',width:1280,height:720,heightClip:[],low:-Infinity,high:Infinity,scale:.1,offsetX:0,offsetY:0};
  let followed=null,snapped=null,redraws=0;
  r.host={state:()=>state,focusPoint:p=>followed=p,focusBounds:b=>snapped=b,redraw:()=>redraws++};r.update3D();assert.equal(state.scene.children.length,1);
  const full=r.lines[0].line.geometry.instanceCount;assert(full>0);
  r.time=r.trace.duration/2;r.mode='scrub';r.update3D();assert(r.lines[0].line.geometry.instanceCount<full);
  r.thickness=11;r.playerColor='#ff00aa';r.update3D();assert.equal(r.lines[0].line.material.uniforms.width.value,11);assert.equal(r.proxies[0].pill.material.color.getHexString(),'ff00aa');
  r.sync=()=>{};r.follow=true;r.seek(150);assert(followed);assert(redraws>0);r.snap();assert(snapped);assert.equal(snapped.max[0]-snapped.min[0],800);r.follow=false;
  for(const [stateRaw,height] of [[4,55],[3,115]]){
    const s=r.trace.samples.find(s=>s.player_state_raw===stateRaw);if(!s)continue;
    r.time=s.t_ms;r.update3D();assert.equal(r.proxies[0].box.scale.y,height);assert.equal(r.proxies[0].box.scale.x,60);
  }
  if(r.trace.victims.length){
    r.time=200;r.update3D();assert(r.proxies[1].group.visible);assert.equal(r.proxies[1].group.position.x,50);
    r.time=2000;r.update3D();assert.equal(r.proxies[1].group.visible,false);
  }
  state.data.key='bank';r.update3D();assert.equal(r.group.visible,false);state.data.key='training';r.update3D();assert.equal(r.group.visible,true);
  let strokes=0,boxes=0;const widths=[];const c=new Proxy({measureText:t=>({width:t.length*7}),stroke:()=>strokes++,strokeRect:()=>boxes++},{get:(o,k)=>k in o?o[k]:(()=>{}),set:(o,k,v)=>{if(k==='lineWidth')widths.push(v);o[k]=v;return true;}});
  r.draw2D(c);assert(strokes>0&&boxes>0);assert(widths.includes(11));
  let disposed=0;r.group.traverse(o=>o.geometry?.addEventListener('dispose',()=>disposed++));r.dispose();assert(disposed>0);assert.equal(state.scene.children.length,0);
}
console.log('PASS: 2D draw, 3D tracks, scrubbing, player hull states, victim positions/lifetimes, map mismatch and GPU cleanup');
