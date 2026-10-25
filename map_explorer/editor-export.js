// Run the existing exporter off the UI thread. No installed Python or game needed.
let worker, pending, nextId=0;
export function exportPackage(project,onProgress=()=>{}) {
  if(pending)return Promise.reject(Error('An export is already running.'));
  if(!worker){
    worker=new Worker(new URL('./editor-worker.js',import.meta.url),{type:'module'});
    worker.onmessage=({data})=>{
      if(!pending||pending.id!==data.id)return;
      if(data.progress){pending.progress(data.progress);return;}
      const request=pending;pending=null;
      if(data.error)request.reject(Error(data.error));else request.resolve(new Blob([data.zip],{type:'application/zip'}));
    };
    worker.onerror=()=>{const request=pending;pending=null;worker.terminate();worker=null;request?.reject(Error('Export could not start. Check your connection and try again.'));};
  }
  return new Promise((resolve,reject)=>{pending={id:++nextId,resolve,reject,progress:onProgress};worker.postMessage({id:nextId,project});});
}
