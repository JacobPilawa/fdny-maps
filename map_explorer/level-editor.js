import * as THREE from 'three';
const escape=s=>String(s??'').replace(/[&<>"']/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
export class LevelEditor {
  constructor(host){
    this.host=host;this.selected=-1;this.active=false;this.placing=false;this.group=null;this.tool='move';this.snap=16;this.gridY=0;
    this.panel=document.createElement('section');this.panel.id='levelEditor';this.panel.hidden=true;this.panel.setAttribute('aria-label','Custom Triggers');host.view.parentElement.append(this.panel);
    this.panel.addEventListener('click',e=>this.click(e).catch(err=>this.status(err.message,true)));
    this.panel.addEventListener('change',e=>this.change(e).catch(err=>this.status(err.message,true)));
    document.getElementById('levelEdit').onclick=()=>{document.getElementById('toolsMenu').open=false;if(this.active)this.close();else this.open().catch(err=>this.status(err.message,true));};
    window.addEventListener('beforeunload',e=>{if(this.project?.triggers.length){e.preventDefault();e.returnValue='';}});
    window.addEventListener('keydown',e=>{if(!this.active||/input|select|textarea/i.test(e.target.tagName))return;if(e.key==='Escape'&&(this.drag||this.placing||this.pickGrid)){e.preventDefault();e.stopImmediatePropagation();if(this.drag)this.endDrag(true);else{this.finishPlacement(true);this.placing=false;this.pickGrid=false;this.render();}}},true);
    window.addEventListener('blur',()=>{this.endDrag(true);this.finishPlacement(true);});
    if(['localhost','127.0.0.1','[::1]'].includes(location.hostname))this.init();
  }
  async api(path,options){const r=await fetch('/api/level-edit/'+path,options);if(!r.ok){let error;try{error=(await r.json()).error;}catch{}throw Error(error||'Local editor server unavailable. Restart the viewer using start.sh.');}return r;}
  async init(){try{this.missions=(await (await this.api('missions')).json()).missions;document.getElementById('levelEdit').hidden=false;}catch{ /* Static hosting has no retail editor backend. */ }}
  async open(){this.active=true;this.panel.hidden=false;this.host.view.parentElement.classList.add('editor-mode');document.getElementById('levelEdit').setAttribute('aria-pressed','true');if(!this.project){this.render();await this.load(this.host.state().data?.key&&this.missions?.some(m=>m.key===this.host.state().data.key)?this.host.state().data.key:'training');}else{await this.host.loadStage(this.project.source);this.host.setMode('3d');this.render();}this.host.redraw();}
  close(){this.endDrag(true);this.finishPlacement(true);this.active=false;this.placing=false;this.pickGrid=false;this.panel.hidden=true;this.host.view.parentElement.classList.remove('editor-mode');document.getElementById('levelEdit').setAttribute('aria-pressed','false');this.host.redraw();}
  status(message,error=false){const el=this.panel.querySelector('[data-status]');if(el){el.textContent=message;el.classList.toggle('error',error);}}
  splitSummary(){
    const p=this.project;if(!p)return '';
    const checkpoints=p.triggers.map((trigger,index)=>({trigger,index})).filter(x=>x.trigger.checkpoint),stages=this.info?.stage_finishes||[p.source];
    return `<div class="le-section le-summary"><h3>Split order <span>${checkpoints.length+stages.length}</span></h3><ol aria-label="Split order">${checkpoints.map(({trigger:t,index})=>`<li><button data-action="select" data-index="${index}" aria-pressed="${index===this.selected}">${escape(t.split_label||t.name)}</button><small>Custom</small></li>`).join('')}${stages.map(stage=>`<li><span>${escape(stage)} complete</span><small>Default</small></li>`).join('')}</ol>${!checkpoints.length?'<p class="le-help">Add a trigger to insert a subsplit before the stage finish.</p>':''}</div>`;
  }
  render(){
    const p=this.project,t=p?.triggers[this.selected],checkpointIndex=p?.triggers.filter(x=>x.checkpoint).indexOf(t),checkpointCount=p?.triggers.filter(x=>x.checkpoint).length;
    this.panel.innerHTML=`<div class="le-head"><strong>Custom Triggers</strong><button data-action="close">Exit</button></div><div class="le-body">
    <div class="le-section"><h3>Mission</h3><div class="le-row"><select data-field="mission" aria-label="Rescue mission"><option value="">Choose a mission…</option>${(this.missions||[]).map(m=>`<option value="${escape(m.key)}" ${p?.source===m.key?'selected':''}>${escape(m.title)}</option>`).join('')}</select><button data-action="load">Load</button></div></div>
    ${p?`${this.splitSummary()}
    <div class="le-section"><div class="le-row"><button data-action="add">＋ Add trigger</button><button data-action="existing">Use existing…</button></div>
    ${this.showExisting?`<div class="le-row"><select data-field="existing" aria-label="Existing trigger"><option value="">Choose a volume…</option>${this.info.triggers.map(x=>`<option value="${x.entity}">${escape(x.name)}</option>`).join('')}</select><button data-action="useExisting">Use</button></div>`:''}
    ${p.triggers.some(x=>!x.checkpoint)?`<div class="le-trigger-list" role="group" aria-label="Hint-only triggers"><h4>Hints · excluded from splits</h4>${p.triggers.map((x,i)=>!x.checkpoint?`<button data-action="select" data-index="${i}" aria-pressed="${i===this.selected}">${escape(x.name)}</button>`:'').join('')}</div>`:''}</div>
    ${t?`<div class="le-section"><h3>Selected trigger</h3><div class="le-row le-tools">${t.existing_entity==null?`<button data-action="place" aria-pressed="${this.placing}">${this.placing?'Cancel':'Place'}</button><button data-action="move" aria-pressed="${!this.placing&&this.tool==='move'}">Move</button><button data-action="resize" aria-pressed="${!this.placing&&this.tool==='resize'}">Resize</button>`:'<small>Existing volume · fixed bounds</small>'}<button data-action="focus">Focus</button><button data-action="delete" class="le-remove">Remove</button></div>
    <label>Name <input data-field="name" value="${escape(t.name)}" maxlength="40" ${t.existing_entity!=null?'readonly':''}></label><label><input type="checkbox" data-field="checkpoint" ${t.checkpoint?'checked':''}> Include in splits</label>
    ${t.checkpoint?`<label>Label <input data-field="split_label" maxlength="80" value="${escape(t.split_label||t.name)}"></label><div class="le-row"><small>Subsplit ${checkpointIndex+1} of ${checkpointCount}</small><button data-action="earlier" ${checkpointIndex===0?'disabled':''}>↑ Earlier</button><button data-action="later" ${checkpointIndex===checkpointCount-1?'disabled':''}>↓ Later</button></div>`:''}
    <div class="le-coordinates">${['center','size'].map(kind=>`<span>${kind==='center'?'Position':'Size'}</span>${['X','Y','Z'].map((axis,a)=>`<label>${axis}<input type="number" step="1" ${kind==='size'?'min="8" max="10000"':''} data-vector="${kind}" data-axis="${a}" value="${t[kind][a]}" ${t.existing_entity!=null?'readonly':''}></label>`).join('')}`).join('')}</div>
    <label>Hint <textarea data-field="message" rows="1" maxlength="240">${escape(t.message)}</textarea></label><label>Seconds <input data-field="seconds" type="number" min="1" max="30" step="1" value="${t.seconds}"></label></div>`:''}
    <div class="le-section le-grid"><h3>Grid & view</h3><div class="le-row"><button data-action="gridDown" aria-label="Lower grid">−</button><label>Height <input data-field="gridY" type="number" step="${this.snap}" value="${this.gridY}"></label><button data-action="gridUp" aria-label="Raise grid">＋</button></div><div class="le-row"><button data-action="pickGrid">${this.pickGrid?'Click a floor…':'Pick floor'}</button><button data-action="gridBase" ${t?'':'disabled'}>Box base</button><label>Snap <select data-field="snap">${[1,8,16,32].map(n=>`<option ${this.snap===n?'selected':''}>${n}</option>`).join('')}</select></label></div><div class="le-row"><button data-action="top">Top</button><button data-action="perspective">Perspective</button></div></div>
    <div class="le-section le-export"><h3>Project & export</h3><div class="le-row"><button data-action="save">Save project</button><button data-action="import">Open project</button><button data-action="export" ${p.triggers.length?'':'disabled'}>Export level + splits</button></div><small>${this.info?.custom_autosplitter?'Includes custom ASLs; gameplay validation pending.':'ASL generation needs fdny-tas/livesplit/custom_triggers.py.'}</small></div>`:'<button data-action="import">Open project</button>'}
    </div><p data-status role="status">${this.placing?'Click for a cube; drag a footprint. Esc cancels.':this.pickGrid?'Click a visible floor to set grid height.':t?'Drag X / Y / Z handles · Shift: fine · Esc: cancel.':''}</p>`;
    this.dirty=true;this.host.redraw();
  }
  async load(key){if(!key)throw Error('Choose a rescue mission.');const info=await(await this.api('mission?key='+encodeURIComponent(key))).json();await this.host.loadStage(key);this.host.setMode('3d');this.info=info;this.project={schema:'fdny-level-edit-v1',source:key,source_sha256:info.source_sha256,triggers:[]};this.selected=-1;this.placing=false;this.pickGrid=false;this.showExisting=false;this.gridY=info.spawn[1]-30;this.render();this.viewPreset(false);}
  async click(e){const action=e.target.closest('[data-action]')?.dataset.action;if(!action)return;
    const t=this.project?.triggers[this.selected];
    if(action==='close'){this.close();return;}
    if(action==='select'){this.pickGrid=false;this.selected=Number(e.target.closest('[data-index]').dataset.index);this.placing=false;this.render();return;}
    if(action==='top'||action==='perspective'){this.viewPreset(action==='top');return;}
    if(action==='gridDown'||action==='gridUp'){this.gridY=Math.max(-100000,Math.min(100000,this.gridY+this.snap*(action==='gridUp'?1:-1)));this.render();return;}
    if(action==='gridBase'){this.gridY=t.center[1]-t.size[1]/2;this.render();return;}
    if(action==='pickGrid'){this.pickGrid=!this.pickGrid;this.placing=false;this.render();this.status(this.pickGrid?'Click a visible floor to set the placement grid height.':'Floor picking cancelled.');return;}
    if(action==='load'){if(this.project?.triggers.length&&!confirm('Discard this draft and load another mission? Save project first to keep it.'))return;return this.load(this.panel.querySelector('[data-field=mission]').value);}
    if(action==='add'){
      if(this.project.triggers.length>=32)throw Error('This proof of concept supports up to 32 trigger edits.');let n=1;while(this.project.triggers.some(x=>x.name==='custom_hint'+n)||this.info.triggers.some(x=>x.name==='custom_hint'+n))n++;
      this.project.triggers.push({name:'custom_hint'+n,center:[this.info.spawn[0],this.gridY+60,this.info.spawn[2]],size:[120,120,120],message:'Custom trigger works!',seconds:5,checkpoint:true,split_label:'Checkpoint '+n});this.selected=this.project.triggers.length-1;this.placing=true;this.pickGrid=false;this.render();return;
    }
    if(action==='existing'){this.showExisting=!this.showExisting;this.render();return;}
    if(action==='useExisting'){
      const value=this.panel.querySelector('[data-field=existing]').value;if(value==='')throw Error('Choose an existing trigger.');const original=this.info.triggers.find(x=>x.entity===Number(value));
      if(this.project.triggers.some(x=>x.existing_entity===original.entity))throw Error('That trigger is already in this draft.');
      const hint=original.scripts.map(s=>s.match(/^\s*touch:\s*hint\s+([\d.]+)\s+\((.*)\)\s*$/i)).find(Boolean);
      this.project.triggers.push({name:original.name,existing_entity:original.entity,center:[...original.center],size:[...original.size],message:hint?.[2]||'Custom trigger works!',seconds:hint?Number(hint[1]):5});this.selected=this.project.triggers.length-1;this.showExisting=false;this.placing=false;this.render();this.focus();return;
    }
    if(action==='place'){this.placing=!this.placing;this.pickGrid=false;this.render();return;}
    if(action==='move'||action==='resize'){this.tool=action;this.placing=false;this.pickGrid=false;this.render();return;}
    if(action==='earlier'||action==='later'){const list=this.project.triggers,step=action==='earlier'?-1:1;let j=this.selected+step;while(j>=0&&j<list.length&&!list[j].checkpoint)j+=step;if(j>=0&&j<list.length){[list[this.selected],list[j]]=[list[j],list[this.selected]];this.selected=j;}this.render();return;}
    if(action==='focus'){this.focus();return;}
    if(action==='delete'){this.project.triggers.splice(this.selected,1);this.selected=this.project.triggers.length-1;this.placing=false;this.render();return;}
    if(action==='save'){this.download(new Blob([JSON.stringify(this.project,null,2)],{type:'application/json'}),'fdny-level-project.json');return;}
    if(action==='import'){
      const input=document.createElement('input');input.type='file';input.accept='.json';input.onchange=async()=>{try{if(input.files[0].size>100000)throw Error('Project file is too large.');const p=JSON.parse(await input.files[0].text());if(p.schema!=='fdny-level-edit-v1'||!Array.isArray(p.triggers)||p.triggers.length>32||p.triggers.some(t=>!t||typeof t.name!=='string'||typeof t.message!=='string'||!Number.isFinite(t.seconds)||t.seconds<1||t.seconds>30||['center','size'].some(k=>!Array.isArray(t[k])||t[k].length!==3||t[k].some(v=>!Number.isFinite(v)||Math.abs(v)>100000))||t.size.some(v=>v<8||v>10000)||(t.checkpoint!=null&&typeof t.checkpoint!=='boolean')||(t.split_label!=null&&(typeof t.split_label!=='string'||!t.split_label.trim()||t.split_label.length>80||/[\u0000-\u001f]/.test(t.split_label)))))throw Error('Not a valid Level Edit project.');if(this.project?.triggers.length&&!confirm('Replace the current draft?'))return;await this.load(p.source);if(p.source_sha256!==this.info.source_sha256)throw Error('Project source map differs from your installed game.');this.project=p;this.selected=p.triggers.length?0:-1;this.render();}catch(err){this.status(err.message,true);}};input.click();return;
    }
    if(action==='export'){
      this.status('Building retail map and installation package…');e.target.disabled=true;
      try{const r=await this.api('export',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(this.project)});this.download(await r.blob(),'fdny-level-edit-'+this.project.source+'.zip');this.status('Export downloaded. Unzip and read README.txt for install and restore commands.');}finally{e.target.disabled=false;}
    }
  }
  async change(e){const t=this.project?.triggers[this.selected],el=e.target;
    if(el.dataset.field==='snap'){this.snap=Number(el.value);this.render();return;}
    if(el.dataset.field==='gridY'){const y=Number(el.value);if(Number.isFinite(y)&&Math.abs(y)<=100000)this.gridY=y;this.render();return;}
    if(!t)return;
    if(el.dataset.vector){const value=Number(el.value);if(!Number.isFinite(value)||Math.abs(value)>100000||(el.dataset.vector==='size'&&(value<8||value>10000))){el.value=t[el.dataset.vector][Number(el.dataset.axis)];this.status('Use finite coordinates within 100000 and sizes between 8 and 10000.',true);return;}t[el.dataset.vector][Number(el.dataset.axis)]=value;this.dirty=true;this.host.redraw();}
    else if(el.dataset.field==='checkpoint'){t.checkpoint=el.checked;this.render();}
    else if(['name','message','seconds','split_label'].includes(el.dataset.field)){t[el.dataset.field]=el.dataset.field==='seconds'?Number(el.value):el.value;this.dirty=true;this.host.redraw();if(['name','split_label'].includes(el.dataset.field))this.render();}
  }
  focus(){const t=this.project?.triggers[this.selected];if(t)this.host.focusBounds({min:t.center.map((v,a)=>v-t.size[a]/2),max:t.center.map((v,a)=>v+t.size[a]/2)});}
  viewPreset(top){const s=this.host.state();if(!s.camera||!this.info)return;const t=this.project?.triggers[this.selected],center=t?t.center:[this.info.spawn[0],this.gridY,this.info.spawn[2]],radius=Math.max(t?Math.max(...t.size)*3:700,500);this.host.focusBounds({min:center.map(v=>v-radius/2),max:center.map(v=>v+radius/2)});s.camera.position.copy(s.controls.target).add(new THREE.Vector3(...(top?[0,radius*1.5,.01]:[radius*.6,radius,radius])));s.controls.update();this.host.redraw();}
  download(blob,name){const url=URL.createObjectURL(blob),a=document.createElement('a');a.href=url;a.download=name;a.click();setTimeout(()=>URL.revokeObjectURL(url),10000);}
  ray(e){const s=this.host.state(),r=s.renderer.domElement.getBoundingClientRect(),ray=new THREE.Raycaster();s.camera.updateMatrixWorld();s.scene.updateMatrixWorld(true);ray.setFromCamera(new THREE.Vector2((e.clientX-r.left)/r.width*2-1,-(e.clientY-r.top)/r.height*2+1),s.camera);return ray;}
  placementPoint(e){const point=this.ray(e).ray.intersectPlane(new THREE.Plane(new THREE.Vector3(0,1,0),-this.gridY),new THREE.Vector3());if(!point)return null;const step=this.snap||1;point.x=Math.round(point.x/step)*step;point.z=Math.round(point.z/step)*step;return Math.abs(point.x)<=100000&&Math.abs(point.z)<=100000?point:null;}
  previewPlacement(e){
    const t=this.project?.triggers[this.selected],p=this.placementPoint(e);if(!t||!p)return false;
    const d=this.placementDrag;
    if(d&&Math.hypot(e.clientX-d.x,e.clientY-d.y)>4)d.draw=true;
    if(d?.draw){for(const a of [0,2]){t.size[a]=Math.max(8,Math.min(10000,Math.abs(p.getComponent(a)-d.start.getComponent(a))));t.center[a]=d.start.getComponent(a)+Math.sign(p.getComponent(a)-d.start.getComponent(a))*t.size[a]/2;}}
    else{t.center[0]=p.x;t.center[2]=p.z;}
    t.center[1]=this.gridY+t.size[1]/2;this.dirty=true;this.host.redraw();return true;
  }
  finishPlacement(cancel=false){const d=this.placementDrag;if(!d)return;if(cancel){d.trigger.center=d.center;d.trigger.size=d.size;}else{this.placing=false;this.tool='move';}const s=this.host.state();if(s.controls)s.controls.enabled=d.controlsEnabled;this.placementDrag=null;this.render();}
  bindCanvas(canvas){
    // Capture before OrbitControls so dragging a handle never rotates the camera.
    canvas.addEventListener('pointerdown',e=>{if(this.active&&this.placing&&e.button===0){const p=this.placementPoint(e),t=this.project?.triggers[this.selected];if(!p||!t)return;const s=this.host.state();this.placementDrag={start:p,x:e.clientX,y:e.clientY,trigger:t,center:[...t.center],size:[...t.size],controlsEnabled:s.controls?.enabled};if(s.controls)s.controls.enabled=false;this.previewPlacement(e);}else if(!this.beginDrag(e))return;e.preventDefault();e.stopImmediatePropagation();canvas.setPointerCapture(e.pointerId);},true);
    canvas.addEventListener('pointermove',e=>{if(this.active&&this.placing){this.previewPlacement(e);if(!this.placementDrag)return;}else if(!this.drag)return;e.preventDefault();e.stopImmediatePropagation();if(this.drag)this.moveDrag(e);},true);
    for(const type of ['pointerup','pointercancel','lostpointercapture'])canvas.addEventListener(type,e=>{if(!this.drag&&!this.placementDrag)return;e.preventDefault();e.stopImmediatePropagation();if(this.placementDrag)this.finishPlacement(type!=='pointerup');else this.endDrag(type!=='pointerup');if(canvas.hasPointerCapture(e.pointerId))canvas.releasePointerCapture(e.pointerId);},true);
  }
  beginDrag(e){
    const s=this.host.state(),t=this.project?.triggers[this.selected];
    if(e.button!==0||!this.active||this.placing||this.pickGrid||!t||t.existing_entity!=null||s.mode!=='3d'||s.data?.key!==this.project.source)return false;
    const hit=this.gizmo&&this.ray(e).intersectObjects(this.gizmo.children.filter(x=>x.isMesh),false)[0];if(!hit)return false;
    const {axis,sign}=hit.object.userData,r=s.renderer.domElement.getBoundingClientRect();
    const origin=new THREE.Vector3(...t.center),unit=new THREE.Vector3();unit.setComponent(axis,1);
    const a=origin.clone().project(s.camera),b=origin.clone().addScaledVector(unit,this.handleUnit).project(s.camera);
    const dx=(b.x-a.x)*r.width/2,dy=-(b.y-a.y)*r.height/2,length=dx*dx+dy*dy;
    if(length<64){this.status('This axis points toward the camera. Orbit slightly to drag it.');return true;}
    this.drag={axis,sign,x:e.clientX,y:e.clientY,dx,dy,length,unit:this.handleUnit,center:[...t.center],size:[...t.size],trigger:t,controlsEnabled:s.controls?.enabled};
    if(s.controls)s.controls.enabled=false;return true;
  }
  moveDrag(e){
    const d=this.drag;if(!d)return;const t=d.trigger,a=d.axis,step=e.shiftKey?1:(this.snap||1);
    let delta=((e.clientX-d.x)*d.dx+(e.clientY-d.y)*d.dy)/d.length*d.unit;
    if(e.shiftKey)delta*=.1;delta=Math.round(delta/step)*step;
    if(d.sign){const size=Math.max(8,Math.min(10000,d.size[a]+delta*d.sign));t.size[a]=size;t.center[a]=d.center[a]+d.sign*(size-d.size[a])/2;}
    else t.center[a]=Math.max(-100000,Math.min(100000,d.center[a]+delta));
    this.dirty=true;this.host.redraw();
    for(const kind of ['center','size']){const input=this.panel?.querySelector(`[data-vector="${kind}"][data-axis="${a}"]`);if(input)input.value=t[kind][a];}
  }
  endDrag(cancel=false){if(!this.drag)return;const d=this.drag,s=this.host.state();if(cancel){d.trigger.center=d.center;d.trigger.size=d.size;}if(s.controls)s.controls.enabled=d.controlsEnabled;this.drag=null;this.render();}
  clearGizmo(){this.gizmoKey=null;if(!this.gizmo)return;for(const x of this.gizmo.children){x.geometry?.dispose();x.material?.dispose();}this.gizmo.removeFromParent();this.gizmo=null;}
  updateGizmo(){
    const t=this.project?.triggers[this.selected],s=this.host.state();
    if(!this.group.visible||this.placing||this.pickGrid||!t||t.existing_entity!=null){this.clearGizmo();return;}
    s.camera.updateMatrixWorld();
    const r=s.renderer.domElement.getBoundingClientRect(),origin=new THREE.Vector3(...t.center);
    // Constant screen size; all transforms remain in the game's world axes.
    const depth=Math.max(s.camera.near,origin.clone().applyMatrix4(s.camera.matrixWorldInverse).z*-1);
    const unit=2*depth*Math.tan(s.camera.fov*Math.PI/360)/r.height*80;this.handleUnit=unit;
    const key=[this.selected,this.tool,...t.center,...t.size,unit].join(',');if(this.gizmoKey===key)return;this.clearGizmo();this.gizmoKey=key;
    this.gizmo=new THREE.Group();this.gizmo.position.copy(origin);this.group.add(this.gizmo);
    const colors=[0xe24d4d,0x33a663,0x4388ed];
    for(let axis=0;axis<3;axis++){
      const direction=new THREE.Vector3().setComponent(axis,1),rotation=new THREE.Quaternion().setFromUnitVectors(new THREE.Vector3(0,1,0),direction);
      for(const sign of this.tool==='resize'?[-1,1]:[0]){
        const distance=sign?sign*t.size[axis]/2:unit;
        const material=new THREE.MeshBasicMaterial({color:colors[axis],depthTest:false,depthWrite:false});
        const geometry=sign?new THREE.BoxGeometry(unit*.16,unit*.16,unit*.16):new THREE.ConeGeometry(unit*.09,unit*.25,12);
        const handle=new THREE.Mesh(geometry,material);handle.position.copy(direction).multiplyScalar(distance);if(!sign)handle.quaternion.copy(rotation);handle.userData={axis,sign,levelEdit:true};handle.renderOrder=910;this.gizmo.add(handle);
        if(!sign){const shaft=new THREE.Mesh(new THREE.CylinderGeometry(unit*.025,unit*.025,unit*.85,8),material.clone());shaft.position.copy(direction).multiplyScalar(unit*.425);shaft.quaternion.copy(rotation);shaft.userData={axis,sign,levelEdit:true};shaft.renderOrder=910;this.gizmo.add(shaft);}
        const line=new THREE.Line(new THREE.BufferGeometry().setFromPoints([new THREE.Vector3(),direction.clone().multiplyScalar(distance)]),new THREE.LineBasicMaterial({color:colors[axis],depthTest:false,depthWrite:false,transparent:true,opacity:.8}));line.renderOrder=909;this.gizmo.add(line);
      }
    }
  }
  pointerClick(e){
    const s=this.host.state();if(!this.active||!this.project||s.mode!=='3d'||s.data?.key!==this.project.source)return false;
    s.camera.updateMatrixWorld();s.scene.updateMatrixWorld(true);const rect=s.renderer.domElement.getBoundingClientRect(),ray=new THREE.Raycaster();ray.setFromCamera(new THREE.Vector2((e.clientX-rect.left)/rect.width*2-1,-(e.clientY-rect.top)/rect.height*2+1),s.camera);
    if(this.pickGrid){const targets=[];s.scene.traverse(x=>{if(x.isMesh&&x.userData.pickIds&&!x.userData.levelEdit){let visible=true;for(let p=x;p;p=p.parent)if(!p.visible)visible=false;if(visible&&x.geometry?.attributes.position)targets.push(x);}});const hit=ray.intersectObjects(targets,false).find(h=>{const material=Array.isArray(h.object.material)?h.object.material[h.face.materialIndex]:h.object.material;return !(material.clippingPlanes||[]).some(p=>p.distanceToPoint(h.point)<0);});if(!hit){this.status('No visible surface here. Try Top view or orbit to expose a floor.',true);return true;}this.gridY=Math.round(hit.point.y*10)/10;this.pickGrid=false;this.render();return true;}
    if(this.placing){if(this.previewPlacement(e)){this.placing=false;this.tool='move';this.render();}else this.status('The grid is edge-on. Choose Top view to place a box.',true);return true;}
    const hit=this.group&&ray.intersectObjects(this.group.children,false).find(x=>x.object.userData.triggerIndex!=null);if(hit){this.selected=hit.object.userData.triggerIndex;this.render();return true;}return false;
  }
  update3D(){
    const s=this.host.state();if(!s.scene)return;
    if(this.group&&this.group.parent!==s.scene){this.dispose();}
    if(!this.group){this.group=new THREE.Group();s.scene.add(this.group);this.dirty=true;}
    this.group.visible=this.active&&s.data?.key===this.project?.source;
    this.updateGrid();
    if(!this.dirty||!this.project){this.updateGizmo();return;}
    this.clearGizmo();
    for(const x of [...this.group.children]){x.geometry?.dispose();x.material?.dispose();this.group.remove(x);}
    this.project.triggers.forEach((t,i)=>{
      const geometry=new THREE.BoxGeometry(...t.size),material=new THREE.MeshBasicMaterial({color:i===this.selected?0xff8a22:0x14a5a0,transparent:true,opacity:this.placing&&i===this.selected ? .08 : .15,depthTest:false,depthWrite:false,side:THREE.DoubleSide});const mesh=new THREE.Mesh(geometry,material);mesh.position.set(...t.center);mesh.userData={levelEdit:true,triggerIndex:i};mesh.renderOrder=900;this.group.add(mesh);
      const edge=new THREE.LineSegments(new THREE.EdgesGeometry(geometry),new THREE.LineBasicMaterial({color:i===this.selected?0xff8a22:0x14a5a0,depthTest:false,transparent:true,opacity:.95}));edge.position.copy(mesh.position);edge.userData.levelEdit=true;edge.renderOrder=901;this.group.add(edge);
    });this.dirty=false;this.updateGizmo();
  }
  updateGrid(){const s=this.host.state();if(this.grid&&this.grid.parent!==s.scene){this.grid.geometry.dispose();this.grid.material.dispose();this.grid=null;this.gridKey=null;}const b=s.data?.bounds,step=Math.max(16,this.snap||16),span=b?Math.max(b.maxX-b.minX,b.maxZ-b.minZ)+step*4:2048,divisions=Math.min(1024,Math.ceil(span/step/2)*2),key=[this.project?.source,step,divisions].join(',');if(this.group.visible&&key!==this.gridKey){if(this.grid){this.grid.geometry.dispose();this.grid.material.dispose();this.grid.removeFromParent();}this.grid=new THREE.GridHelper(divisions*step,divisions,0x496c73,0x719c9c);this.grid.material.transparent=true;this.grid.material.opacity=.28;this.grid.material.depthWrite=false;this.grid.material.depthTest=false;this.grid.renderOrder=850;this.grid.position.x=b?Math.round((b.minX+b.maxX)/2/step)*step:0;this.grid.position.z=b?Math.round((b.minZ+b.maxZ)/2/step)*step:0;s.scene.add(this.grid);this.gridKey=key;}if(this.grid){this.grid.visible=this.group.visible;this.grid.position.y=this.gridY||0;}}
  dispose(){this.endDrag(true);this.finishPlacement(true);this.clearGizmo();if(this.grid){this.grid.geometry.dispose();this.grid.material.dispose();this.grid.removeFromParent();this.grid=null;this.gridKey=null;}if(!this.group)return;for(const x of this.group.children){x.geometry?.dispose();x.material?.dispose();}this.group.removeFromParent();this.group=null;}
}
