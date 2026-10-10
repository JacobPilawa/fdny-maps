#!/usr/bin/env python3
from __future__ import annotations
import argparse, importlib.util, json, sys, struct
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
for name,path in [('extract_gbsp',ROOT/'tools/extract_gbsp.py'),('render_annotated_maps',ROOT/'tools/render_annotated_maps.py'),('render_simple_maps',ROOT/'tools/render_simple_maps.py')]:
 spec=importlib.util.spec_from_file_location(name,path); mod=importlib.util.module_from_spec(spec); sys.modules[name]=mod; spec.loader.exec_module(mod)
eg=sys.modules['extract_gbsp']; ra=sys.modules['render_annotated_maps']; sm=sys.modules['render_simple_maps']

def load(path):
 _,_,p=eg.read_chunks(path)
 return {
  'faces': eg.parse_faces(p.get(11,b'')), 'verts': eg.vecs(p.get(14,b'')),
  'idx': list(struct.unpack('<'+'i'*(len(p.get(13,b''))//4),p.get(13,b''))) if 13 in p else [],
  'textures': eg.parse_textures(p.get(18,b'')), 'texinfo': eg.parse_texinfo(p.get(17,b'')),
  'planes': eg.parse_planes(p.get(10,b'')), 'entities': eg.parse_entities(p.get(16,b''))[1],
 }

def marker_payload(anns):
 out=[]
 victim_by_internal={}
 for a in anns:
  if a['category']=='VICTIM':
   internal=a.get('raw',{}).get('%name%','')
   if internal:
    victim_by_internal[internal]=a['label']
 for a in anns:
  if a['category'] not in {'START','VICTIM','ALT','FIRE','HOSE','VALVE','EXIT','DOOR'}: continue
  row={k:a.get(k,'') for k in ['label','category','entity_index','class','name','x','y','z','victim_link','scripts']}
  row['internal_id']=a.get('raw',{}).get('%name%','')
  row['victim_label']=victim_by_internal.get(a.get('victim_link',''),'')
  out.append(row)
 return out

def export(name,bsp,out):
 g=load(bsp); floors,walls=sm.classify(g); anns=ra.build_annotations(g['entities'])
 markers=marker_payload(anns)
 # Use only larger floor polys for outline/reference; wall segments do the main work.
 floor_lines=[]
 for p in floors:
  if p['area'] < 700: continue
  pts=[[round(x,2),round(y,2),round(z,2)] for x,y,z in p['points']]
  floor_lines.append(pts)
 xs=[]; zs=[]
 for a,b in walls: xs += [a[0],b[0]]; zs += [a[1],b[1]]
 for m in markers: xs.append(m['x']); zs.append(m['z'])
 data={'name':name,'bounds':{'minX':min(xs),'maxX':max(xs),'minZ':min(zs),'maxZ':max(zs)},'walls':walls,'floors':floor_lines,'markers':markers}
 out.parent.mkdir(parents=True,exist_ok=True); out.write_text(json.dumps(data,separators=(',',':')),encoding='utf-8')
 print(json.dumps({'map':name,'out':str(out),'walls':len(walls),'floors':len(floor_lines),'markers':len(markers)},indent=2))

def main():
 ap=argparse.ArgumentParser(); ap.add_argument('name'); ap.add_argument('bsp',type=Path); ap.add_argument('out',type=Path)
 a=ap.parse_args(); export(a.name,a.bsp,a.out)
if __name__=='__main__': main()
