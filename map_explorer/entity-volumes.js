// Static, authored positions. These boxes do not simulate moving entities.
export const VOLUME_TYPES={
  pickup:{label:'Pickup boxes',color:'#008246',description:'Victim pickup contact · J_Radius (fallback 60) on each axis, at base and alternate spawn candidates. Actual victims can move.'},
  brush:{label:'Brush bounds',color:'#865aae',description:'Authored bounds of all linked animated brush models · doors, ladders, breakable objects and other brushes. Bounds are not exact collision shapes.'}
};
export function getEntityVolumes(data){
  const victims=new Map(data.markers.filter(m=>m.category==='VICTIM').map(m=>[m.label,m]));
  return data.markers.flatMap(m=>{
    const victim=m.category==='VICTIM'?m:m.category==='ALT'?victims.get(m.victim_label):null;
    if(victim){
      const authored=Number(victim.raw.J_Radius),radius=Number.isFinite(authored)&&authored>0?authored:60;
      const center=[m.x,m.y,m.z];
      return [{key:m.id+':pickup',markerId:m.id,kind:'pickup',radius,bounds:[{min:center.map(v=>v-radius),max:center.map(v=>v+radius)}]}];
    }
    if(m.class==='RCS_BModel'&&m.brush_bounds?.length)return [{key:m.id+':brush',markerId:m.id,kind:'brush',bounds:m.brush_bounds}];
    return [];
  });
}
