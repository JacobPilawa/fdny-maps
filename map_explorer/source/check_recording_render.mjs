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
  r.host={state:()=>state,focusPoint:p=>followed=p,focusBounds:b=>snapped=b,redraw:()=>redraws++};r.updateSingle3D();assert.equal(state.scene.children.length,1);
  const full=r.lines[0].line.geometry.instanceCount;assert(full>0);
  r.time=r.trace.duration/2;r.mode='scrub';r.updateSingle3D();assert(r.lines[0].line.geometry.instanceCount<full);
  r.thickness=11;r.playerColor='#ff00aa';r.updateSingle3D();assert.equal(r.lines[0].line.material.uniforms.width.value,11);assert.equal(r.proxies[0].pill.material.color.getHexString(),'ff00aa');
  const controller=Object.create(RecordingMode.prototype);Object.assign(controller,{recordings:[r],trace:r.trace,followIndex:0,host:r.host,follow:true,sync:()=>{}});controller.seek(150);assert(followed);assert(redraws>0);controller.snap();assert(snapped);assert.equal(snapped.max[0]-snapped.min[0],1100);
  for(const [stateRaw,height] of [[4,55],[3,115]]){
    const s=r.trace.samples.find(s=>s.player_state_raw===stateRaw);if(!s)continue;
    r.time=s.t_ms;r.updateSingle3D();assert.equal(r.proxies[0].box.scale.y,height);assert.equal(r.proxies[0].box.scale.x,60);
  }
  if(r.trace.victims.length){
    r.time=200;r.updateSingle3D();assert(r.proxies[1].group.visible);assert.equal(r.proxies[1].group.position.x,50);
    r.time=2000;r.updateSingle3D();assert.equal(r.proxies[1].group.visible,false);
  }
  state.data.key='bank';r.updateSingle3D();assert.equal(r.group.visible,false);state.data.key='training';r.updateSingle3D();assert.equal(r.group.visible,true);
  let strokes=0,boxes=0;const widths=[];const c=new Proxy({measureText:t=>({width:t.length*7}),stroke:()=>strokes++,strokeRect:()=>boxes++},{get:(o,k)=>k in o?o[k]:(()=>{}),set:(o,k,v)=>{if(k==='lineWidth')widths.push(v);o[k]=v;return true;}});
  r.drawSingle2D(c);assert(strokes>0&&boxes>0);assert(widths.includes(11));
  let disposed=0;r.group.traverse(o=>o.geometry?.addEventListener('dispose',()=>disposed++));r.dispose();assert(disposed>0);assert.equal(state.scene.children.length,0);
}
console.log('PASS: 2D draw, 3D tracks, scrubbing, player hull states, victim positions/lifetimes, map mismatch and GPU cleanup');

// One clock drives unequal runs and freezes the shorter player in both renderers.
const state={data:{key:'training'},scene:new THREE.Scene(),mode:'3d',width:800,height:600,heightClip:[],low:-Infinity,high:Infinity};
const runs=[1000,2000].map((duration,i)=>{
  const r=Object.create(RecordingMode.prototype);
  const trace=parseRecording([start,...samples.filter(s=>s.t_ms<=duration)].map(s=>JSON.stringify(s)).join('\n'));
  Object.assign(r,{trace,host:{state:()=>state},time:0,mode:'scrub',group:null,showPlayer:true,showVictims:true,xray:true,playerColor:i?'#2878c7':'#d33f49',thickness:3});return r;
});
const controller=Object.create(RecordingMode.prototype);Object.assign(controller,{recordings:runs,time:1500});
assert.equal(controller.duration,2000);controller.update3D();
assert.equal(runs[0].time,1000);assert.equal(runs[1].time,1500);
assert.equal(runs[0].proxies[0].group.position.x,100);assert.equal(runs[1].proxies[0].group.position.x,150);
assert.notEqual(runs[0].proxies[0].pill.material.color.getHexString(),runs[1].proxies[0].pill.material.color.getHexString());
controller.time=2000;controller.update3D();assert.equal(runs[0].proxies[0].group.position.x,100);assert.equal(runs[1].proxies[0].group.position.x,200);
controller.dispose();assert.equal(state.scene.children.length,0);
console.log('PASS: shared timeline, distinct colors, unequal recording durations, final-frame hold and multi-recording cleanup');

// Automatic single-run gradients and explicit metric coloring share both render paths.
const one=runs[0];one.multi=false;one.trailColorMode='auto';
assert.equal(one.effectiveTrailMode(),'time');assert.notEqual(one.trailTint(0),one.trailTint(one.trace.samples.length-1));
one.multi=true;assert.equal(one.trailTint(0),one.playerColor);
for(const mode of ['time','distance','speed','acceleration']){one.trailColorMode=mode;one.dispose();one.updateSingle3D();assert(one.lines[0].line.geometry.attributes.colorStart);}
controller.dispose();
const {trailMetrics}=await import(url('recording-data.js'));
const points=[{t_ms:0,position:[0,0,0]},{t_ms:100,position:[10,0,0]},{t_ms:200,position:[30,0,0]},{t_ms:300,position:[60,0,0]},{t_ms:10000,position:[10000,0,0]},{t_ms:10100,position:[10010,0,0]}];
const metrics=trailMetrics(points);
assert.equal(metrics.speed[1],100);assert.equal(metrics.speed[2],200);assert.equal(metrics.acceleration[2],1000);
assert(metrics.speedRank[1]<metrics.speedRank[2]);assert(metrics.speedRank[2]<metrics.speedRank[3]);
assert.equal(metrics.speed[4],null);assert.equal(metrics.acceleration[5],null);assert.equal(metrics.distance[4],60);
assert.equal(trailMetrics([{t_ms:0,position:[0,0,0]},{t_ms:0,position:[1,0,0]}]).speed[1],null);
console.log('PASS: automatic gradients, metric render modes, speed/acceleration, percentiles, gap and duplicate-time handling');

// Verify actual GPU color values, not just attribute presence: CSS HSL syntax
// accepted by canvas is not necessarily understood by Three.Color.set().
one.multi=false;
for(const mode of ['time','distance','speed','acceleration','player']){
  one.trailColorMode=mode;one.dispose();one.updateSingle3D();
  const colors=one.lines[0].line.geometry.attributes.colorStart;
  assert(colors.count>0);
  for(let i=0;i<colors.count;i++)assert(Math.min(colors.getX(i),colors.getY(i),colors.getZ(i))<.99,mode+' must not produce white segments');
  if(mode==='time'||mode==='distance')assert.notDeepEqual([colors.getX(0),colors.getY(0),colors.getZ(0)],[colors.getX(colors.count-1),colors.getY(colors.count-1),colors.getZ(colors.count-1)]);
}
one.dispose();console.log('PASS: GPU trail colors stay nonwhite in every mode and rainbow endpoints differ');
