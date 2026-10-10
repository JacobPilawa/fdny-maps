"""Rebuild compact explorer data from the copied BSPs and April 30 annotations."""
from pathlib import Path
import json,sys,struct,math,re
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'source/tools'))
import extract_gbsp as eg
import render_annotated_maps as ra
names={'training':'Training','bank':'Bank','bar_r':'Downtown Bar','gas_r':'Gas Station','mbase':'Military Base','boat_a':'Capsized Boat A','boat_b':'Capsized Boat B','city_h':'City · hose','const_h':'Construction · hose','dock_h':'Dock · hose','oil_rig_h':'Oil Rig · hose'}
manifest=[]
for m in json.loads((ROOT/'source/manifest.json').read_text()):
 key,stem=m['key'],m['stem'];name=names.get(key,m['name']);_,_,p=eg.read_chunks(ROOT/'source/maps'/(stem+'.bsp'))
 models=eg.parse_models(p[1]);faces=eg.parse_faces(p[11]);verts=eg.vecs(p[14]);indices=list(struct.unpack('<'+'i'*(len(p[13])//4),p[13]));planes=eg.parse_planes(p[10]);textures=eg.parse_textures(p[18]);ti=eg.parse_texinfo(p[17]);entities=eg.parse_entities(p[16])[1]
 markers=json.loads((ROOT/'source'/key/'annotated_entities.json').read_text())
 # The early annotations covered rescue stages only. Preserve their IDs and
 # add the original hose-camera path records for the four hose maps.
 for idx,e in enumerate(entities):
  cls=e.get('classname','');loc=ra.get_location(e)
  if not loc or cls not in {'RCS_PlayerHose','RCS_HosePath'}:continue
  cat='START' if cls=='RCS_PlayerHose' else 'PATH'
  label=('S' if cat=='START' else 'N')+str(1+sum(a['category']==cat for a in markers))
  markers.append({'entity_index':idx,'label':label,'category':cat,'class':cls,'name':e.get('A_Name') or e.get('%name%',label),'x':loc[0],'y':loc[1],'z':loc[2],'scripts':'','victim_link':'','raw':e})
 modelByName={e['%name%']:models[int(e['Model'])] for e in entities if e.get('classname')=='%Model%' and 'Model' in e and 0<=int(e['Model'])<len(models)}
 hiddenFaces=set()
 for e in entities:
  if e.get('classname')=='RCS_BoundsTrigger':
   mod=modelByName.get(e.get('C_Model'))
   if mod:hiddenFaces.update(range(mod['first_face'],mod['first_face']+mod['num_faces']))
 victimById={a['raw'].get('%name%'):a['label'] for a in markers if a['category']=='VICTIM'}
 victim_order=0
 for a in markers:
  raw=a['raw'];mod=modelByName.get(raw.get('C_Model')) or modelByName.get(raw.get('C_AnimModel0'))
  if mod:a['bounds']={'min':mod['mins'],'max':mod['maxs']}
  if raw.get('classname')=='RCS_BModel':
   a['brush_bounds']=[{'model':model_name,'min':modelByName[model_name]['mins'],'max':modelByName[model_name]['maxs']} for field,model_name in sorted(raw.items()) if field.startswith('C_AnimModel') and model_name in modelByName]
  a['victim_label']=victimById.get(a.get('victim_link'),'')
  a['script_lines']=[v for k,v in sorted(raw.items()) if k.startswith('S_Script') and v and v!='<null>']
  a['id']=a['label'];a['internal_id']=raw.get('%name%','')
  if a['category']=='VICTIM':
   victim_order+=1;a['victim_order']=victim_order
   match=re.search(r'\broam\s+([0-9.]+)',a['scripts'],re.I)
   a['roam_radius']=float(match[1]) if match else None
  a['path_next']=raw.get('A_NextPoint') or raw.get('FirstPathPoint')
 polys=[];walls=[];wallSeen=set();xs=[];ys=[];zs=[]
 for idx,face in enumerate(faces):
  if idx in hiddenFaces:continue
  pts=eg.face_vertices(face,indices,verts)
  if len(pts)<3:continue
  tex=ra.face_texture(face,ti,textures)
  if tex.startswith(('sky_','col_')) or any(t in tex for t in ['trigger','clip','hint','null']):continue
  plane=planes[face['plane_num']];n=[v*(-1 if face['plane_side'] else 1) for v in plane['normal']]
  rounded=[[round(v,3) for v in point] for point in pts]
  # Retain actual heights and polygons, rather than extruding old 2D lines.
  polys.append({'p':rounded,'n':[round(v,3) for v in n],'y':round(sum(v[1] for v in pts)/len(pts),3)})
  for x,y,z in pts:xs.append(x);ys.append(y);zs.append(z)
  if abs(n[1])<.28 and max(v[1] for v in pts)-min(v[1] for v in pts)>=32:
   a,b=max(((a,b) for i,a in enumerate(pts) for b in pts[i+1:]),key=lambda ab:(ab[0][0]-ab[1][0])**2+(ab[0][2]-ab[1][2])**2)
   if math.hypot(a[0]-b[0],a[2]-b[2])<80:continue
   qa=[round(a[0]/16)*16,round(a[2]/16)*16];qb=[round(b[0]/16)*16,round(b[2]/16)*16]
   lo=round(min(v[1] for v in pts),2);hi=round(max(v[1] for v in pts),2);sig=(tuple(sorted([tuple(qa),tuple(qb)])),lo,hi)
   if sig not in wallSeen:wallSeen.add(sig);walls.append([qa,qb,lo,hi])
 for a in markers:xs.append(a['x']);ys.append(a['y']);zs.append(a['z'])
 bounds={'minX':min(xs),'maxX':max(xs),'minY':min(ys),'maxY':max(ys),'minZ':min(zs),'maxZ':max(zs)}
 data={'key':key,'stem':stem,'name':name,'bounds':bounds,'markers':markers,'walls':walls,'polygons':polys}
 data['script_files']=[str(f.relative_to(ROOT)) for f in sorted((ROOT/'source/scripts/scripts').glob('*.evs'))]
 for f in (ROOT/'source/scripts/maps').glob('*.evs'):
  if f.stem.lower()==stem.lower():data['script_files'].insert(0,str(f.relative_to(ROOT)))
 (ROOT/'data'/(key+'.json')).write_text(json.dumps(data,separators=(',',':')))
 manifest.append({'key':key,'stem':stem,'name':name,'entities':len(markers),'victims':sum(a['category']=='VICTIM' for a in markers)})
 print(key,len(markers),len(polys),len(walls))
manifest.sort(key=lambda m:(m['key']!='training',m['name']))
(ROOT/'data/manifest.json').write_text(json.dumps(manifest,separators=(',',':')))
