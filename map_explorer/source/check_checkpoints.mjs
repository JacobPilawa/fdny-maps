import assert from 'node:assert/strict';
import fs from 'node:fs';
import {checkpointFile} from '../checkpoints.js';
const project={source:'training',triggers:[{name:'one',label:'Door',center:[0,60,0],size:[120,120,120]},{name:'two',label:'Stairs',center:[200,100,200],size:[80,160,80]}]};
assert.equal(checkpointFile(project),'FDNY_CHECKPOINTS_V1|training|2\n-60|0|-60|60|120|60|Door\n160|20|160|240|180|240|Stairs\n');
assert.equal(checkpointFile({...project,triggers:[{...project.triggers[0],label:undefined,split_label:'Old project'}]}).split('\n')[1].split('|')[6],'Old project');
for(const label of ['', 'x'.repeat(81),'bad(hint)','bad|field','line\nbreak','é'])assert.throws(()=>checkpointFile({...project,triggers:[{...project.triggers[0],label}]}));
for(const source of ['../bad','fdny_edit',''])assert.throws(()=>checkpointFile({...project,source}));
for(const size of [[0,120,120],[11000,120,120],[NaN,120,120]])assert.throws(()=>checkpointFile({...project,triggers:[{...project.triggers[0],size}]}));
assert.throws(()=>checkpointFile({...project,triggers:[]}));assert.throws(()=>checkpointFile({...project,triggers:Array(33).fill(project.triggers[0])}));
assert(checkpointFile({...project,triggers:Array(32).fill({...project.triggers[0],label:'x'.repeat(80)})}).length<8192);
// Exercise actual export metadata for all supported first-stage missions.
const missions=JSON.parse(fs.readFileSync(new URL('../data/editor/missions.json',import.meta.url)));
for(const mission of missions)assert(checkpointFile({source:mission.key,triggers:[{name:'Spawn',center:mission.info.spawn,size:[120,120,120]}]}).startsWith('FDNY_CHECKPOINTS_V1|'+mission.key+'|1\n'));
if(process.argv[2])fs.writeFileSync(process.argv[2],checkpointFile(project));
console.log('PASS: readable file bounds/labels, legacy labels, validation, 32-checkpoint limit and all '+missions.length+' rescue missions');
