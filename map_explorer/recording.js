import * as THREE from 'three';
import {parseRecording,frameAt,indexAt,boundsFor,canJoin,timeColor,trailMetrics,MAX_RECORDING_BYTES} from './recording-data.js';

const clock=ms=>`${Math.floor(ms/60000)}:${(ms/1000%60).toFixed(2).padStart(5,'0')}`;
const RECORDING_COLORS=['#d33f49','#2878c7','#9b51b6','#d47b16','#008875','#bd468d'];
const xyz=p=>p.map(n=>n.toFixed(1)).join(' · ');
// Screen-space ribbon quads keep width adjustable on WebGL implementations
// that clamp native line width to one pixel. Dimensions are in CSS pixels.
function thickTrack(positions,colors,opacity){
  const geometry=new THREE.InstancedBufferGeometry();
  geometry.setAttribute('position',new THREE.Float32BufferAttribute([0,-1,0,0,1,0,1,-1,0,1,1,0],3));
  geometry.setIndex([0,2,1,2,3,1]);
  const endpoints=new THREE.InstancedInterleavedBuffer(new Float32Array(positions),6),tints=new THREE.InstancedInterleavedBuffer(new Float32Array(colors),6);
  geometry.setAttribute('segmentStart',new THREE.InterleavedBufferAttribute(endpoints,3,0));geometry.setAttribute('segmentEnd',new THREE.InterleavedBufferAttribute(endpoints,3,3));
  geometry.setAttribute('colorStart',new THREE.InterleavedBufferAttribute(tints,3,0));geometry.setAttribute('colorEnd',new THREE.InterleavedBufferAttribute(tints,3,3));
  geometry.instanceCount=positions.length/6;
  const material=new THREE.ShaderMaterial({transparent:true,depthWrite:false,side:THREE.DoubleSide,toneMapped:false,
    uniforms:{resolution:{value:new THREE.Vector2(1,1)},width:{value:3},opacity:{value:opacity},minY:{value:-1e9},maxY:{value:1e9}},
    vertexShader:`attribute vec3 segmentStart; attribute vec3 segmentEnd; attribute vec3 colorStart; attribute vec3 colorEnd;
      uniform vec2 resolution; uniform float width; varying vec3 tint; varying float worldY;
      void main(){
        float along=position.x;vec4 a=projectionMatrix*modelViewMatrix*vec4(segmentStart,1.0);vec4 b=projectionMatrix*modelViewMatrix*vec4(segmentEnd,1.0);
        tint=mix(colorStart,colorEnd,along);worldY=mix(segmentStart.y,segmentEnd.y,along);
        if(a.w<=0.001||b.w<=0.001){gl_Position=vec4(2.0,2.0,2.0,1.0);return;}
        vec2 delta=(b.xy/b.w-a.xy/a.w)*resolution;float len=length(delta);vec2 tangent=len>0.001?delta/len:vec2(1.0,0.0);vec2 normal=vec2(-tangent.y,tangent.x);
        vec4 point=mix(a,b,along);point.xy+=(normal*position.y+tangent*(along*2.0-1.0))*width/resolution*point.w;gl_Position=point;
      }`,
    fragmentShader:`uniform float opacity;uniform float minY;uniform float maxY;varying vec3 tint;varying float worldY;
      void main(){if(worldY<minY||worldY>maxY)discard;gl_FragColor=vec4(tint,opacity);
      #include <colorspace_fragment>
      }`
  });
  const mesh=new THREE.Mesh(geometry,material);mesh.frustumCulled=false;return mesh;
}
export class RecordingMode {
  constructor(host){
    this.host=host;this.recordings=[];this.followIndex=0;this.settingsIndex=0;this.trace=null;this.time=0;this.playing=false;this.mode='scrub';this.speed=1;this.group=null;this.generation=0;this.showPlayer=true;this.showVictims=true;this.xray=true;this.playerColor='#ff0000';this.thickness=3;this.follow=true;
    this.panel=document.createElement('section');this.panel.id='recordingPanel';this.panel.hidden=true;this.panel.setAttribute('aria-label','Recording playback');
    this.panel.dataset.expanded='false';
    this.panel.title='Drag the bottom-right corner to resize';
    this.panel.innerHTML=`<div class="recHeading"><span class="recDot"></span><b data-rec="title">Recording</b><span class="spacer"></span><label class="recFollow"><input data-rec="follow" type="checkbox" checked>Follow player</label><select data-rec="followSelect" aria-label="Follow player" hidden></select><button class="recSettingsToggle" data-rec="settingsToggle" aria-expanded="false" aria-controls="recordingSettings" title="Show recording settings">Settings <span aria-hidden="true">⌄</span></button><button class="recClose" data-rec="close" aria-label="Remove recording" title="Remove recording">×</button></div>
      <div class="recTimeline"><div class="recTransport" role="group" aria-label="Playback controls"><button data-rec="back" aria-label="Previous recorded sample" title="Previous sample">‹</button><button class="recPlay" data-rec="play" aria-label="Play recording" title="Play / pause">▶</button><button data-rec="next" aria-label="Next recorded sample" title="Next sample">›</button></div><input data-rec="time" type="range" min="0" max="1" step="1" value="0" aria-label="Recording time"><output data-rec="clock"></output><select data-rec="speed" aria-label="Playback speed" title="Playback speed"><option value=".25">¼×</option><option value=".5">½×</option><option value="1" selected>1×</option><option value="2">2×</option><option value="4">4×</option></select></div>
      <div id="recordingSettings" class="recSettings" data-rec="settings" hidden>
        <label class="recPlayerSelect" data-rec="playerSelectLabel" hidden>Player settings <select data-rec="playerSelect" aria-label="Player settings"></select></label><div class="recSettingsGrid">
          <fieldset class="recSection"><legend>View</legend><div class="recChecks"><label><input data-rec="player" type="checkbox" checked>Player &amp; hitbox</label><label data-rec="victimsLabel"><input data-rec="victims" type="checkbox" checked>Victims</label><label data-rec="xrayLabel"><input data-rec="xray" type="checkbox" checked>Through walls</label></div></fieldset>
          <fieldset class="recSection"><legend>Camera</legend><div class="recButtonGroup"><button data-rec="fit">Fit track</button><button data-rec="focus">Snap to player</button></div></fieldset>
          <fieldset class="recSection recAppearance"><legend>Appearance</legend><label>Trail color<select data-rec="trailColor" aria-label="Trail color"><option value="time">Elapsed time</option><option value="distance">Distance traveled</option><option value="speed">Speed · percentile</option><option value="acceleration">Acceleration · percentile</option><option value="player">Player color</option></select></label><label>Player color<input data-rec="color" type="color" value="#ff0000" aria-label="Player color"></label><label class="recThickness">Track width<input data-rec="thickness" type="range" min="1" max="14" step="1" value="3" aria-label="Track thickness"><output data-rec="thicknessValue">3 px</output></label></fieldset>
        </div><div class="recTrailLegend" data-rec="trailLegend"></div>
        <details class="recDetails"><summary>Recording details <span data-rec="stats"></span></summary><div class="recReadout" data-rec="readout"></div><div class="recNote" data-rec="note"></div></details>
        <div class="recSettingsFooter"><span class="recResizeHint">Drag corner to resize</span></div>
      </div><button class="recMapReturn" data-rec="map" hidden>Return to recorded map</button>`;
    host.view.append(this.panel);
    this.el=Object.fromEntries([...this.panel.querySelectorAll('[data-rec]')].map(e=>[e.dataset.rec,e]));
    this.input=document.createElement('input');this.input.type='file';this.input.multiple=true;this.input.accept='.jsonl,.json,application/json';this.input.hidden=true;host.view.append(this.input);
    const upload=()=>{document.getElementById('toolsMenu').open=false;this.input.value='';this.input.click();};
    document.getElementById('uploadRecording').onclick=upload;
    this.input.onchange=async()=>{const files=[...this.input.files||[]];if(!files.length)return;try{await this.upload(files);}catch(error){host.error(`Recording: ${error.message}`);}};
    this.el.close.onclick=()=>this.clear();
    this.el.settingsToggle.onclick=()=>this.setExpanded(this.el.settings.hidden);
    this.panelResize=new ResizeObserver(()=>host.view.style.setProperty('--recording-panel-height',this.panel.getBoundingClientRect().height+'px'));
    this.panelResize.observe(this.panel);
    this.el.time.oninput=()=>{this.playing=false;this.seek(Number(this.el.time.value));};
    this.el.play.onclick=()=>{if(!this.trace||!this.matches())return;this.mode='scrub';if(this.time>=this.duration)this.time=0;this.playing=!this.playing;this.last=performance.now();this.sync();host.redraw();};
    this.el.speed.onchange=()=>{this.speed=Number(this.el.speed.value);};
    this.el.back.onclick=()=>this.stepSample(-1);this.el.next.onclick=()=>this.stepSample(1);
    for(const name of ['player','victims','xray'])this.el[name].onchange=()=>{const r=this.settingsRecording();if(r)r[{player:'showPlayer',victims:'showVictims',xray:'xray'}[name]]=this.el[name].checked;host.redraw();};
    this.el.fit.onclick=()=>this.fit();this.el.focus.onclick=()=>this.snap();
    this.el.playerSelect.onchange=()=>{this.settingsIndex=Number(this.el.playerSelect.value);this.syncPlayerSettings();this.sync();};
    this.el.followSelect.onchange=()=>{this.follow=this.el.followSelect.value!=='';if(this.follow){this.followIndex=Number(this.el.followSelect.value);this.snap();}};
    this.el.follow.onchange=()=>{this.follow=this.el.follow.checked;if(this.follow)this.snap();};
    this.el.trailColor.onchange=()=>{const r=this.settingsRecording();if(r){r.trailColorMode=this.el.trailColor.value;r.dispose();this.syncTrailLegend();host.redraw();}};
    this.el.color.oninput=()=>{const r=this.settingsRecording();if(r){r.playerColor=this.el.color.value;r.dispose();}host.redraw();};
    this.el.thickness.oninput=()=>{const r=this.settingsRecording();if(r)r.thickness=Number(this.el.thickness.value);this.el.thicknessValue.textContent=this.el.thickness.value+' px';host.redraw();};
    this.el.map.onclick=async()=>{try{await host.loadStage(this.trace.stage);this.sync();host.redraw();}catch(e){host.error(e.message);}};
    document.addEventListener('visibilitychange',()=>{if(document.hidden){this.playing=false;this.sync();}});
  }
  setExpanded(expanded){
    if(!expanded){this.expandedHeight=this.panel.style.height;this.panel.style.height='';}
    else this.panel.style.height=this.expandedHeight||'';
    this.el.settings.hidden=!expanded;this.panel.dataset.expanded=String(expanded);
    this.el.settingsToggle.setAttribute('aria-expanded',String(expanded));
    this.el.settingsToggle.title=expanded?'Collapse settings; keep playback visible':'Show recording settings';
  }
  matches(){return this.trace?.stage===this.host.state().data?.key;}
  get duration(){return Math.max(0,...this.recordings.map(r=>r.trace.duration));}
  settingsRecording(){return this.recordings[this.settingsIndex];}
  followRecording(){return this.recordings[this.followIndex]||this.recordings[0];}
  syncPlayerSettings(){
    const r=this.settingsRecording();if(!r)return;
    this.el.trailColor.value=r.effectiveTrailMode();this.syncTrailLegend();this.el.player.checked=r.showPlayer;this.el.victims.checked=r.showVictims;this.el.xray.checked=r.xray;
    this.el.color.value=r.playerColor;this.el.thickness.value=r.thickness;this.el.thicknessValue.textContent=r.thickness+' px';
  }
  updatePlayerSelectors(){
    const multi=this.recordings.length>1;
    this.el.follow.closest('label').hidden=multi;this.el.followSelect.hidden=!multi;this.el.playerSelectLabel.hidden=!multi;
    this.el.followSelect.replaceChildren();this.el.playerSelect.replaceChildren();
    this.el.followSelect.add(new Option('Follow: None',''));
    this.recordings.forEach((r,i)=>{
      const label=`Player ${i+1} · ${r.trace.filename}`;
      this.el.followSelect.add(new Option('Follow: '+label,String(i)));
      this.el.playerSelect.add(new Option(label,String(i)));
    });
    this.el.followSelect.value=this.follow?String(this.followIndex):'';this.el.playerSelect.value=String(this.settingsIndex);
    this.syncPlayerSettings();
  }
  async upload(files){
    const generation=++this.generation;
    const parsed=await Promise.all(files.map(async file=>{
      if(file.size>MAX_RECORDING_BYTES)throw Error('Choose recordings smaller than 128 MB each.');
      const trace=parseRecording(await file.text(),file.name);
      if(!this.host.maps().some(m=>m.key===trace.stage))throw Error(`Map “${trace.stage}” is not available in this viewer.`);
      return trace;
    }));
    if(generation!==this.generation)return;
    const stage=this.trace?.stage||parsed[0].stage;
    if(parsed.some(tr=>tr.stage!==stage))throw Error('Load recordings from the same map together. Remove the current recordings to switch maps.');
    if(this.host.state().data?.key!==stage)await this.host.loadStage(stage);
    if(generation!==this.generation)return;
    if(this.host.state().data?.key!==stage)throw Error('The map changed while loading; please upload again.');
    for(const trace of parsed){
      const r=Object.create(RecordingMode.prototype);
      Object.assign(r,{host:this.host,trace,time:0,mode:'scrub',group:null,showPlayer:true,showVictims:true,xray:true,playerColor:RECORDING_COLORS[this.recordings.length%RECORDING_COLORS.length],thickness:3});
      this.recordings.push(r);
    }
    for(const r of this.recordings){r.multi=this.recordings.length>1;r.dispose();}
    this.trace=this.recordings[0].trace;this.time=0;this.playing=false;this.follow=true;this.el.follow.checked=true;
    this.updatePlayerSelectors();this.panel.hidden=false;this.host.error(null);this.sync();this.snap();this.host.redraw();
  }
  clear(){++this.generation;this.playing=false;this.dispose();this.recordings=[];this.trace=null;this.followIndex=0;this.settingsIndex=0;this.panel.hidden=true;this.host.redraw();}
  dispose(){
    if(this.recordings){for(const r of this.recordings)r.dispose();return;}
    if(!this.group)return;
    this.group.traverse(o=>{o.geometry?.dispose();for(const m of o.material?(Array.isArray(o.material)?o.material:[o.material]):[])m.dispose();});
    this.group.removeFromParent();this.group=null;this.lines=[];this.proxies=[];
  }
  seek(t){if(!this.trace)return;this.time=Math.max(0,Math.min(this.duration,t));this.sync();this.followNow();this.host.redraw();}
  stepSample(direction){
    if(!this.trace)return;this.playing=false;
    let next=direction>0?this.duration:0;
    for(const r of this.recordings){const samples=r.trace.samples;let i=indexAt(samples,this.time);
      if(direction>0){i++;if(i<samples.length)next=Math.min(next,samples[i].t_ms);}
      else{if(samples[i].t_ms>=this.time)i--;if(i>=0)next=Math.max(next,samples[i].t_ms);}
    }
    this.seek(next);
  }
  tick(){
    if(!this.trace)return;
    const viewKey=this.host.state().mode+':'+this.host.state().data?.key;
    if(this.lastView!==viewKey){this.lastView=viewKey;this.sync();}
    if(!this.matches()){this.playing=false;this.sync();return;}
    if(!this.playing)return;
    const now=performance.now();this.time=Math.min(this.duration,this.time+Math.min(now-this.last,250)*this.speed);this.last=now;
    if(this.time>=this.duration)this.playing=false;
    this.sync();this.followNow();if(this.playing)this.host.redraw();
  }
  sync(){
    if(!this.trace)return;
    const tr=this.settingsRecording().trace,s=frameAt(tr.samples,Math.min(this.time,tr.duration)),b=boundsFor(s),matching=this.matches();
    this.el.title.textContent=`${this.host.maps().find(m=>m.key===tr.stage)?.name||tr.stage} · ${this.recordings.length===1?'Recording':this.recordings.length+' recordings'}`;
    this.el.title.title=tr.filename+'\n'+String(tr.header.started_utc||'');
    this.el.time.max=this.duration;this.el.time.value=this.time;
    this.el.clock.textContent=`${clock(this.time)} / ${clock(this.duration)}`;this.el.play.textContent=this.playing?'Ⅱ':'▶';this.el.play.setAttribute('aria-label',this.playing?'Pause recording':'Play recording');
    this.el.stats.textContent=`${tr.samples.length.toLocaleString()} samples · ${(tr.distance/1000).toFixed(1)}k units`;
    this.el.victimsLabel.hidden=!tr.victims.length;this.el.xrayLabel.hidden=this.host.state().mode!=='3d';this.el.map.hidden=matching;this.el.settingsToggle.classList.toggle('hasNotice',!matching||tr.warnings.length>0);
    for(const id of ['play','time','back','next','focus','fit'])this.el[id].disabled=!matching;
    const state=s.player_state_raw===4?'Crouched':s.player_state_raw===5?'Climbing':s.ground_contact_raw?'Standing / grounded':'Airborne';
    this.el.readout.textContent=`${state} · box ${b.max.map((n,i)=>Math.round(n-b.min[i])).join(' × ')} u${b.measured?'':' (inferred)'} · XYZ ${xyz(s.position)}`;
    const notes=[];
    if(!matching)notes.push('Recording hidden on this map.');
    if(!tr.victims.length)notes.push('No victim positions in this file; use recorder 1.1 for future runs.');
    else notes.push(`${tr.victims.length} victim track${tr.victims.length===1?'':'s'} · last observed positions`);
    if(tr.warnings.length)notes.push(tr.warnings.join(' '));
    this.el.note.textContent=notes.join(' ');
  }
  fit(){
    if(!this.trace||!this.matches())return;
    this.follow=false;this.el.follow.checked=false;this.el.followSelect.value='';
    const min=[Infinity,Infinity,Infinity],max=[-Infinity,-Infinity,-Infinity];
    for(const r of this.recordings)for(const s of r.trace.samples)for(let i=0;i<3;i++){min[i]=Math.min(min[i],s.position[i]-100);max[i]=Math.max(max[i],s.position[i]+100);}
    this.host.focusBounds({min,max});
  }
  followNow(){if(this.follow&&this.trace&&this.matches())this.focus();}
  snap(){if(!this.trace||!this.matches())return;const p=frameAt(this.followRecording().trace.samples,this.time).position;this.host.focusBounds({min:[p[0]-550,p[1]-150,p[2]-550],max:[p[0]+550,p[1]+250,p[2]+550]});}
  focus(){if(!this.trace||!this.matches())return;const p=frameAt(this.followRecording().trace.samples,this.time).position;this.host.focusPoint(p);}
  victimFrame(track){if(this.time<track.samples[0].t_ms||track.end!==null&&this.time>=track.end)return null;return frameAt(track.samples,this.time);}
  effectiveTrailMode(){return !this.trailColorMode||this.trailColorMode==='auto'?(this.multi?'player':'time'):this.trailColorMode;}
  trailFraction(i){
    const mode=this.effectiveTrailMode();if(mode==='time')return this.trace.samples[i].t_ms/(this.trace.duration||1);
    const m=this.metrics||(this.metrics=trailMetrics(this.trace.samples));
    if(mode==='distance')return m.distance[i]/(m.distance.at(-1)||1);
    return mode==='speed'?m.speedRank[i]:m.accelerationRank[i];
  }
  trailTint(i){if(this.effectiveTrailMode()==='player')return this.playerColor;const f=this.trailFraction(i);return f===null?'#9aa39b':timeColor(f);}
  syncTrailLegend(){
    const r=this.settingsRecording();if(!r)return;const mode=r.effectiveTrailMode(),el=this.el.trailLegend;el.replaceChildren();
    if(mode==='player'){el.textContent='Track matches player color.';return;}
    const ramp=document.createElement('i');el.append(ramp);
    if(mode==='speed'||mode==='acceleration'){
      const metrics=r.metrics||(r.metrics=trailMetrics(r.trace.samples)),values=metrics[mode],ranks=metrics[mode+'Rank'];
      const quantile=q=>{let best=null;for(let i=0;i<values.length;i++)if(values[i]!==null&&(best===null||Math.abs(ranks[i]-q)<Math.abs(ranks[best]-q)))best=i;return best===null?'—':Math.round(values[best]).toLocaleString();};
      el.append(`Slow / low → fast / high · P50 ${quantile(.5)} · P90 ${quantile(.9)} · P99 ${quantile(.99)} ${mode==='speed'?'u/s':'u/s²'}. Percentiles within this recording; gray = unavailable. Acceleration is magnitude, including turns.`);
    }else el.append(mode==='time'?'Start → end · elapsed time':'Start → end · cumulative distance');
  }
  draw2D(c){for(const r of this.recordings){r.time=Math.min(this.time,r.trace.duration);r.drawSingle2D(c);}}
  drawSingle2D(c){
    if(!this.trace||!this.matches())return;
    const h=this.host.state(),tr=this.trace,screen=p=>[p[0]*h.scale+h.offsetX,p[2]*h.scale+h.offsetY];
    c.save();c.lineCap='round';c.lineJoin='round';
    const path=(samples,victim=false)=>{
      c.lineWidth=victim?Math.max(1,this.thickness*.7):this.thickness;c.globalAlpha=victim ? .7 : .9;
      let bucket=null;
      for(let i=1;i<samples.length;i++){
        const a=samples[i-1],b=samples[i];if(this.mode==='scrub'&&a.t_ms>this.time)break;
        if(!canJoin(a,b)||(a.position[1]<h.low&&b.position[1]<h.low)||(a.position[1]>h.high&&b.position[1]>h.high))continue;
        const end=this.mode==='scrub'&&b.t_ms>this.time?frameAt(samples,this.time):b;
        const nextBucket=victim?'#008574':this.trailTint(i);
        if(bucket!==nextBucket){if(bucket!==null)c.stroke();bucket=nextBucket;c.strokeStyle=nextBucket;c.beginPath();}
        c.moveTo(...screen(a.position));c.lineTo(...screen(end.position));
      }
      if(bucket!==null)c.stroke();
      c.globalAlpha=1;
    };
    path(tr.samples);if(this.showVictims)for(const track of tr.victims)path(track.samples,true);
    const marker=(s,color,label,victim=false)=>{
      const b=boundsFor(s,victim),p=s.position;if(p[1]+b.max[1]<h.low||p[1]+b.min[1]>h.high)return;
      const [x,y]=screen(p),w=(b.max[0]-b.min[0])*h.scale,d=(b.max[2]-b.min[2])*h.scale;
      c.fillStyle=color+'25';c.strokeStyle=color;c.lineWidth=1.8;c.fillRect(x+b.min[0]*h.scale,y+b.min[2]*h.scale,w,d);c.strokeRect(x+b.min[0]*h.scale,y+b.min[2]*h.scale,w,d);
      c.beginPath();c.arc(x,y,4,0,2*Math.PI);c.fillStyle=color;c.fill();
      if(s.body_forward){const f=s.body_forward,len=Math.max(22,100*h.scale),angle=Math.atan2(f[2],f[0]);c.beginPath();c.moveTo(x,y);c.lineTo(x+Math.cos(angle)*len,y+Math.sin(angle)*len);c.stroke();}
      c.font='11px ui-monospace,monospace';const text=label,tw=c.measureText(text).width;
      c.fillStyle='#fffffff0';c.fillRect(x+9,y-22,tw+10,18);c.fillStyle=color;c.fillText(text,x+14,y-9);
    };
    if(this.showVictims)for(const track of tr.victims){const s=this.victimFrame(track);if(s)marker(s,'#008574',track.name,true);}
    if(this.showPlayer){const s=frameAt(tr.samples,this.time);marker(s,this.playerColor,s.player_state_raw===4?'Player · crouched':'Player');}
    c.restore();
  }
  makeProxy(color){
    const group=new THREE.Group(),geo=new THREE.BoxGeometry(1,1,1);
    const box=new THREE.Mesh(geo,new THREE.MeshBasicMaterial({color,transparent:true,opacity:.12,depthWrite:false}));
    const edges=new THREE.LineSegments(new THREE.EdgesGeometry(geo),new THREE.LineBasicMaterial({color,transparent:true,opacity:.95}));box.add(edges);group.add(box);
    const pill=new THREE.Mesh(new THREE.CapsuleGeometry(.22,.56,4,8),new THREE.MeshBasicMaterial({color,transparent:true,opacity:.55,depthWrite:false}));group.add(pill);
    const arrow=new THREE.ArrowHelper(new THREE.Vector3(0,0,1),new THREE.Vector3(),100,color,20,10);group.add(arrow);
    group.traverse(o=>o.renderOrder=12);return {group,box,pill,arrow};
  }
  build3D(scene){
    this.dispose();this.group=new THREE.Group();this.group.name='Gameplay recording';this.lines=[];this.proxies=[];
    const addLine=(samples,victim=false)=>{
      const positions=[],colors=[],endTimes=[],col=new THREE.Color();
      for(let i=1;i<samples.length;i++)if(canJoin(samples[i-1],samples[i])){
        for(const s of [samples[i-1],samples[i]]){positions.push(...s.position);if(victim)col.set('#008574');else if(this.effectiveTrailMode()==='player')col.set(this.playerColor);else{const f=this.trailFraction(s===samples[i-1]?i-1:i);if(f===null)col.set('#9aa39b');else col.setHSL(260/360*(1-Math.max(0,Math.min(1,f))),.85,.48);}colors.push(col.r,col.g,col.b);}
        endTimes.push(samples[i].t_ms);
      }
      const line=thickTrack(positions,colors,victim ? .65 : .95);line.renderOrder=10;
      this.group.add(line);this.lines.push({line,endTimes,victim});
    };
    addLine(this.trace.samples);
    const player=this.makeProxy(this.playerColor);this.group.add(player.group);this.proxies.push({...player,track:null});
    for(const track of this.trace.victims){addLine(track.samples,true);const proxy=this.makeProxy('#008574');this.group.add(proxy.group);this.proxies.push({...proxy,track});}
    scene.add(this.group);
  }
  update3D(){for(const r of this.recordings){r.time=Math.min(this.time,r.trace.duration);r.updateSingle3D();}}
  updateSingle3D(){
    const h=this.host.state();
    if(!this.trace||!h.scene)return;
    if(!this.group||this.group.parent!==h.scene)this.build3D(h.scene);
    this.group.visible=this.matches();if(!this.group.visible)return;
    for(const {line,endTimes,victim} of this.lines){
      line.visible=!victim||this.showVictims;
      let count=endTimes.length;if(this.mode==='scrub'){let lo=0,hi=count;while(lo<hi){const m=(lo+hi)>>>1;if(endTimes[m]<=this.time)lo=m+1;else hi=m;}count=lo;}
      line.geometry.instanceCount=count;
      line.material.uniforms.width.value=victim?Math.max(1,this.thickness*.7):this.thickness;
      line.material.uniforms.resolution.value.set(h.width||1,h.height||1);
      line.material.uniforms.minY.value=Number.isFinite(h.low)?h.low:-1e9;line.material.uniforms.maxY.value=Number.isFinite(h.high)?h.high:1e9;
    }
    for(const proxy of this.proxies){
      const s=proxy.track?this.victimFrame(proxy.track):frameAt(this.trace.samples,this.time),show=proxy.track?this.showVictims:this.showPlayer;
      proxy.group.visible=!!s&&show;if(!s||!show)continue;
      if(!proxy.track)proxy.group.traverse(o=>{if(o.material?.color)o.material.color.set(this.playerColor);});
      const b=boundsFor(s,!!proxy.track),size=b.max.map((n,i)=>n-b.min[i]),center=b.min.map((n,i)=>(n+b.max[i])/2);
      proxy.group.position.set(...s.position);proxy.box.position.set(...center);proxy.box.scale.set(...size);
      proxy.pill.position.set(...center);proxy.pill.scale.set(size[0],size[1],size[2]);
      proxy.arrow.position.set(0,center[1],0);const f=s.body_forward||[0,0,1],pitch=(s.pitch_deg||0)*Math.PI/180;
      proxy.arrow.setDirection(new THREE.Vector3(f[0]*Math.cos(pitch),-Math.sin(pitch),f[2]*Math.cos(pitch)).normalize());proxy.arrow.visible=!proxy.track;
    }
    this.group.traverse(o=>{for(const m of o.material?(Array.isArray(o.material)?o.material:[o.material]):[]){m.depthTest=!this.xray;m.depthWrite=false;m.clippingPlanes=h.heightClip;}});
  }
}
