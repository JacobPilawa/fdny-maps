import assert from 'node:assert/strict';
import fs from 'node:fs';
import {parseRecording,boundsFor,frameAt,canJoin} from '../recording-data.js';
const start={type:'start',schema:'fdny-trajectory-v1',stage:'Maps/Training.bsp'};
const sample=(n,t,x,state=3)=>({type:'sample',n,t_ms:t,position:[x,0,0],body_forward:[0,0,1],pitch_deg:0,player_state_raw:state});
const text=rs=>rs.map(r=>JSON.stringify(r)).join('\n')+'\n';
const trace=parseRecording(text([start,sample(0,0,0),sample(1,100,10,4),sample(2,200,20)]));
assert.equal(trace.stage,'training');assert.equal(trace.distance,20);assert.equal(frameAt(trace.samples,50).position[0],5);
assert.equal(boundsFor(frameAt(trace.samples,99)).max[1],85);assert.equal(boundsFor(frameAt(trace.samples,100)).max[1],25);
assert.deepEqual(boundsFor({...sample(0,0,0),bounds_min:[-1,-2,-3],bounds_max:[1,2,3]}).max,[1,2,3]);
assert.equal(frameAt(trace.samples,-100).position[0],0);assert.equal(frameAt(trace.samples,9999).position[0],20);
assert.equal(canJoin(sample(0,0,0),sample(1,10000,50)),false);
const gap=parseRecording(text([start,sample(0,0,0),sample(1,1000,50)]));assert.equal(frameAt(gap.samples,500).position[0],0);
assert.throws(()=>parseRecording(text([start,sample(1,0,0)])),/out-of-order/);
assert.throws(()=>parseRecording(text([start,{...sample(0,0,0),position:[null,0,0]}])),/Invalid/);
assert.throws(()=>parseRecording(text([start,sample(0,0,0),{...sample(1,100,1),bounds_min:[5,0,0],bounds_max:[0,1,1]}])),/bounds/);
assert.throws(()=>parseRecording(text([start,sample(0,0,0),start])),/one run/);
assert.throws(()=>parseRecording(text([start,sample(0,100,0),sample(1,50,1)])),/out-of-order/);
const truncated=parseRecording(text([start,sample(0,0,0)])+'{"type":"sam');assert(truncated.warnings.some(w=>w.includes('unfinished')));
const victim={type:'victim_sample',id:'v1',name:'victim1',t_ms:0,position:[1,2,3],state_raw:7};
const extended=parseRecording(text([start,sample(0,0,0),victim,{...victim,t_ms:50,position:[2,2,3]},{type:'victim_end',id:'v1',t_ms:100}]));
assert.equal(extended.victims.length,1);assert.equal(extended.victims[0].end,100);assert.equal(frameAt(extended.victims[0].samples,25).position[0],1.5);
for(const file of process.argv.slice(2)){
 const recording=parseRecording(fs.readFileSync(file,'utf8'),file);
 assert(recording.samples.length>0);assert(recording.duration>0);
 assert(recording.samples.some(s=>s.player_state_raw===3));
 console.log(file, {stage:recording.stage,samples:recording.samples.length,duration:recording.duration,crouched:recording.samples.filter(s=>s.player_state_raw===4).length});
}
console.log('PASS: parsing, legacy/new bounds, interpolation, state changes, gaps, malformed/truncated files and victim tracks');
