// Recording parsing and timeline math are independent of the renderer.
export const MAX_RECORDING_BYTES = 128 * 1024 * 1024;
const vector = v => Array.isArray(v) && v.length === 3 && v.every(n => Number.isFinite(n) && Math.abs(n) < 1e8);
const time = t => Number.isFinite(t) && t >= 0 && t <= 31 * 86400000;
export function boundsFor(sample, victim = false) {
  if (vector(sample.bounds_min) && vector(sample.bounds_max) && sample.bounds_min.every((n,i) => n < sample.bounds_max[i]))
    return {min: sample.bounds_min, max: sample.bounds_max, measured: true};
  return victim ? {min:[-24,-24,-24],max:[24,24,24],measured:false} :
    {min:[-30,-30,-30],max:[30,sample.player_state_raw === 4 ? 25 : 85,30],measured:false};
}
export function canJoin(a,b) {
  return b.t_ms - a.t_ms <= 500 && Math.hypot(...a.position.map((v,i)=>v-b.position[i])) <= 2000;
}
export function indexAt(samples,t) {
  let lo=0,hi=samples.length;
  while(lo<hi){const m=(lo+hi)>>>1;if(samples[m].t_ms<=t)lo=m+1;else hi=m;}
  return Math.max(0,lo-1);
}
export function frameAt(samples,t) {
  if(!samples.length)return null;
  const i=indexAt(samples,t),a=samples[i],b=samples[i+1];
  if(!b||t<=a.t_ms||b.t_ms===a.t_ms||!canJoin(a,b))return a;
  const f=(t-a.t_ms)/(b.t_ms-a.t_ms),lerp=(u,v)=>u.map((n,j)=>n+(v[j]-n)*f);
  const result={...a,position:lerp(a.position,b.position)};
  if(vector(a.body_forward)&&vector(b.body_forward)){
    const yaw=Math.atan2(a.body_forward[0],a.body_forward[2]);
    let delta=Math.atan2(b.body_forward[0],b.body_forward[2])-yaw;
    delta=Math.atan2(Math.sin(delta),Math.cos(delta));
    result.body_forward=[Math.sin(yaw+delta*f),0,Math.cos(yaw+delta*f)];
  }
  if(Number.isFinite(a.pitch_deg)&&Number.isFinite(b.pitch_deg))result.pitch_deg=a.pitch_deg+(b.pitch_deg-a.pitch_deg)*f;
  return result;
}
export function timeColor(f){return `hsl(${260*(1-Math.max(0,Math.min(1,f)))} 85% 48%)`;}
export function parseRecording(text, filename='Recording') {
  if(typeof text!=='string'||text.length>MAX_RECORDING_BYTES)throw Error('Recording is too large (128 MB maximum).');
  const lines=text.replace(/^\uFEFF/,'').split('\n'),samples=[],victims=new Map(),warnings=[];
  let header=null,end=null,records=0;
  const checkSample=(r,line)=>{
    if(!time(r.t_ms)||!vector(r.position))throw Error(`Invalid time or position at line ${line}.`);
    if((r.bounds_min!==undefined||r.bounds_max!==undefined)&&(!vector(r.bounds_min)||!vector(r.bounds_max)||!r.bounds_min.every((n,i)=>n<r.bounds_max[i])))throw Error(`Invalid collision bounds at line ${line}.`);
  };
  for(let i=0;i<lines.length;i++){
    const raw=lines[i].trim();if(!raw)continue;
    if(++records>600000)throw Error('Recording has too many records (600,000 maximum).');
    let r;
    try{r=JSON.parse(raw);}catch{
      if(i===lines.length-1&&!text.endsWith('\n')){warnings.push('Ignored an unfinished final line.');break;}
      throw Error(`Invalid JSON at line ${i+1}.`);
    }
    if(!r||typeof r!=='object'||Array.isArray(r))throw Error(`Invalid record at line ${i+1}.`);
    if(r.type==='start'){
      if(header||samples.length)throw Error('Choose a file containing one run.');
      if(r.schema!=='fdny-trajectory-v1'||typeof r.stage!=='string')throw Error('Unsupported recording format.');
      header=r;
    }else if(r.type==='sample'){
      if(!header||end)throw Error(`Unexpected sample at line ${i+1}.`);
      checkSample(r,i+1);
      if(r.n!==samples.length||samples.length&&r.t_ms<samples.at(-1).t_ms)throw Error(`Missing or out-of-order sample at line ${i+1}.`);
      samples.push(r);
    }else if(r.type==='victim_sample'){
      if(!header||end||typeof r.id!=='string'||r.id.length>128)throw Error(`Invalid victim record at line ${i+1}.`);
      checkSample(r,i+1);
      let track=victims.get(r.id);
      if(!track){track={id:r.id,name:String(r.name||r.id).slice(0,128),samples:[],end:null};victims.set(r.id,track);}
      if(track.samples.length&&r.t_ms<track.samples.at(-1).t_ms)throw Error(`Out-of-order victim time at line ${i+1}.`);
      track.samples.push(r);
    }else if(r.type==='victim_end'){
      if(!time(r.t_ms))throw Error(`Invalid victim end at line ${i+1}.`);
      const track=victims.get(r.id);if(track)track.end=r.t_ms;
    }else if(r.type==='end'){
      if(end||!header)throw Error('Unexpected end record.');end=r;
    }
  }
  if(!header||!samples.length)throw Error('No player movement samples found in this file.');
  if(!end)warnings.push('This file has no closing record; showing the saved samples.');
  if(end&&end.samples!==samples.length)warnings.push('The closing sample count differs; showing the saved samples.');
  const stage=header.stage.replace(/\\/g,'/').split('/').at(-1).replace(/\.bsp$/i,'').toLowerCase();
  const duration=samples.at(-1).t_ms;
  let distance=0,gaps=0;
  for(let i=1;i<samples.length;i++){if(canJoin(samples[i-1],samples[i]))distance+=Math.hypot(...samples[i].position.map((n,j)=>n-samples[i-1].position[j]));else gaps++;}
  if(gaps)warnings.push(`${gaps} pause/position gap${gaps===1?'':'s'} shown as breaks in the track.`);
  return {header,filename,stage,samples,victims:[...victims.values()],duration,distance,warnings};
}

