import * as THREE from 'three';
const escape=s=>String(s??'').replace(/[&<>"']/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
export class LevelEditor {
  constructor(host){
    this.host=host;this.selected=-1;this.active=false;this.placing=false;this.group=null;
    this.panel=document.createElement('section');this.panel.id='levelEditor';this.panel.hidden=true;this.panel.setAttribute('aria-label','Retail level editor');host.view.append(this.panel);
    this.panel.addEventListener('click',e=>this.click(e).catch(err=>this.status(err.message,true)));
    this.panel.addEventListener('change',e=>this.change(e).catch(err=>this.status(err.message,true)));
    document.getElementById('levelEdit').onclick=()=>{document.getElementById('toolsMenu').open=false;this.open().catch(err=>this.status(err.message,true));};
    window.addEventListener('beforeunload',e=>{if(this.project?.triggers.length){e.preventDefault();e.returnValue='';}});
    if(['localhost','127.0.0.1','[::1]'].includes(location.hostname))this.init();
  }
  async api(path,options){const r=await fetch('/api/level-edit/'+path,options);if(!r.ok){let error;try{error=(await r.json()).error;}catch{}throw Error(error||'Local editor server unavailable. Restart the viewer using start.sh.');}return r;}
  async init(){try{this.missions=(await (await this.api('missions')).json()).missions;document.getElementById('levelEdit').hidden=false;}catch{ /* Static hosting has no retail editor backend. */ }}
  async open(){this.active=true;this.panel.hidden=false;if(!this.project)this.render();else{await this.host.loadStage(this.project.source);this.host.setMode('3d');this.render();}this.host.redraw();}
  status(message,error=false){const el=this.panel.querySelector('[data-status]');if(el){el.textContent=message;el.classList.toggle('error',error);}}
  render(){
    const p=this.project,t=p?.triggers[this.selected];
    this.panel.innerHTML=`<div class="le-head"><strong>Level Edit <small>Retail test</small></strong><span><button data-action="collapse" aria-label="Collapse editor">${this.collapsed?'▾':'▴'}</button><button data-action="close" aria-label="Close editor">×</button></span></div><div class="le-body" ${this.collapsed?'hidden':''}>
    <div class="le-row"><label>Rescue mission <select data-field="mission"><option value="">Choose a mission…</option>${(this.missions||[]).map(m=>`<option value="${escape(m.key)}" ${p?.source===m.key?'selected':''}>${escape(m.title)}</option>`).join('')}</select></label><button data-action="load">Load</button></div>
    ${p?`<div class="le-row"><button data-action="add">＋ Add trigger</button><button data-action="existing">Edit existing hint…</button></div><label>Edited triggers <select data-field="selection"><option value="-1">${p.triggers.length?'Select a trigger…':'No edits yet'}</option>${p.triggers.map((x,i)=>`<option value="${i}" ${this.selected===i?'selected':''}>${escape(x.name)}${x.existing_entity!=null?' (existing)':''}</option>`).join('')}</select></label>
    ${this.showExisting?`<div class="le-row"><select data-field="existing"><option value="">Choose an existing volume…</option>${this.info.triggers.map(x=>`<option value="${x.entity}">${escape(x.name)}</option>`).join('')}</select><button data-action="useExisting">Add edit</button></div>`:''}
    ${t?`<fieldset><legend>${t.existing_entity!=null?'Existing hint · fixed box':'Custom hint box'}</legend><label>Name <input data-field="name" value="${escape(t.name)}" maxlength="40" ${t.existing_entity!=null?'readonly':''}></label><label>Message <textarea data-field="message" rows="2" maxlength="240">${escape(t.message)}</textarea></label><label>Display seconds <input data-field="seconds" type="number" min="1" max="30" step="1" value="${t.seconds}"></label>
    ${['center','size'].map(kind=>`<div class="le-coordinates"><span>${kind==='center'?'Center':'Size'}</span>${['X','Y ↑','Z'].map((axis,a)=>`<label>${axis}<input type="number" step="1" ${kind==='size'?'min="8" max="10000"':''} data-vector="${kind}" data-axis="${a}" value="${t[kind][a]}" ${t.existing_entity!=null?'readonly':''}></label>`).join('')}</div>`).join('')}
    <div class="le-row"><button data-action="place" ${t.existing_entity!=null?'disabled':''}>${this.placing?'Cancel placement':'Place on surface'}</button><button data-action="focus">Focus</button><button data-action="delete">Remove</button></div></fieldset>`:''}
    <p class="le-help">Y is up; dimensions use engine units. Click a visible floor to place the bottom of a new box there. Touching the box displays its hint; it does not block movement. Use plain text without parentheses. Existing scripts stay active.</p>
    <div class="le-row"><button data-action="export" ${p.triggers.length?'':'disabled'}>Export game files</button><button data-action="save">Save project</button><button data-action="import">Open project</button></div>`:`<p class="le-help">Choose a rescue mission to make a trigger test for the first Hose slot. Your installed game stays unchanged until you install an export.</p><button data-action="import">Open project</button>`}
    </div><p data-status role="status">${this.placing?'Click a visible surface in the 3D view to place the selected box.':p?'Export replaces the first Hose slot with this rescue test.':'Local editor ready.'}</p>`;
    this.dirty=true;this.host.redraw();
  }
  async load(key){if(!key)throw Error('Choose a rescue mission.');const info=await(await this.api('mission?key='+encodeURIComponent(key))).json();await this.host.loadStage(key);this.host.setMode('3d');this.info=info;this.project={schema:'fdny-level-edit-v1',source:key,source_sha256:info.source_sha256,triggers:[]};this.selected=-1;this.placing=false;this.render();}
  async click(e){const action=e.target.closest('[data-action]')?.dataset.action;if(!action)return;
    const t=this.project?.triggers[this.selected];
    if(action==='close'){this.active=false;this.placing=false;this.panel.hidden=true;this.host.redraw();return;}
    if(action==='collapse'){this.collapsed=!this.collapsed;this.render();return;}
    if(action==='load'){if(this.project?.triggers.length&&!confirm('Discard this draft and load another mission? Save project first to keep it.'))return;return this.load(this.panel.querySelector('[data-field=mission]').value);}
    if(action==='add'){
      if(this.project.triggers.length>=32)throw Error('This proof of concept supports up to 32 trigger edits.');let n=1;while(this.project.triggers.some(x=>x.name==='custom_hint'+n)||this.info.triggers.some(x=>x.name==='custom_hint'+n))n++;
      this.project.triggers.push({name:'custom_hint'+n,center:[...this.info.spawn],size:[120,160,120],message:'Custom trigger works!',seconds:5});this.selected=this.project.triggers.length-1;this.placing=true;this.render();return;
    }
    if(action==='existing'){this.showExisting=!this.showExisting;this.render();return;}
    if(action==='useExisting'){
      const value=this.panel.querySelector('[data-field=existing]').value;if(value==='')throw Error('Choose an existing trigger.');const original=this.info.triggers.find(x=>x.entity===Number(value));
      if(this.project.triggers.some(x=>x.existing_entity===original.entity))throw Error('That trigger is already in this draft.');
      const hint=original.scripts.map(s=>s.match(/^\s*touch:\s*hint\s+([\d.]+)\s+\((.*)\)\s*$/i)).find(Boolean);
      this.project.triggers.push({name:original.name,existing_entity:original.entity,center:[...original.center],size:[...original.size],message:hint?.[2]||'Custom trigger works!',seconds:hint?Number(hint[1]):5});this.selected=this.project.triggers.length-1;this.showExisting=false;this.placing=false;this.render();this.focus();return;
    }
    if(action==='place'){this.placing=!this.placing;this.render();return;}
    if(action==='focus'){this.focus();return;}
    if(action==='delete'){this.project.triggers.splice(this.selected,1);this.selected=this.project.triggers.length-1;this.placing=false;this.render();return;}
    if(action==='save'){this.download(new Blob([JSON.stringify(this.project,null,2)],{type:'application/json'}),'fdny-level-project.json');return;}
    if(action==='import'){
      const input=document.createElement('input');input.type='file';input.accept='.json';input.onchange=async()=>{try{if(input.files[0].size>100000)throw Error('Project file is too large.');const p=JSON.parse(await input.files[0].text());if(p.schema!=='fdny-level-edit-v1'||!Array.isArray(p.triggers)||p.triggers.length>32||p.triggers.some(t=>!t||typeof t.name!=='string'||typeof t.message!=='string'||!Number.isFinite(t.seconds)||t.seconds<1||t.seconds>30||['center','size'].some(k=>!Array.isArray(t[k])||t[k].length!==3||t[k].some(v=>!Number.isFinite(v)||Math.abs(v)>100000))||t.size.some(v=>v<8||v>10000)))throw Error('Not a valid Level Edit project.');if(this.project?.triggers.length&&!confirm('Replace the current draft?'))return;await this.load(p.source);if(p.source_sha256!==this.info.source_sha256)throw Error('Project source map differs from your installed game.');this.project=p;this.selected=p.triggers.length?0:-1;this.render();}catch(err){this.status(err.message,true);}};input.click();return;
    }
    if(action==='export'){
      this.status('Building retail map and installation package…');e.target.disabled=true;
      try{const r=await this.api('export',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(this.project)});this.download(await r.blob(),'fdny-level-edit-'+this.project.source+'.zip');this.status('Export downloaded. Unzip and read README.txt for install and restore commands.');}finally{e.target.disabled=false;}
    }
  }
  async change(e){const t=this.project?.triggers[this.selected],el=e.target;
    if(el.dataset.field==='selection'){this.selected=Number(el.value);this.placing=false;this.render();return;}
    if(!t)return;
    if(el.dataset.vector){const value=Number(el.value);if(!Number.isFinite(value)||Math.abs(value)>100000||(el.dataset.vector==='size'&&(value<8||value>10000))){el.value=t[el.dataset.vector][Number(el.dataset.axis)];this.status('Use finite coordinates within 100000 and sizes between 8 and 10000.',true);return;}t[el.dataset.vector][Number(el.dataset.axis)]=value;this.dirty=true;this.host.redraw();}
    else if(['name','message','seconds'].includes(el.dataset.field)){t[el.dataset.field]=el.dataset.field==='seconds'?Number(el.value):el.value;this.dirty=true;this.host.redraw();if(el.dataset.field==='name'){const option=this.panel.querySelector('[data-field=selection]')?.selectedOptions[0];if(option)option.textContent=t.name;}}
  }
  focus(){const t=this.project?.triggers[this.selected];if(!t)return;this.host.focusBounds({min:t.center.map((v,a)=>v-t.size[a]/2),max:t.center.map((v,a)=>v+t.size[a]/2)});}
  download(blob,name){const url=URL.createObjectURL(blob),a=document.createElement('a');a.href=url;a.download=name;a.click();setTimeout(()=>URL.revokeObjectURL(url),10000);}
  pointerClick(e){
    const s=this.host.state();if(!this.active||!this.project||s.mode!=='3d'||s.data?.key!==this.project.source)return false;
    s.camera.updateMatrixWorld();s.scene.updateMatrixWorld(true);const rect=s.renderer.domElement.getBoundingClientRect(),ray=new THREE.Raycaster();ray.setFromCamera(new THREE.Vector2((e.clientX-rect.left)/rect.width*2-1,-(e.clientY-rect.top)/rect.height*2+1),s.camera);
    if(this.placing){const t=this.project.triggers[this.selected];if(!t)return false;const targets=[];s.scene.traverse(x=>{if(x.isMesh&&x.userData.pickIds&&!x.userData.levelEdit){let visible=true;for(let p=x;p;p=p.parent)if(!p.visible)visible=false;if(visible&&x.geometry?.attributes.position)targets.push(x);}});const hit=ray.intersectObjects(targets,false).find(h=>{const material=Array.isArray(h.object.material)?h.object.material[h.face.materialIndex]:h.object.material;return !(material.clippingPlanes||[]).some(p=>p.distanceToPoint(h.point)<0);});if(!hit){this.status('No surface under the pointer. Rotate the view and try again.',true);return true;}t.center=[hit.point.x,hit.point.y+t.size[1]/2,hit.point.z].map(v=>Math.round(v*10)/10);this.placing=false;this.render();return true;}
    const hit=this.group&&ray.intersectObjects(this.group.children,false).find(x=>x.object.userData.triggerIndex!=null);if(hit){this.selected=hit.object.userData.triggerIndex;this.render();return true;}return false;
  }
  update3D(){
    const s=this.host.state();if(!s.scene)return;
    if(this.group&&this.group.parent!==s.scene){this.dispose();}
    if(!this.group){this.group=new THREE.Group();s.scene.add(this.group);this.dirty=true;}
    this.group.visible=this.active&&s.data?.key===this.project?.source;
    if(!this.dirty||!this.project)return;
    for(const x of [...this.group.children]){x.geometry?.dispose();x.material?.dispose();this.group.remove(x);}
    this.project.triggers.forEach((t,i)=>{
      const geometry=new THREE.BoxGeometry(...t.size),material=new THREE.MeshBasicMaterial({color:i===this.selected?0xff8a22:0x14a5a0,transparent:true,opacity:.15,depthTest:false,depthWrite:false,side:THREE.DoubleSide});const mesh=new THREE.Mesh(geometry,material);mesh.position.set(...t.center);mesh.userData={levelEdit:true,triggerIndex:i};mesh.renderOrder=900;this.group.add(mesh);
      const edge=new THREE.LineSegments(new THREE.EdgesGeometry(geometry),new THREE.LineBasicMaterial({color:i===this.selected?0xff8a22:0x14a5a0,depthTest:false,transparent:true,opacity:.95}));edge.position.copy(mesh.position);edge.userData.levelEdit=true;edge.renderOrder=901;this.group.add(edge);
    });this.dirty=false;
  }
  dispose(){if(!this.group)return;for(const x of this.group.children){x.geometry?.dispose();x.material?.dispose();}this.group.removeFromParent();this.group=null;}
}
