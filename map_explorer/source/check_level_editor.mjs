// CPU-side editor gestures and project edits; no browser or game interaction.
import assert from 'node:assert/strict';
import fs from 'node:fs';
import {pathToFileURL,fileURLToPath} from 'node:url';
const root=fileURLToPath(new URL('../',import.meta.url));
const url=pathToFileURL(root+'vendor/three.module.min.js').href;
const THREE=await import(url);
const source=fs.readFileSync(root+'level-editor.js','utf8').replace("from 'three'",`from '${url}'`);
const {LevelEditor}=await import('data:text/javascript;base64,'+Buffer.from(source).toString('base64'));
const copy=value=>JSON.parse(JSON.stringify(value));
const volume=(name='volume1')=>({name,center:[0,60,0],size:[120,120,120],message:'Test',seconds:5,checkpoint:true,split_label:name});
const event=(x=500,y=500,extra={})=>({button:0,buttons:0,pointerId:1,clientX:x,clientY:y,preventDefault(){},stopImmediatePropagation(){},...extra});
function setup(triggers=[volume()]){
  const scene=new THREE.Scene(),camera=new THREE.PerspectiveCamera(50,1,1,10000);camera.position.set(400,400,600);camera.lookAt(0,60,0);camera.updateMatrixWorld();
  const floor=new THREE.Mesh(new THREE.BoxGeometry(2000,10,2000),new THREE.MeshBasicMaterial());floor.position.y=-5;floor.userData.pickIds=[];scene.add(floor);
  const controls={enabled:true,target:new THREE.Vector3(0,60,0),update(){camera.lookAt(this.target);camera.updateMatrixWorld();}};
  const state={scene,camera,controls,mode:'3d',data:{key:'training'},renderer:{domElement:{getBoundingClientRect:()=>({left:0,top:0,width:1000,height:1000})}}};
  const editor=Object.create(LevelEditor.prototype);
  Object.assign(editor,{host:{state:()=>state,redraw(){},focusBounds(b){controls.target.fromArray(b.min.map((v,a)=>(v+b.max[a])/2));camera.position.copy(controls.target).add(new THREE.Vector3(400,400,600));controls.update();}},panel:{querySelector:()=>null},active:true,selected:triggers.length?0:-1,placing:false,gridY:0,snap:16,tool:'select',tab:'volumes',undoStack:[],redoStack:[],info:{spawn:[0,30,0],triggers:[],stage_finishes:['training']},project:{schema:'fdny-level-edit-v1',source:'training',source_sha256:'test',triggers:copy(triggers)},group:null,dirty:true,render(){this.renderCount=(this.renderCount||0)+1;this.dirty=true;},status(message,error){this.lastStatus={message,error};}});
  const handlers={},canvas={addEventListener(type,fn,capture){assert.equal(capture,true);handlers[type]=fn;},setPointerCapture(){},hasPointerCapture(){return true;},releasePointerCapture(){}};editor.bindCanvas(canvas);
  const screen=point=>{camera.updateMatrixWorld();const p=new THREE.Vector3(...point).project(camera);return event((p.x+1)*500,(1-p.y)*500);};
  const top=()=>{camera.position.set(0,800,.01);camera.lookAt(0,0,0);camera.updateMatrixWorld();};
  return {editor,state,scene,camera,controls,handlers,screen,top};
}
async function check(name,fn){const ctx=setup();try{await fn(ctx);console.log('PASS: '+name);}finally{ctx.editor.dispose();assert.equal(ctx.scene.children.length,1);}}
function handleEvent(ctx,axis,sign){const e=ctx.editor;e.update3D();ctx.scene.updateMatrixWorld(true);const h=e.gizmo.children.find(x=>x.isMesh&&x.userData.axis===axis&&x.userData.sign===sign);return ctx.screen(h.getWorldPosition(new THREE.Vector3()).toArray());}
try{
  await check('new-volume preview never mutates placed volumes; click commits once; navigation cannot move it',({editor:e,handlers:h,top})=>{
    top();const original=copy(e.project.triggers);e.newVolume();assert.equal(e.project.triggers.length,1);h.pointermove(event(650,650));assert(e.pendingVisible);assert.deepEqual(e.project.triggers,original);
    const pending=copy(e.pending);h.pointermove(event(900,900,{buttons:2}));assert.deepEqual(e.pending,pending);
    h.pointerdown(event(650,650));h.pointerup(event(650,650));assert.equal(e.project.triggers.length,2);assert.equal(e.pending,null);assert.equal(e.placing,false);assert.equal(e.undoStack.length,1);
    const committed=copy(e.project);h.pointermove(event(10000,10000));assert.deepEqual(e.project,committed);
    e.newVolume();h.pointermove(event(800,800));e.history();assert.equal(e.pending,null);assert.deepEqual(e.project,committed);assert.equal(e.undoStack.length,1);
  });
  await check('footprint drawing, fixed grid height, cancel and delete during preview preserve existing volumes',async({editor:e,handlers:h,controls,top})=>{
    top();e.gridY=48;e.selected=-1;e.newVolume();e.gridY=48;const before=copy(e.project);h.pointerdown(event(500,500));assert.equal(controls.enabled,false);const anchor=e.placementDrag.start.clone();
    h.pointermove(event(650,650));assert.equal(Math.abs(e.pending.center[0]-anchor.x),e.pending.size[0]/2);assert.equal(e.pending.center[1]-e.pending.size[1]/2,48);
    h.pointercancel(event(650,650));assert.equal(controls.enabled,true);assert.deepEqual(e.project,before);assert.equal(e.undoStack.length,0);
    await e.command('cancel');assert.equal(e.pending,null);assert.deepEqual(e.project,before);
    e.select(0);e.newVolume();await e.command('delete');assert.equal(e.pending,null);assert.deepEqual(e.project,before);
    e.selected=-1;e.newVolume();h.pointerdown(event(500,500));h.pointermove(event(650,650));h.pointerup(event(650,650));assert.equal(e.project.triggers.length,2);assert.equal(e.undoStack.length,1);
    e.history();assert.deepEqual(e.project,before);e.history(true);assert.equal(e.project.triggers.length,2);
  });
  await check('selection, deselection and empty deletion never remove volumes; selected volumes remain editable',async({editor:e,screen})=>{
    e.selected=-1;e.update3D();assert(e.pointerClick(screen([0,60,0])));assert.equal(e.selected,0);assert(!e.gizmo);
    e.pointerClick(event(-2000,-2000));assert.equal(e.selected,-1);await e.command('delete');assert.equal(e.project.triggers.length,1);
    e.select(0);await e.command('move');e.update3D();assert(e.gizmo);assert.equal(e.undoStack.length,0);
    await e.command('selectTool');e.update3D();assert(!e.gizmo);assert.equal(e.project.triggers.length,1);
    for(const tool of ['select','move','resize']){e.tool=tool;e.select(0);e.update3D();e.pointerClick(event(-2000,-2000));assert.equal(e.selected,-1);e.update3D();assert(!e.gizmo);e.pointerClick(screen([0,60,0]));assert.equal(e.selected,0);assert.equal(e.tool,tool);assert.equal(e.project.triggers.length,1);assert.equal(e.undoStack.length,0);}
  });
  await check('axis movement/resize snap, fixed opposite face, limits, cancel and single-step undo',async ctx=>{
    const {editor:e,controls}=ctx;e.tool='move';e.snap=8;let point=handleEvent(ctx,0,0),before=copy(e.project);assert(e.beginDrag(point));assert.equal(controls.enabled,false);let d=e.drag;
    e.moveDrag({...point,clientX:point.clientX+d.dx*32/d.unit,clientY:point.clientY+d.dy*32/d.unit});assert.equal(e.project.triggers[0].center[0],32);e.endDrag(true);assert.deepEqual(e.project,before);assert.equal(e.undoStack.length,0);
    e.tool='resize';point=handleEvent(ctx,0,1);const fixed=e.project.triggers[0].center[0]-e.project.triggers[0].size[0]/2;assert(e.beginDrag(point));d=e.drag;
    e.moveDrag({...point,clientX:point.clientX+d.dx*24/d.unit,clientY:point.clientY+d.dy*24/d.unit});assert.equal(e.project.triggers[0].size[0],144);assert.equal(e.project.triggers[0].center[0]-e.project.triggers[0].size[0]/2,fixed);e.endDrag();assert.equal(e.undoStack.length,1);assert.equal(controls.enabled,true);
    e.history();assert.deepEqual(e.project,before);e.history(true);assert.equal(e.project.triggers[0].size[0],144);
    point=handleEvent(ctx,0,-1);assert(e.beginDrag(point));d=e.drag;e.moveDrag({...point,clientX:point.clientX+d.dx*20000/d.unit,clientY:point.clientY+d.dy*20000/d.unit});assert.equal(e.project.triggers[0].size[0],8);e.endDrag(true);assert.equal(e.project.triggers[0].size[0],144);assert.equal(e.undoStack.length,1);
  });
  await check('Move can drag a box body in X/Z without altering height; a click does not enter history',({editor:e,screen,controls})=>{
    e.tool='move';e.update3D();const point=screen([0,60,0]);assert(e.beginDrag(point));assert.equal(e.drag.kind,'body');e.endDrag();assert.equal(e.undoStack.length,0);
    const before=copy(e.project);assert(e.beginDrag(point));e.moveDrag({...point,clientX:point.clientX+90,clientY:point.clientY+50});assert.notDeepEqual(e.project,before);assert.equal(e.project.triggers[0].center[1],60);e.endDrag();assert.equal(e.undoStack.length,1);assert.equal(controls.enabled,true);e.history();assert.deepEqual(e.project,before);
  });
  await check('copy/paste/duplicate use independent geometry, unique IDs/labels and undoable deletion',async({editor:e})=>{
    const original=copy(e.project.triggers[0]);await e.command('copy');assert.equal(e.undoStack.length,0);await e.command('paste');assert.equal(e.project.triggers.length,2);
    let pasted=e.project.triggers[1];assert.notEqual(pasted.name,original.name);assert.notEqual(pasted.split_label,original.split_label);assert.match(pasted.name,/^[A-Za-z][A-Za-z0-9_]{0,39}$/);assert.deepEqual(e.project.triggers[0],original);assert.notEqual(pasted.size,e.project.triggers[0].size);
    e.history();assert.equal(e.project.triggers.length,1);e.history(true);assert.equal(e.project.triggers.length,2);
    const clipboard=copy(e.clipboard);await e.command('duplicate');assert.deepEqual(e.clipboard,clipboard);assert.equal(e.project.triggers.length,3);assert.equal(new Set(e.project.triggers.map(t=>t.name)).size,3);
    const before=copy(e.project);await e.command('delete');assert.equal(e.project.triggers.length,2);e.history();assert.deepEqual(e.project,before);e.history(true);assert.equal(e.project.triggers.length,2);
    e.history();await e.command('duplicate');assert.equal(e.redoStack.length,0);
  });
  await check('undo and paste reveal the selected volume if navigation left it out of view',async({editor:e,controls})=>{
    e.edit(()=>{e.project.triggers[0].center[0]=6000;});e.focus();assert.equal(controls.target.x,6000);e.history();assert.equal(controls.target.x,0);
    await e.command('copy');controls.target.x=10000;e.host.state().camera.position.x=10400;controls.update();await e.command('paste');assert.equal(controls.target.x,e.project.triggers[e.selected].center[0]);
  });
  await check('authored volume copies become editable boxes and the volume limit is enforced',async({editor:e})=>{
    e.project.triggers[0].existing_entity=5;e.project.triggers[0].name='Authored hint';await e.command('duplicate');assert.equal(e.project.triggers[0].existing_entity,5);assert.equal(e.project.triggers[1].existing_entity,undefined);assert.match(e.project.triggers[1].name,/^[A-Za-z][A-Za-z0-9_]{0,39}$/);
    while(e.project.triggers.length<32)e.project.triggers.push(volume('other'+e.project.triggers.length));const before=copy(e.project);e.newVolume();await e.command('paste');assert.deepEqual(e.project,before);assert(!e.pending);
  });
  await check('property edits and split ordering undo correctly; same-mission Load preserves the project',async({editor:e})=>{
    e.project.triggers=[volume('First <door>'),{...volume('hint_only'),checkpoint:false},volume('Second')];e.info.stage_finishes=['boat_a','boat_b'];e.selected=0;
    let summary=e.splitSummary();assert(summary.indexOf('First &lt;door&gt;')<summary.indexOf('Second'));assert(summary.indexOf('Second')<summary.indexOf('boat_a complete'));assert(summary.indexOf('boat_a complete')<summary.indexOf('boat_b complete'));assert(!summary.includes('hint_only'));
    const before=copy(e.project);await e.command('later');assert.equal(e.selected,2);e.history();assert.deepEqual(e.project,before);
    const renders=e.renderCount;await e.change({target:{dataset:{field:'split_label'},value:'Renamed'}});assert.equal(e.renderCount,renders);assert.equal(e.project.triggers[0].split_label,'Renamed');e.history();assert.deepEqual(e.project,before);
    e.panel.querySelector=selector=>selector==='[data-field=mission]'?{value:'training'}:null;await e.command('load');assert.deepEqual(e.project,before);
  });
  await check('a failed project import leaves the current draft and its undo history intact',async({editor:e})=>{
    e.edit(()=>{e.project.triggers[0].split_label='Keep this draft';});const before=copy(e.project),history=copy(e.undoStack);
    e.api=async()=>({json:async()=>({source_sha256:'actual',spawn:[0,30,0],triggers:[]})});e.host.loadStage=async()=>{throw Error('Should not load a mismatched map');};
    await assert.rejects(e.load('training',{...before,source_sha256:'different'}),/differs/);assert.deepEqual(e.project,before);assert.deepEqual(e.undoStack,history);
  });
  await check('keyboard shortcuts respect text inputs, avoid viewer hotkeys, and undo/cancel safely',async({editor:e})=>{
    function key(key,extra={}){let handled=false;e.keyDown({key,target:{tagName:'CANVAS'},preventDefault(){handled=true;},stopImmediatePropagation(){},...extra});return handled;}
    assert.equal(key('Delete',{target:{tagName:'INPUT'}}),false);assert.equal(e.project.triggers.length,1);
    assert(key('c',{ctrlKey:true}));assert(key('v',{ctrlKey:true}));assert.equal(e.project.triggers.length,2);assert(key('z',{ctrlKey:true}));assert.equal(e.project.triggers.length,1);assert(key('z',{ctrlKey:true,shiftKey:true}));assert.equal(e.project.triggers.length,2);
    assert(key('n'));assert(e.pending);assert(key('Escape'));assert.equal(e.pending,null);assert.equal(e.project.triggers.length,2);assert.equal(e.tool,'select');
    const htmlRender=LevelEditor.prototype.render;e.panel={querySelector:()=>null,innerHTML:''};htmlRender.call(e);for(const text of ['Select','New volume','Copy','Paste','Duplicate','Delete','Undo','Redo','Placed volumes','Split order'])assert(e.panel.innerHTML.includes(text));
  });
  await check('flight keys pass through; entering the current mission preserves its camera',async({editor:e,state,camera})=>{
    let handled=false;
    const key=k=>{handled=false;e.keyDown({key:k,target:{tagName:'CANVAS'},preventDefault(){handled=true;},stopImmediatePropagation(){}});return handled;};
    assert.equal(key('f'),false);
    state.flying=true;e.tool='select';for(const k of ['w','a','s','d','q','e','Shift','ArrowUp','Escape'])assert.equal(key(k),false);assert.equal(e.tool,'select');
    assert(key('.'));assert(key('n'));assert(e.pending);e.cancelInteraction();
    state.flying=false;assert(key('s'));assert.equal(e.tool,'resize');
    const position=camera.position.clone(),target=state.controls.target.clone();let roof=false;
    e.host.loadStage=()=>{throw Error('Current stage should not reload');};e.host.setMode=()=>{throw Error('Current 3D mode should not reset');};e.host.cutRoof=()=>{roof=true;};
    e.browserExport=true;e.missions=[{key:'training',info:{source_sha256:'test',spawn:[0,30,0],triggers:[],stage_finishes:['training']}}];
    await e.load('training');assert(roof);assert(camera.position.equals(position));assert(state.controls.target.equals(target));
  });
  await check('map mismatch/closing hide overlays and release geometry',({editor:e,state})=>{
    e.tool='move';e.update3D();assert(e.group.visible);state.data.key='bank';e.update3D();assert.equal(e.group.visible,false);assert.equal(e.pointerClick(event()),false);state.data.key='training';e.active=false;e.update3D();assert.equal(e.group.visible,false);
  });
}catch(error){console.error((error.stack||String(error)).replace(/data:text\/javascript;base64,[A-Za-z0-9+/=]+/g,'level-editor.js'));process.exitCode=1;}
