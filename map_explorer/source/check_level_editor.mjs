// CPU-side editor geometry/placement checks; no browser or game interaction.
import assert from 'node:assert/strict';
import fs from 'node:fs';
import {pathToFileURL,fileURLToPath} from 'node:url';
const root=fileURLToPath(new URL('../',import.meta.url));
const url=pathToFileURL(root+'vendor/three.module.min.js').href;
const THREE=await import(url);
const source=fs.readFileSync(root+'level-editor.js','utf8').replace("from 'three'",`from '${url}'`);
const {LevelEditor}=await import('data:text/javascript;base64,'+Buffer.from(source).toString('base64'));
const editor=Object.create(LevelEditor.prototype);
const scene=new THREE.Scene(),camera=new THREE.PerspectiveCamera(50,1,1,10000);camera.position.set(0,800,1);camera.lookAt(0,0,0);camera.updateMatrixWorld();
const floor=new THREE.Mesh(new THREE.BoxGeometry(1000,10,1000),new THREE.MeshBasicMaterial());floor.position.y=-5;floor.userData.pickIds=[];scene.add(floor);scene.updateMatrixWorld(true);
const state={scene,camera,mode:'3d',data:{key:'training'},renderer:{domElement:{getBoundingClientRect:()=>({left:0,top:0,width:1000,height:1000})}}};
const trigger={name:'custom_hint1',center:[0,0,0],size:[120,160,120],message:'Test',seconds:5};
Object.assign(editor,{host:{state:()=>state,redraw:()=>{}},active:true,selected:0,placing:true,gridY:0,snap:16,project:{source:'training',triggers:[trigger]},group:null,dirty:true,render(){this.dirty=true;},status(message){throw Error(message);}});
editor.update3D();assert.equal(editor.group.children.length,2);assert.deepEqual(editor.group.children[0].geometry.parameters,{width:120,height:160,depth:120,widthSegments:1,heightSegments:1,depthSegments:1});
assert(editor.pointerClick({clientX:500,clientY:500}));assert.equal(trigger.center[1],80);assert.equal(editor.placing,false);assert(editor.grid.visible);assert.equal(editor.grid.position.y,0);
editor.update3D();assert.deepEqual(editor.group.children[0].position.toArray(),trigger.center);
trigger.size=[60,55,60];editor.dirty=true;editor.update3D();assert.equal(editor.group.children[0].geometry.parameters.height,55);
// Pick a real projected handle, drag one world axis, and cancel/commit a face resize.
camera.position.set(400,400,600);camera.lookAt(...trigger.center);camera.updateMatrixWorld();state.controls={enabled:true};editor.tool='move';editor.snap=8;editor.update3D();scene.updateMatrixWorld(true);
function handleEvent(axis,sign){const h=editor.gizmo.children.find(x=>x.isMesh&&x.userData.axis===axis&&x.userData.sign===sign),p=h.getWorldPosition(new THREE.Vector3()).project(camera);return {button:0,clientX:(p.x+1)*500,clientY:(1-p.y)*500};}
let e=handleEvent(0,0),beforeCenter=[...trigger.center];assert(editor.beginDrag(e));assert.equal(state.controls.enabled,false);
let d=editor.drag;editor.moveDrag({...e,clientX:e.clientX+d.dx*32/d.unit,clientY:e.clientY+d.dy*32/d.unit});assert.equal(trigger.center[0],beforeCenter[0]+32);assert.equal(trigger.center[1],beforeCenter[1]);editor.endDrag(true);assert.deepEqual(trigger.center,beforeCenter);assert.equal(state.controls.enabled,true);
editor.tool='resize';editor.update3D();scene.updateMatrixWorld(true);e=handleEvent(0,1);const oldSize=[...trigger.size],fixedFace=trigger.center[0]-trigger.size[0]/2;assert(editor.beginDrag(e));d=editor.drag;
editor.moveDrag({...e,clientX:e.clientX+d.dx*24/d.unit,clientY:e.clientY+d.dy*24/d.unit});assert.equal(trigger.size[0],oldSize[0]+24);assert.equal(trigger.center[0]-trigger.size[0]/2,fixedFace);assert.equal(trigger.size[1],oldSize[1]);editor.endDrag();
editor.update3D();scene.updateMatrixWorld(true);e=handleEvent(0,-1);assert(editor.beginDrag(e));d=editor.drag;editor.moveDrag({...e,clientX:e.clientX+d.dx*20000/d.unit,clientY:e.clientY+d.dy*20000/d.unit});assert.equal(trigger.size[0],8);editor.endDrag(true);
// Placement uses a grid plane even where no rendered floor exists. Capture handlers
// claim drawing gestures before OrbitControls, then restore navigation on release.
camera.position.set(0,1000,.01);camera.lookAt(0,0,0);camera.updateMatrixWorld();editor.gridY=48;editor.snap=16;editor.placing=true;editor.update3D();assert.equal(editor.grid.position.y,48);
const handlers={},canvas={addEventListener(type,fn,capture){assert.equal(capture,true);handlers[type]=fn;},setPointerCapture(){},hasPointerCapture(){return true;},releasePointerCapture(){}};editor.bindCanvas(canvas);
const event=(x,y)=>({button:0,pointerId:1,clientX:x,clientY:y,preventDefault(){},stopImmediatePropagation(){}});
handlers.pointermove(event(800,500));assert.equal(trigger.center[0]%16,0);assert.equal(trigger.center[1]-trigger.size[1]/2,48);
handlers.pointerdown(event(500,500));assert.equal(state.controls.enabled,false);const anchor=editor.placementDrag.start.clone();handlers.pointermove(event(650,650));assert(trigger.size[0]>=8&&trigger.size[2]>=8);assert.equal(Math.abs(trigger.center[0]-anchor.x),trigger.size[0]/2);handlers.pointerup(event(650,650));assert.equal(editor.placing,false);assert.equal(editor.tool,'move');assert.equal(state.controls.enabled,true);
editor.placing=true;const savedCenter=[...trigger.center],savedSize=[...trigger.size];handlers.pointerdown(event(400,400));handlers.pointermove(event(700,700));handlers.pointercancel(event(700,700));assert.deepEqual(trigger.center,savedCenter);assert.deepEqual(trigger.size,savedSize);assert.equal(state.controls.enabled,true);editor.placing=false;
state.data.key='bank';editor.update3D();assert.equal(editor.group.visible,false);assert.equal(editor.pointerClick({clientX:500,clientY:500}),false);
state.data.key='training';editor.active=false;editor.update3D();assert.equal(editor.group.visible,false);
editor.dispose();assert.equal(scene.children.length,1);
// Overview follows checkpoint order, excludes hint-only volumes and retains stock finishes.
const first={...trigger,name:'first',split_label:'First <door>',checkpoint:true},hint={...trigger,name:'hint_only',checkpoint:false},last={...trigger,name:'last',split_label:'Second',checkpoint:true};
editor.project={source:'boat_a',triggers:[first,hint,last]};editor.info={stage_finishes:['boat_a','boat_b']};editor.selected=0;
let summary=editor.splitSummary();assert(summary.indexOf('First &lt;door&gt;')<summary.indexOf('Second'));assert(summary.indexOf('Second')<summary.indexOf('boat_a complete'));assert(summary.indexOf('boat_a complete')<summary.indexOf('boat_b complete'));assert(!summary.includes('hint_only'));
await editor.click({target:{closest:()=>({dataset:{action:'later'}})}});summary=editor.splitSummary();assert(summary.indexOf('Second')<summary.indexOf('First &lt;door&gt;'));assert.equal(editor.selected,2);
console.log('PASS: grid preview, height, footprint drawing/cancel, pointer capture, axis drag, snapping, fixed face, limits, navigation restore, map mismatch and disposal');
console.log('PASS: split overview, escaped labels, reordering, hint exclusion and default stage finishes');
