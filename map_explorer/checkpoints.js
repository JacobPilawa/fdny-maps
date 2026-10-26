// One plain-text file, read automatically by FDNYCore for the named mission.
const missions=new Set(['training','bank','bar_r','gas_r','mbase','boat_a','boat_b','subway_a','subway_b','oil_rig_a','oil_rig_b','oil_rig_c','oil_rig_d']);
export function checkpointFile(project){
  if(!project||!missions.has(project.source)||!Array.isArray(project.triggers)||project.triggers.length<1||project.triggers.length>32)throw Error('Add between 1 and 32 checkpoints to a rescue mission.');
  const rows=project.triggers.map((t,index)=>{
    const label=(t.label??t.split_label??t.name??`Checkpoint ${index+1}`).trim();
    if(!label||label.length>80||/[^\x20-\x7e]|[()|]/.test(label))throw Error('Checkpoint labels need 1–80 plain text characters, without parentheses or |.');
    if(['center','size'].some(key=>!Array.isArray(t[key])||t[key].length!==3||t[key].some(v=>!Number.isFinite(v)||Math.abs(v)>100000))||t.size.some(v=>v<8||v>10000))throw Error('Use valid positions and checkpoint dimensions between 8 and 10000.');
    const bounds=[...t.center.map((v,a)=>v-t.size[a]/2),...t.center.map((v,a)=>v+t.size[a]/2)].map(v=>Number(v.toFixed(4)));
    return [...bounds,label].join('|');
  });
  return `FDNY_CHECKPOINTS_V1|${project.source}|${rows.length}\n${rows.join('\n')}\n`;
}

// Reopen the same file installed in FDNY; reconstruct editable centers/sizes.
export function readCheckpointFile(text){
  if(typeof text!=='string'||text.length>=8192)throw Error('Invalid checkpoint file.');
  const lines=text.replace(/^\uFEFF/,'').replace(/\r\n/g,'\n').replace(/\n$/,'').split('\n'),header=lines.shift().split('|');
  if(header.length!==3||header[0]!=='FDNY_CHECKPOINTS_V1'||!/^\d+$/.test(header[2])||Number(header[2])!==lines.length)throw Error('Invalid checkpoint file header or count.');
  const project={source:header[1],triggers:lines.map((line,i)=>{
    const fields=line.split('|');
    if(fields.length!==7||fields.slice(0,6).some(v=>!/^[-+]?(?:\d+(?:\.\d*)?|\.\d+)$/.test(v)))throw Error('Invalid checkpoint bounds.');
    const bounds=fields.slice(0,6).map(Number);
    return {name:'volume'+(i+1),label:fields[6],center:[0,1,2].map(a=>Number(((bounds[a]+bounds[a+3])/2).toFixed(6))),size:[0,1,2].map(a=>Number((bounds[a+3]-bounds[a]).toFixed(6)))};
  })};
  checkpointFile(project);return project;
}