// Derivatives use recorded positions in units/second, never bridge pauses/teleports.
// Duration-weighted percentiles keep irregular sampling from biasing the colors.
export function trailMetrics(samples){
  const distance=new Float64Array(samples.length),speed=new Array(samples.length).fill(null),acceleration=new Array(samples.length).fill(null),velocities=new Array(samples.length).fill(null),weights=new Float64Array(samples.length);
  for(let i=1;i<samples.length;i++){
    distance[i]=distance[i-1];const a=samples[i-1],b=samples[i],dt=(b.t_ms-a.t_ms)/1000;
    if(dt<=0||!canJoin(a,b))continue;
    const v=b.position.map((n,j)=>(n-a.position[j])/dt);velocities[i]=v;weights[i]=dt;speed[i]=Math.hypot(...v);distance[i]+=speed[i]*dt;
    if(velocities[i-1]){const midDt=(b.t_ms-samples[i-2].t_ms)/2000;acceleration[i]=Math.hypot(...v.map((n,j)=>(n-velocities[i-1][j])/midDt));}
  }
  const percentile=values=>{
    const entries=values.flatMap((value,i)=>value===null?[]:[{value,i,weight:weights[i]}]).sort((a,b)=>a.value-b.value),result=new Array(values.length).fill(null);
    const total=entries.reduce((n,e)=>n+e.weight,0);let before=0;
    for(let start=0;start<entries.length;){let end=start,weight=0;while(end<entries.length&&entries[end].value===entries[start].value)weight+=entries[end++].weight;
      const rank=total===weight?.5:before/(total-weight);for(let j=start;j<end;j++)result[entries[j].i]=rank;before+=weight;start=end;
    }return result;
  };
  return {distance,speed,acceleration,speedRank:percentile(speed),accelerationRank:percentile(acceleration)};
}
