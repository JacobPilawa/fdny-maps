let runtime;
async function bytes(path){const r=await fetch(new URL(path,import.meta.url));if(!r.ok)throw Error('Cannot load export file: '+path);return new Uint8Array(await r.arrayBuffer());}
export async function initialize(loadRuntime,indexURL='https://cdn.jsdelivr.net/pyodide/v0.27.7/full/'){
  const load=loadRuntime||(await import(indexURL+'pyodide.mjs')).loadPyodide;
  const py=await load({indexURL});
  for(const folder of ['/exporter','/exporter/livesplit'])py.FS.mkdirTree(folder);
  for(const path of ['level_edit.py','livesplit/custom_triggers.py','livesplit/FDNY_ILs.asl'])py.FS.writeFile('/exporter/'+path,await bytes(path));
  py.runPython("import sys; sys.path.insert(0, '/exporter')\nimport json, zipfile, shutil\nfrom pathlib import Path\nfrom level_edit import export_project");
  return py;
}
// Also exported for the CPU-only WASM regression check.
export async function buildPackage(py,project,input){
  py.FS.writeFile('/input.zip',input);
  py.globals.set('project_json',JSON.stringify(project));
  py.runPython(`
game = Path('/game')
if game.exists(): shutil.rmtree(game)
with zipfile.ZipFile('/input.zip') as archive: archive.extractall(game)
Path('/output.zip').write_bytes(export_project(game, json.loads(project_json)))
`);
  const zip=py.FS.readFile('/output.zip');
  py.runPython("shutil.rmtree(game)\nPath('/input.zip').unlink()\nPath('/output.zip').unlink()");
  return zip;
}
self.onmessage=async({data:{id,project}})=>{
  try{
    if(!/^[a-z0-9_]+$/.test(project.source))throw Error('Invalid rescue mission.');
    self.postMessage({id,progress:runtime?'Building package…':'Loading export tools (first export only)…'});
    if(!runtime)runtime=initialize().catch(error=>{runtime=null;throw error;});
    const py=await runtime;
    self.postMessage({id,progress:'Loading mission source and building package…'});
    const zip=await buildPackage(py,project,await bytes('data/editor/'+project.source+'.zip'));
    self.postMessage({id,zip},[zip.buffer]);
  }catch(error){self.postMessage({id,error:error.message||String(error)});}
};
