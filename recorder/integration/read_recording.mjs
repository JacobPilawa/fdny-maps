// node recorder/integration/read_recording.mjs path/to/run.jsonl
import fs from 'node:fs';
import {parseRecording,frameAt,boundsFor} from './recording-data.js';
if(!process.argv[2])throw Error('Supply a JSONL recording path.');
const trace=parseRecording(fs.readFileSync(process.argv[2],'utf8'),process.argv[2]);
console.log({stage:trace.stage,samples:trace.samples.length,duration_ms:trace.duration,victim_tracks:trace.victims.length,warnings:trace.warnings});
const midpoint=frameAt(trace.samples,trace.duration/2);
console.log({midpoint:midpoint.position,collision_box:boundsFor(midpoint)});
