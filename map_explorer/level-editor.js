import * as THREE from 'three';
const escape=s=>String(s??'').replace(/[&<>"']/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
const clone=value=>JSON.parse(JSON.stringify(value));
export class LevelEditor {
  constructor(host){
    Object.assign(this,{host,selected:-1,active:false,placing:false,group:null,tool:'select',snap:16,gridY:0,tab:'volumes',undoStack:[],redoStack:[]});
    this.panel=document.createElement('section');this.panel.id='levelEditor';this.panel.hidden=true;this.panel.setAttribute('aria-label','Custom Triggers');host.view.parentElement.append(this.panel);
    this.panel.addEventListener('click',e=>this.click(e).catch(err=>this.status(err.message,true)));
    this.panel.addEventListener('change',e=>this.change(e).catch(err=>this.status(err.message,true)));
    document.getElementById('levelEdit').onclick=()=>{document.getElementById('toolsMenu').open=false;if(this.active)this.close();else this.open().catch(err=>this.status(err.message,true));};
    window.addEventListener('beforeunload',e=>{if(this.project?.triggers.length){e.preventDefault();e.returnValue='';}});
    window.addEventListener('keydown',e=>this.keyDown(e),true);
    window.addEventListener('blur',()=>{this.endDrag(true);this.finishPlacement(true);});
    this.init();
  }
  async init(){
    try{const r=await fetch('data/editor/missions.json');if(!r.ok)throw Error('Rescue missions unavailable.');this.missions=await r.json();document.getElementById('levelEdit').hidden=false;}catch(error){console.error(error);}
  }
  async open(){this.active=true;this.panel.hidden=false;this.host.view.parentElement.classList.add('editor-mode');document.getElementById('levelEdit').setAttribute('aria-pressed','true');if(!this.project){this.render();await this.load(this.missions?.some(m=>m.key===this.host.state().data?.key)?this.host.state().data.key:'training');}else{if(this.host.state().data?.key!==this.project.source)await this.host.loadStage(this.project.source);if(this.host.state().mode!=='3d')this.host.setMode('3d');this.render();}this.host.redraw();}
  close(){this.cancelInteraction();this.active=false;this.panel.hidden=true;this.host.view.parentElement.classList.remove('editor-mode');document.getElementById('levelEdit').setAttribute('aria-pressed','false');this.host.redraw();}
  status(message,error=false){const el=this.panel.querySelector('[data-status]');if(el){el.textContent=message;el.classList.toggle('error',error);}}
  snapshot(){return this.project?{project:clone(this.project),selected:this.selected}:null;}
  remember(before){if(!before||JSON.stringify(before.project)===JSON.stringify(this.project))return;(this.undoStack??=[]).push(before);if(this.undoStack.length>50)this.undoStack.shift();this.redoStack=[];}
  edit(change){this.cancelInteraction();const before=this.snapshot();change();this.remember(before);this.render();}
  history(redo=false){
    if(this.pending||this.drag||this.placementDrag){this.cancelInteraction();this.render();return;}
    const from=redo?this.redoStack:this.undoStack,to=redo?(this.undoStack??=[]):(this.redoStack??=[]);if(!from?.length)return;
    const previous=from.pop();to.push(this.snapshot());this.project=previous.project;this.selected=previous.selected;this.pickGrid=false;this.render();this.revealSelection();
  }
  cancelInteraction(){this.endDrag(true);this.finishPlacement(true);this.pending=null;this.placing=false;this.pickGrid=false;}
  select(index){if(!this.project?.triggers[index])return;this.cancelInteraction();this.selected=index;this.tab='volumes';this.render();}
  uniqueName(base='volume'){
    let root=base.replace(/[^A-Za-z0-9_]/g,'_');if(!/^[A-Za-z]/.test(root))root='volume_'+root;root=root.slice(0,30);
    const names=new Set([...this.project.triggers,...(this.info?.triggers||[])].map(x=>x.name.toLowerCase()));let n=1;while(names.has((root+n).toLowerCase()))n++;return root+n;
  }
  copy(){const t=this.project?.triggers[this.selected];if(t){this.clipboard={trigger:clone(t),source:this.project.source};this.pasteOffset=0;}}
  paste(){
    if(!this.clipboard||this.project.triggers.length>=32)return;
    const t=clone(this.clipboard.trigger);delete t.existing_entity;t.name=this.uniqueName(t.name+'_copy');
    const base=(this.clipboard.trigger.label||this.clipboard.trigger.name).slice(0,65)+' copy',labels=new Set(this.project.triggers.map(x=>x.label||x.name));let label=base,n=2;while(labels.has(label))label=base+' '+n++;t.label=label;
    const step=(this.snap||16)*2*(this.pasteOffset=(this.pasteOffset||0)+1);
    if(this.clipboard.source!==this.project.source){const target=this.host.state().controls?.target;t.center=[target?.x??this.info.spawn[0],this.gridY+t.size[1]/2,target?.z??this.info.spawn[2]];}
    else for(const a of [0,2])t.center[a]=Math.max(-100000,Math.min(100000,t.center[a]+step));
    this.edit(()=>{this.project.triggers.push(t);this.selected=this.project.triggers.length-1;this.tool='move';this.tab='volumes';});this.revealSelection();
  }
  newVolume(){
    if(!this.project||this.project.triggers.length>=32)return;this.cancelInteraction();
    const selected=this.project.triggers[this.selected];if(selected)this.gridY=selected.center[1]-selected.size[1]/2;
    const target=this.host.state().controls?.target,name=this.uniqueName(),labels=new Set(this.project.triggers.map(x=>x.label||x.name));let n=1;while(labels.has('Checkpoint '+n))n++;
    // A preview is not a project volume. Navigation/cancellation cannot move or delete placed boxes.
    this.pending={name,center:[target?.x??this.info.spawn[0],this.gridY+60,target?.z??this.info.spawn[2]],size:[120,120,120],label:'Checkpoint '+n};
    this.placing=true;this.pendingVisible=false;this.tab='volumes';this.render();
  }
  keyDown(e){
    if(!this.active||e.target?.isContentEditable||/input|select|textarea/i.test(e.target?.tagName||''))return;
    const modifier=e.ctrlKey||e.metaKey,key=e.key.toLowerCase();let action;
    if(!modifier&&(key==='f'||(this.host.state().flying&&['w','a','s','d','q','e','shift','arrowup','arrowdown','arrowleft','arrowright','escape'].includes(key))))return;
    if(modifier)action=({z:e.shiftKey?'redo':'undo',y:'redo',c:'copy',v:'paste',d:'duplicate',s:'save'})[key];
    else if(!e.altKey)action=({v:'selectTool',n:'add',g:'move',s:'resize','.':'focus',delete:'delete',backspace:'delete',escape:'cancel'})[key];
    if(!action)return;e.preventDefault();e.stopImmediatePropagation();if(e.repeat&&['add','paste','duplicate','delete'].includes(action))return;
    this.command(action).catch(err=>this.status(err.message,true));
  }
  render(){
    const p=this.project,t=p?.triggers[this.selected],editable=t&&t.existing_entity==null,busy=this.placing;
    const hint=busy?'Preview only · click to place, or drag a footprint. Esc cancels.':this.pickGrid?'Click a floor to set placement height.':this.tool==='move'?'Drag a box along the floor, or drag X / Y / Z arrows.':this.tool==='resize'?'Drag a face handle to resize. The opposite face stays fixed.':'Click a box to select it. Drag empty space to orbit.';
    this.panel.innerHTML=`<div class="le-head"><strong>Custom Triggers</strong><button data-action="close">Exit</button></div>
    ${p?`<div class="le-toolbar"><div class="le-modes"><button data-action="selectTool" aria-pressed="${!busy&&this.tool==='select'}">Select <kbd>V</kbd></button><button data-action="add" aria-pressed="${busy}" ${p.triggers.length>=32?'disabled':''}>＋ New volume <kbd>N</kbd></button><button data-action="move" aria-pressed="${!busy&&this.tool==='move'}" ${editable?'':'disabled'}>Move <kbd>G</kbd></button><button data-action="resize" aria-pressed="${!busy&&this.tool==='resize'}" ${editable?'':'disabled'}>Resize <kbd>S</kbd></button></div>
    <div class="le-actions"><button data-action="copy" title="Copy · Ctrl/⌘ C" ${t&&!busy?'':'disabled'}>Copy</button><button data-action="paste" title="Paste · Ctrl/⌘ V" ${this.clipboard&&p.triggers.length<32?'':'disabled'}>Paste</button><button data-action="duplicate" title="Duplicate · Ctrl/⌘ D" ${t&&!busy&&p.triggers.length<32?'':'disabled'}>Duplicate</button><button data-action="delete" title="Delete · undo restores the volume" ${t&&!busy?'':'disabled'}>Delete</button></div>
    <div class="le-row"><button data-action="undo" title="Undo · Ctrl/⌘ Z" ${this.pending||this.undoStack?.length?'':'disabled'}>↶ Undo</button><button data-action="redo" title="Redo · Ctrl/⌘ Shift Z" ${this.redoStack?.length?'':'disabled'}>↷ Redo</button><button data-action="focus" title="Find selected volume · ." ${t?'':'disabled'}>Find <kbd>.</kbd></button></div><p class="le-help">${hint}</p></div>
    <div class="le-section le-trigger-list" role="group" aria-label="Placed volumes"><h3>Checkpoints (${p.triggers.length})</h3>${p.triggers.length?p.triggers.map((x,i)=>`<button data-action="select" data-index="${i}" aria-pressed="${i===this.selected}"><span class="le-volume-label">${escape(x.label||x.name)}</span></button>`).join(''):'<p class="le-help">No checkpoints yet. Choose New volume, then click the grid.</p>'}</div>
    ${t&&!busy?`<div class="le-section"><h3>Selected checkpoint</h3><label>Label <input data-field="label" maxlength="80" value="${escape(t.label||t.name)}"></label><p class="le-help">Displays elapsed level time, for example 00:15:243. Each checkpoint fires once per attempt.</p><button data-action="details" aria-expanded="${!!this.details}">${this.details?'Hide':'Show'} exact values</button>
    ${this.details?`<div class="le-coordinates">${['center','size'].map(kind=>`<span>${kind==='center'?'Position':'Size'}</span>${['X','Y','Z'].map((axis,a)=>`<label>${axis}<input type="number" step="1" ${kind==='size'?'min="8" max="10000"':''} data-vector="${kind}" data-axis="${a}" value="${t[kind][a]}"></label>`).join('')}`).join('')}</div>`:''}</div>`:''}
    <div class="le-section le-grid"><h3>Placement & view</h3><div class="le-row"><label>Height <input data-field="gridY" aria-label="Placement height" type="number" step="${this.snap}" value="${this.gridY}"></label><button data-action="pickGrid">${this.pickGrid?'Cancel':'Pick floor'}</button></div><div class="le-row"><label>Snap <select data-field="snap">${[1,8,16,32].map(n=>`<option ${this.snap===n?'selected':''}>${n}</option>`).join('')}</select></label><button data-action="top">Top</button><button data-action="perspective">Perspective</button></div></div>
    <div class="le-section"><h3>${escape(this.missions?.find(m=>m.key===p.source)?.title||p.source)}</h3><button data-action="mission">${this.showMission?'Cancel mission change':'Change mission…'}</button>${this.showMission?this.missionControls():''}<button data-action="existing">Use existing bounds…</button>${this.showExisting?`<div class="le-row"><select data-field="existing" aria-label="Existing trigger"><option value="">Choose a volume…</option>${this.info.triggers.map(x=>`<option value="${x.entity}">${escape(x.name)}</option>`).join('')}</select><button data-action="useExisting">Use</button></div>`:''}</div>
    <div class="le-section le-export"><button data-action="export" ${p.triggers.length&&!busy?'':'disabled'}>Export checkpoints</button><p class="le-help">Requires the practice-checkpoint FDNYCore build. Put ${escape(p.source)}.txt in the game's Checkpoints folder, beside FDNYFirefighter.exe. Restart the mission. V toggles timestamps (saved).</p><div class="le-row"><button data-action="save">Save project</button><button data-action="import">Open project</button></div></div>`:this.missionControls()}
    <p data-status role="status"></p>`;this.dirty=true;this.host.redraw();
  }
  missionControls(){return `<div class="le-row"><select data-field="mission" aria-label="Rescue mission"><option value="">Choose a mission…</option>${(this.missions||[]).map(m=>`<option value="${escape(m.key)}" ${this.project?.source===m.key?'selected':''}>${escape(m.title)}</option>`).join('')}</select><button data-action="load">Load</button></div>`;}
  async load(key,project=null){if(!key)throw Error('Choose a rescue mission.');const info=this.missions?.find(m=>m.key===key)?.info;if(!info)throw Error('Choose a rescue mission.');if(project&&project.source_sha256!==info.source_sha256)throw Error('Project source map differs from this mission.');const sameView=this.host.state().data?.key===key&&this.host.state().mode==='3d';if(this.host.state().data?.key!==key)await this.host.loadStage(key);if(this.host.state().mode!=='3d')this.host.setMode('3d');this.host.cutRoof?.();this.info=info;this.project=project||{schema:'fdny-level-edit-v1',source:key,source_sha256:info.source_sha256,triggers:[]};Object.assign(this,{selected:this.project.triggers.length?0:-1,placing:false,pending:null,pickGrid:false,showExisting:false,showMission:false,undoStack:[],redoStack:[],tool:'select',tab:'volumes',gridY:info.spawn[1]-30});this.render();if(!sameView)this.viewPreset(false);}
  async click(e){const button=e.target.closest('[data-action]');if(button)return this.command(button.dataset.action,button.dataset.index,button);}
  async command(action,index,button){
    const t=this.project?.triggers[this.selected];
    if(action==='close'){this.close();return;}
    if(action==='cancel'){this.cancelInteraction();this.tool='select';this.render();return;}
    if(action==='select'){this.select(Number(index));return;}
    if(action==='selectTool'||action==='move'||action==='resize'){if(action!=='selectTool'&&(!t||t.existing_entity!=null))return;this.cancelInteraction();this.tool=action==='selectTool'?'select':action;this.render();return;}
    if(action==='add'){this.newVolume();return;}
    if(action==='copy'){if(!this.pending)this.copy();this.render();return;}
    if(action==='paste'){this.paste();return;}
    if(action==='duplicate'){if(t&&!this.pending){const clipboard=this.clipboard,offset=this.pasteOffset;this.copy();this.paste();this.clipboard=clipboard;this.pasteOffset=offset;this.render();}return;}
    if(action==='delete'){if(this.pending){this.cancelInteraction();this.render();return;}if(t)this.edit(()=>{this.project.triggers.splice(this.selected,1);this.selected=Math.min(this.selected,this.project.triggers.length-1);});return;}
    if(action==='undo'||action==='redo'){this.history(action==='redo');return;}
    if(action==='top'||action==='perspective'){this.viewPreset(action==='top');return;}
    if(action==='focus'){this.focus();return;}
    if(action==='pickGrid'){const pick=!this.pickGrid;this.cancelInteraction();this.pickGrid=pick;this.render();return;}
    if(action==='details'||action==='mission'||action==='existing'){const field={details:'details',mission:'showMission',existing:'showExisting'}[action];this[field]=!this[field];this.render();return;}
    if(action==='load'){const key=this.panel.querySelector('[data-field=mission]').value;if(key===this.project?.source){this.showMission=false;this.render();return;}if(this.project?.triggers.length&&!confirm('Switch mission? Save your project first to keep these volumes.'))return;this.cancelInteraction();return this.load(key);}
    if(action==='useExisting'){
      const value=this.panel.querySelector('[data-field=existing]').value;if(value==='')throw Error('Choose an existing trigger.');const original=this.info.triggers.find(x=>x.entity===Number(value));
      if(this.project.triggers.some(x=>x.existing_entity===original.entity))throw Error('That trigger is already in this draft.');
      this.edit(()=>{this.project.triggers.push({name:this.uniqueName(),label:original.name,center:[...original.center],size:[...original.size]});this.selected=this.project.triggers.length-1;this.showExisting=false;});this.focus();return;
    }
    if(action==='save'){this.download(new Blob([JSON.stringify(this.project,null,2)],{type:'application/json'}),'fdny-level-project.json');return;}
    if(action==='import'){
      const input=document.createElement('input');input.type='file';input.accept='.json';input.onchange=async()=>{try{if(!input.files[0])return;if(input.files[0].size>100000)throw Error('Project file is too large.');const p=JSON.parse(await input.files[0].text());if(p.schema!=='fdny-level-edit-v1'||!Array.isArray(p.triggers)||p.triggers.length>32)throw Error('Not a valid Custom Triggers project.');p.triggers=p.triggers.map((t,i)=>({name:typeof t.name==='string'?t.name:'volume'+(i+1),label:t.label??t.split_label??t.name,center:t.center,size:t.size}));if(p.triggers.length){const {checkpointFile}=await import('./checkpoints.js');checkpointFile(p);}if(this.project?.triggers.length&&!confirm('Replace the current draft?'))return;this.cancelInteraction();await this.load(p.source,p);}catch(err){this.status(err.message,true);}};input.click();return;
    }
    if(action==='export'){
      const {checkpointFile}=await import('./checkpoints.js');
      this.download(new Blob([checkpointFile(this.project)],{type:'text/plain'}),this.project.source+'.txt');
      this.status('Downloaded. Put the file in the game’s Checkpoints folder, restart the mission, and press V to enable timestamps.');
    }
  }
  propertyEdit(change){
    const before=this.snapshot();change();this.remember(before);this.dirty=true;this.host.redraw();
    // Keep the current inputs mounted: changing a field must not swallow a click
    // or steal focus from the next field. Refresh only the affected row/buttons.
    for(const action of ['undo','redo']){const button=this.panel.querySelector(`[data-action="${action}"]`);if(button)button.disabled=!(action==='undo'?this.undoStack?.length:this.redoStack?.length);}
    const t=this.project.triggers[this.selected],row=this.panel.querySelector(`[data-action=select][data-index="${this.selected}"]`);
    const label=row?.querySelector('.le-volume-label');if(label)label.textContent=t.label||t.name;
  }
  async change(e){const t=this.project?.triggers[this.selected],el=e.target;
    if(el.dataset.field==='snap'){this.snap=Number(el.value);this.render();return;}
    if(el.dataset.field==='gridY'){const y=Number(el.value);if(Number.isFinite(y)&&Math.abs(y)<=100000)this.gridY=y;this.render();return;}
    if(!t)return;
    if(el.dataset.vector){const value=Number(el.value);if(!Number.isFinite(value)||Math.abs(value)>100000||(el.dataset.vector==='size'&&(value<8||value>10000))){el.value=t[el.dataset.vector][Number(el.dataset.axis)];this.status('Use finite coordinates within 100000 and sizes between 8 and 10000.',true);return;}this.propertyEdit(()=>{t[el.dataset.vector][Number(el.dataset.axis)]=value;});}
    else if(el.dataset.field==='label')this.propertyEdit(()=>{t.label=el.value;});
  }
  revealSelection(){const t=this.project?.triggers[this.selected],s=this.host.state();if(!t||!s.camera||s.mode!=='3d')return;s.camera.updateMatrixWorld();const p=new THREE.Vector3(...t.center).project(s.camera);if(Math.abs(p.x)>.9||Math.abs(p.y)>.9||p.z< -1||p.z>1)this.focus();}
  focus(){const t=this.project?.triggers[this.selected];if(t)this.host.focusBounds({min:t.center.map((v,a)=>v-t.size[a]/2),max:t.center.map((v,a)=>v+t.size[a]/2)});}
  viewPreset(top){const s=this.host.state();if(!s.camera||!this.info)return;const t=this.project?.triggers[this.selected],center=t?t.center:[this.info.spawn[0],this.gridY,this.info.spawn[2]],radius=Math.max(t?Math.max(...t.size)*3:700,500);this.host.focusBounds({min:center.map(v=>v-radius/2),max:center.map(v=>v+radius/2)});s.camera.position.copy(s.controls.target).add(new THREE.Vector3(...(top?[0,radius*1.5,.01]:[radius*.6,radius,radius])));s.controls.update();this.host.redraw();}
  download(blob,name){const url=URL.createObjectURL(blob),a=document.createElement('a');a.href=url;a.download=name;a.click();setTimeout(()=>URL.revokeObjectURL(url),10000);}
  ray(e){const s=this.host.state(),r=s.renderer.domElement.getBoundingClientRect(),ray=new THREE.Raycaster();s.camera.updateMatrixWorld();s.scene.updateMatrixWorld(true);ray.setFromCamera(new THREE.Vector2((e.clientX-r.left)/r.width*2-1,-(e.clientY-r.top)/r.height*2+1),s.camera);return ray;}
  editing(){const s=this.host.state();return this.active&&this.project&&s.mode==='3d'&&s.data?.key===this.project.source;}
  placementPoint(e){const point=this.ray(e).ray.intersectPlane(new THREE.Plane(new THREE.Vector3(0,1,0),-this.gridY),new THREE.Vector3());if(!point)return null;const step=this.snap||1;point.x=Math.round(point.x/step)*step;point.z=Math.round(point.z/step)*step;return Math.abs(point.x)<=100000&&Math.abs(point.z)<=100000?point:null;}
  previewPlacement(e){
    if(!this.placing||!this.pending)return false;const t=this.pending,p=this.placementPoint(e);this.pendingVisible=!!p;if(!p){this.dirty=true;this.host.redraw();return false;}
    const d=this.placementDrag;if(d&&Math.hypot(e.clientX-d.x,e.clientY-d.y)>4)d.draw=true;
    if(d?.draw){for(const a of [0,2]){const delta=p.getComponent(a)-d.start.getComponent(a);t.size[a]=Math.max(8,Math.min(10000,Math.abs(delta)));t.center[a]=d.start.getComponent(a)+(Math.sign(delta)||1)*t.size[a]/2;}}
    else{t.center[0]=p.x;t.center[2]=p.z;}
    t.center[1]=this.gridY+t.size[1]/2;this.dirty=true;this.host.redraw();return true;
  }
  commitPlacement(){if(!this.pending||this.project.triggers.length>=32)return;const before=this.snapshot();this.project.triggers.push(this.pending);this.pending=null;this.selected=this.project.triggers.length-1;this.placing=false;this.tool='move';this.remember(before);this.render();}
  finishPlacement(cancel=false){const d=this.placementDrag;if(!d)return;if(cancel&&this.pending){this.pending.center=d.center;this.pending.size=d.size;}const s=this.host.state();if(s.controls)s.controls.enabled=d.controlsEnabled;this.placementDrag=null;if(cancel)this.render();else this.commitPlacement();}
  bindCanvas(canvas){
    // Capture editor gestures before OrbitControls. Empty space still navigates normally.
    canvas.addEventListener('pointerdown',e=>{if(!this.editing())return;
      if(this.placing&&e.button===0){const p=this.placementPoint(e);if(!p||!this.pending)return;const s=this.host.state();this.placementDrag={start:p,x:e.clientX,y:e.clientY,center:[...this.pending.center],size:[...this.pending.size],controlsEnabled:s.controls?.enabled};if(s.controls)s.controls.enabled=false;this.previewPlacement(e);}
      else if(!this.beginDrag(e))return;
      e.preventDefault();e.stopImmediatePropagation();canvas.focus?.({preventScroll:true});canvas.setPointerCapture(e.pointerId);
    },true);
    canvas.addEventListener('pointermove',e=>{if(!this.editing())return;if(this.placing){if((e.buttons||0)&6)return;this.previewPlacement(e);if(!this.placementDrag)return;}else if(!this.drag)return;e.preventDefault();e.stopImmediatePropagation();if(this.drag)this.moveDrag(e);},true);
    for(const type of ['pointerup','pointercancel','lostpointercapture'])canvas.addEventListener(type,e=>{if(!this.drag&&!this.placementDrag)return;e.preventDefault();e.stopImmediatePropagation();if(this.placementDrag){if(type==='pointerup')this.previewPlacement(e);this.finishPlacement(type!=='pointerup');}else{if(type==='pointerup')this.moveDrag(e);this.endDrag(type!=='pointerup');}if(canvas.hasPointerCapture(e.pointerId))canvas.releasePointerCapture(e.pointerId);},true);
  }
  pickVolume(ray){return this.group&&ray.intersectObjects(this.group.children.filter(x=>x.isMesh&&x.userData.triggerIndex>=0),false)[0];}
  beginDrag(e){
    if(e.button!==0||!this.editing()||this.placing||this.pickGrid||!['move','resize'].includes(this.tool))return false;
    const s=this.host.state(),ray=this.ray(e),hit=this.gizmo&&ray.intersectObjects(this.gizmo.children.filter(x=>x.isMesh),false)[0];let t=this.project.triggers[this.selected];
    if(!hit){
      if(this.tool!=='move')return false;const body=this.pickVolume(ray);if(!body)return false;
      this.selected=body.object.userData.triggerIndex;t=this.project.triggers[this.selected];this.render();if(t.existing_entity!=null)return true;
      const plane=new THREE.Plane(new THREE.Vector3(0,1,0),-t.center[1]),start=ray.ray.intersectPlane(plane,new THREE.Vector3());if(!start)return true;
      this.drag={kind:'body',plane,start,x:e.clientX,y:e.clientY,center:[...t.center],size:[...t.size],trigger:t,before:this.snapshot(),controlsEnabled:s.controls?.enabled};
    }else{
      if(!t||t.existing_entity!=null)return false;const {axis,sign}=hit.object.userData,r=s.renderer.domElement.getBoundingClientRect(),origin=new THREE.Vector3(...t.center),unit=new THREE.Vector3().setComponent(axis,1);
      const a=origin.clone().project(s.camera),b=origin.clone().addScaledVector(unit,this.handleUnit).project(s.camera),dx=(b.x-a.x)*r.width/2,dy=-(b.y-a.y)*r.height/2,length=dx*dx+dy*dy;
      if(length<64){this.status('Orbit slightly to see this axis from the side.');return true;}
      this.drag={kind:'axis',axis,sign,x:e.clientX,y:e.clientY,dx,dy,length,unit:this.handleUnit,center:[...t.center],size:[...t.size],trigger:t,before:this.snapshot(),controlsEnabled:s.controls?.enabled};
    }
    if(s.controls)s.controls.enabled=false;return true;
  }
  moveDrag(e){
    const d=this.drag;if(!d)return;const t=d.trigger,step=e.shiftKey?1:(this.snap||1);
    if(d.kind==='body'){
      if(Math.hypot(e.clientX-d.x,e.clientY-d.y)<3)return;const point=this.ray(e).ray.intersectPlane(d.plane,new THREE.Vector3());if(!point)return;
      for(const a of [0,2]){const delta=(point.getComponent(a)-d.start.getComponent(a))*(e.shiftKey ? .1 : 1);t.center[a]=Math.max(-100000,Math.min(100000,d.center[a]+Math.round(delta/step)*step));}
    }else{
      const a=d.axis;let delta=((e.clientX-d.x)*d.dx+(e.clientY-d.y)*d.dy)/d.length*d.unit;if(e.shiftKey)delta*=.1;delta=Math.round(delta/step)*step;
      if(d.sign){const size=Math.max(8,Math.min(10000,d.size[a]+delta*d.sign));t.size[a]=size;t.center[a]=d.center[a]+d.sign*(size-d.size[a])/2;}
      else t.center[a]=Math.max(-100000,Math.min(100000,d.center[a]+delta));
    }
    this.dirty=true;this.host.redraw();for(const kind of ['center','size'])for(let a=0;a<3;a++){const input=this.panel?.querySelector(`[data-vector="${kind}"][data-axis="${a}"]`);if(input)input.value=t[kind][a];}
  }
  endDrag(cancel=false){if(!this.drag)return;const d=this.drag,s=this.host.state();if(cancel){d.trigger.center=d.center;d.trigger.size=d.size;}else this.remember(d.before);if(s.controls)s.controls.enabled=d.controlsEnabled;this.drag=null;this.render();}
  clearGizmo(){this.gizmoKey=null;if(!this.gizmo)return;for(const x of this.gizmo.children){x.geometry?.dispose();x.material?.dispose();}this.gizmo.removeFromParent();this.gizmo=null;}
  updateGizmo(){
    const t=this.project?.triggers[this.selected],s=this.host.state();
    if(!this.group.visible||this.placing||this.pickGrid||!['move','resize'].includes(this.tool)||!t||t.existing_entity!=null){this.clearGizmo();return;}
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
        if(!sign){const shaft=new THREE.Mesh(new THREE.CylinderGeometry(unit*.025,unit*.025,unit*.65,8),material.clone());shaft.position.copy(direction).multiplyScalar(unit*.525);shaft.quaternion.copy(rotation);shaft.userData={axis,sign,levelEdit:true};shaft.renderOrder=910;this.gizmo.add(shaft);}
        const line=new THREE.Line(new THREE.BufferGeometry().setFromPoints([new THREE.Vector3(),direction.clone().multiplyScalar(distance)]),new THREE.LineBasicMaterial({color:colors[axis],depthTest:false,depthWrite:false,transparent:true,opacity:.8}));line.renderOrder=909;this.gizmo.add(line);
      }
    }
  }
  pointerClick(e){
    const s=this.host.state();if(!this.active||!this.project||s.mode!=='3d'||s.data?.key!==this.project.source)return false;
    s.camera.updateMatrixWorld();s.scene.updateMatrixWorld(true);const rect=s.renderer.domElement.getBoundingClientRect(),ray=new THREE.Raycaster();ray.setFromCamera(new THREE.Vector2((e.clientX-rect.left)/rect.width*2-1,-(e.clientY-rect.top)/rect.height*2+1),s.camera);
    if(this.pickGrid){const targets=[];s.scene.traverse(x=>{if(x.isMesh&&x.userData.pickIds&&!x.userData.levelEdit){let visible=true;for(let p=x;p;p=p.parent)if(!p.visible)visible=false;if(visible&&x.geometry?.attributes.position)targets.push(x);}});const hit=ray.intersectObjects(targets,false).find(h=>{const material=Array.isArray(h.object.material)?h.object.material[h.face.materialIndex]:h.object.material;return !(material.clippingPlanes||[]).some(p=>p.distanceToPoint(h.point)<0);});if(!hit){this.status('No visible surface here. Try Top view or orbit to expose a floor.',true);return true;}this.gridY=Math.round(hit.point.y*10)/10;this.pickGrid=false;this.render();return true;}
    if(this.placing){if(this.previewPlacement(e))this.commitPlacement();else this.status('Choose Top view to place on an edge-on grid.',true);return true;}
    const hit=this.pickVolume(ray);if(hit)this.select(hit.object.userData.triggerIndex);else{this.selected=-1;this.render();}return true;
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
    const volumes=this.project.triggers.map((t,i)=>({t,i}));if(this.pending&&this.pendingVisible!==false)volumes.push({t:this.pending,i:-1});
    volumes.forEach(({t,i})=>{
      const geometry=new THREE.BoxGeometry(...t.size),material=new THREE.MeshBasicMaterial({color:i===-1?0xe3aa36:i===this.selected?0xff8a22:0x14a5a0,transparent:true,opacity:i===-1 ? .08 : .15,depthTest:false,depthWrite:false,side:THREE.DoubleSide});const mesh=new THREE.Mesh(geometry,material);mesh.position.set(...t.center);mesh.userData={levelEdit:true,triggerIndex:i};mesh.renderOrder=900;this.group.add(mesh);
      const edge=new THREE.LineSegments(new THREE.EdgesGeometry(geometry),new THREE.LineBasicMaterial({color:i===-1?0xe3aa36:i===this.selected?0xff8a22:0x14a5a0,depthTest:false,transparent:true,opacity:.95}));edge.position.copy(mesh.position);edge.userData.levelEdit=true;edge.renderOrder=901;this.group.add(edge);
    });this.dirty=false;this.updateGizmo();
  }
  updateGrid(){const s=this.host.state();if(this.grid&&this.grid.parent!==s.scene){this.grid.geometry.dispose();this.grid.material.dispose();this.grid=null;this.gridKey=null;}const b=s.data?.bounds,step=Math.max(16,this.snap||16),span=b?Math.max(b.maxX-b.minX,b.maxZ-b.minZ)+step*4:2048,divisions=Math.min(1024,Math.ceil(span/step/2)*2),key=[this.project?.source,step,divisions].join(',');if(this.group.visible&&key!==this.gridKey){if(this.grid){this.grid.geometry.dispose();this.grid.material.dispose();this.grid.removeFromParent();}this.grid=new THREE.GridHelper(divisions*step,divisions,0x496c73,0x719c9c);this.grid.material.transparent=true;this.grid.material.opacity=.28;this.grid.material.depthWrite=false;this.grid.material.depthTest=false;this.grid.renderOrder=850;this.grid.position.x=b?Math.round((b.minX+b.maxX)/2/step)*step:0;this.grid.position.z=b?Math.round((b.minZ+b.maxZ)/2/step)*step:0;s.scene.add(this.grid);this.gridKey=key;}if(this.grid){this.grid.visible=this.group.visible;this.grid.position.y=this.gridY||0;}}
  dispose(){this.endDrag(true);this.finishPlacement(true);this.clearGizmo();if(this.grid){this.grid.geometry.dispose();this.grid.material.dispose();this.grid.removeFromParent();this.grid=null;this.gridKey=null;}if(!this.group)return;for(const x of this.group.children){x.geometry?.dispose();x.material?.dispose();}this.group.removeFromParent();this.group=null;}
}
