// CPU-only: run the production browser exporter in WASM and compare every ZIP
// entry against the native Python exporter, using only the published inputs.
// Usage: node source/check_browser_export.mjs /path/to/node_modules/pyodide
import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import {fileURLToPath,pathToFileURL} from 'node:url';
import {spawnSync} from 'node:child_process';
const root=fileURLToPath(new URL('../',import.meta.url));
const runtime=process.argv[2];
assert(runtime,'Pass the directory containing pyodide.mjs (version 0.27.7).');
globalThis.self={};
globalThis.fetch=async url=>({ok:true,arrayBuffer:async()=>new Uint8Array(await fs.readFile(fileURLToPath(url)))});
const {loadPyodide}=await import(pathToFileURL(runtime+'/pyodide.mjs'));
const {initialize,buildPackage}=await import(pathToFileURL(root+'editor-worker.js'));
const py=await initialize(loadPyodide,runtime+'/');
const missions=JSON.parse(await fs.readFile(root+'data/editor/missions.json','utf8'));
for(const mission of missions){
  const project={schema:'fdny-level-edit-v1',source:mission.key,source_sha256:mission.info.source_sha256,triggers:[{name:'BrowserTest',center:mission.info.spawn,size:[120,160,80],message:'Checkpoint reached!',seconds:5,checkpoint:true,split_label:'Browser test'}]};
  const zip=await buildPackage(py,project,new Uint8Array(await fs.readFile(root+'data/editor/'+mission.key+'.zip')));
  py.FS.writeFile('/check.zip',zip);
  const actual=JSON.parse(py.runPython("import hashlib\nwith zipfile.ZipFile('/check.zip') as z: hashes = {n:hashlib.sha256(z.read(n)).hexdigest() for n in z.namelist()}\njson.dumps(hashes)"));
  const expected=spawnSync('python3',['-c',`
import hashlib,io,json,sys,tempfile,zipfile
from pathlib import Path
sys.path.insert(0,'.')
from level_edit import export_project
p=json.load(sys.stdin)
with tempfile.TemporaryDirectory() as d:
    with zipfile.ZipFile('data/editor/'+p['source']+'.zip') as z: z.extractall(d)
    with zipfile.ZipFile(io.BytesIO(export_project(Path(d),p))) as z: print(json.dumps({n:hashlib.sha256(z.read(n)).hexdigest() for n in z.namelist()}))
`],{cwd:root,input:JSON.stringify(project),encoding:'utf8'});
  assert.equal(expected.status,0,expected.stderr);assert.deepEqual(actual,JSON.parse(expected.stdout));
  console.log('PASS: '+mission.key+' WASM package matches native export ('+Object.keys(actual).length+' files)');
}
const bad={schema:'fdny-level-edit-v1',source:'training',source_sha256:'wrong',triggers:[{name:'Bad',center:[0,0,0],size:[120,120,120],message:'Test',seconds:5}]};
await assert.rejects(buildPackage(py,bad,new Uint8Array(await fs.readFile(root+'data/editor/training.zip'))),/Source map changed/);
console.log('PASS: browser export rejects mismatched source maps');
