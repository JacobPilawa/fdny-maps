"""Supplement April 30 annotations with authored inventory, scripts and BSP motion.
No stage simulation: clips inspect brush motion and explicitly authored delays.
"""
import re,json
from pathlib import Path

def macros_from(text):
 out={}
 for match in re.finditer(r'(?im)^\s*set\s+(\w+)\s*\(',text):
  start=match.end()-1; depth=0
  for i in range(start,len(text)):
   depth+=(text[i]=='(')-(text[i]==')')
   if depth==0:
    out[match[1]]=text[start:i+1];break
 return out

def expand(text,macros,seen=()):
 def sub(m):
  key=next((k for k in macros if k.lower()==m[1].lower()),None)
  return expand(macros[key],macros,(*seen,key)) if key and key not in seen else m[0]
 return re.sub(r'\*(\w+)',sub,text)

def motion_tracks(payload):
 tracks={}
 for block in re.split(r'\bModelNum\s+',payload.decode('latin1'))[1:]:
  model=int(block.split()[0]);channels={}
  for name,size in [('Rotation',4),('Translation',3)]:
   match=re.search(name+r'\s+1\s+(\d+)\s+Keys\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s*\n',block)
   if not match:continue
   count=int(match[2]);compression=int(match[4]);lines=block[match.end():].splitlines();times=[];values=[]
   if compression&2:
    start,delta=map(float,lines.pop(0).split()[:2]);times=[start+i*delta for i in range(count)]
   for line in lines[:count]:
    numbers=[float(v) for v in line.split()]
    if not compression&2:times.append(numbers.pop(0))
    assert len(numbers)==size,(model,name,line)
    # BSP text quaternion is WXYZ; Three and the ACT decoder use XYZW.
    values.append(numbers[1:]+numbers[:1] if size==4 else numbers)
   channels[name.lower()]={'times':times,'values':values,'interpolation':int(match[3])}
  if channels:tracks[str(model)]=channels
 return tracks

def tell_actions(text):
 for match in re.finditer(r'\b(?:tell|action)\s+',text,re.I):
  tail=text[match.end():];end=0
  if tail and tail[0] in '([':
   depth=0
   for i,ch in enumerate(tail):
    depth+=(ch in '([')-(ch in ')]')
    if depth==0:end=i+1;break
  else:
   token=re.match(r'[\w.-]+',tail);end=token.end() if token else 0
  targets=re.findall(r'[\w.-]+',tail[:end]);action=re.match(r'\s*\(?\s*(\w+)',tail[end:])
  if action:
   for target in targets:yield target,action[1]

def supplement(root,key,stem,entities,models,markers,payload):
 macros={}
 for f in sorted((root/'source/scripts/scripts').glob('*.evs')):macros.update(macros_from(f.read_text(errors='replace')))
 for f in (root/'source/scripts/maps').glob('*.evs'):
  if f.stem.lower()==stem.lower():macros.update(macros_from(f.read_text(errors='replace')))
 model_ids={e['%name%']:int(e['Model']) for e in entities if e.get('classname')=='%Model%' and 'Model' in e}
 for i,mod in enumerate(models):mod['id']=i
 existing={m['entity_index']:m for m in markers}
 counts={}
 for idx,e in enumerate(entities):
  cls=e.get('classname');scripts='\n'.join(v for k,v in e.items() if k.startswith('S_Script') and v!='<null>')
  if idx in existing:
   if cls=='RCS_PropModel' and re.search(r'RefillExt',scripts,re.I):existing[idx]['category']='REFILL'
   continue
  category={'RCS_BModel':'OBJECT','RCS_PropModel':'BONUS' if re.search(r'BONUS',scripts,re.I) else 'OBJECT','RCS_MiscEmitter':'EMITTER'}.get(cls)
  if not category:continue
  loc=[float(v) for v in e.get('ZZ_location',e.get('location','0 0 0')).split()]
  linked=[model_ids[v] for k,v in e.items() if k.startswith('C_AnimModel') and v in model_ids]
  if linked:
   bs=[models[i] for i in linked];loc=[(min(b['mins'][a] for b in bs)+max(b['maxs'][a] for b in bs))/2 for a in range(3)]
  counts[category]=counts.get(category,0)+1;label={'OBJECT':'O','BONUS':'B','EMITTER':'FX'}[category]+str(counts[category])
  resource_names={50:'Briefcase',51:'Trophy',52:'Laptop',53:'Gas can',54:'Jewelry',55:'Painting',56:'Gun',57:'Documents',58:'Purse',59:'Torch',60:'Money stack',61:'Gold bars',62:'Painting',63:'Deposit box',64:'Medal'}
  display=e.get('A_Name');display=display if display and display!='<null>' else resource_names.get(int(e.get('D_Model','-1')),e.get('%name%',label))
  markers.append({'entity_index':idx,'label':label,'category':category,'class':cls,'name':display,'x':loc[0],'y':loc[1],'z':loc[2],'scripts':scripts,'victim_link':'','raw':e})
 tracks=motion_tracks(payload)
 controllers=[]
 for m in markers:
  e=m['raw'];m['expanded_scripts']=expand(m.get('scripts',''),macros)
  mids=[model_ids[v] for k,v in sorted(e.items()) if k.startswith('C_AnimModel') and v in model_ids]
  m['models']=mids or ([model_ids[e['C_Model']]] if e.get('C_Model') in model_ids else [])
  if not mids:continue
  moving=[i for i in mids if str(i) in tracks and any(len({tuple(v) for v in c['values']})>1 for c in tracks[str(i)].values())]
  if not moving:continue
  expanded=m['expanded_scripts'];rate_match=re.search(r'\bsettimescale\s+([\d.]+)',expanded,re.I);rate=float(rate_match[1]) if rate_match else 1
  rate=max(rate,.01);duration=max(t for i in moving for c in tracks[str(i)].values() for t in c['times'])/rate
  controllers.append({'id':m['label'],'name':m['name'],'duration':duration,'parts':[{'model':i,'rate':rate,'delay':0} for i in moving]})
 major_names={'bank':['break','column'],'bar_r':['ClubRafterFall'],'boat_b':['button'],'oil_rig_b':['catwalk03','catwalk01','catwalk02'],'oil_rig_c':['TowerStairs']}
 clips=[]
 for name in major_names.get(key,[]):
  c=next((c for c in controllers if c['name'].lower()==name.lower()),None)
  if c:clips.append({**c,'major':True})
 major_labels={'bank':{'break':'Bank floor / ramp collapse','column':'Bank structural collapse'},'bar_r':{'ClubRafterFall':'Club rafters collapse'},'boat_b':{'button':'Cargo door / hook lift'}}
 for c in clips:c['name']=major_labels.get(key,{}).get(c['name'],c['name'])
 if key=='oil_rig_b' and len(clips)==3:
  parts=[{**p,'delay':index*.5} for index,c in enumerate(clips) for p in c['parts']]
  clips.insert(0,{'id':'warehouse-catwalks','name':'Warehouse catwalk collapse','major':True,'duration':max(c['duration']+index*.5 for index,c in enumerate(clips)),'parts':parts})
 if key=='oil_rig_c' and clips:
  clips[0]['name']='Tower stairs collapse';clips[0]['parts']=[{**p,'delay':1.5} for p in clips[0]['parts']];clips[0]['duration']+=1.5
 clips.extend(c for c in controllers if c['id'] not in {c['id'] for c in clips})
 by_name={m['name'].lower():m for m in markers};by_name.update({m['raw'].get('%name%','').lower():m for m in markers})
 links=[];zones=[]
 player=next((m for m in markers if m['category']=='START'),None)
 if player:by_name['player']=player
 for m in markers:
  for target,action in tell_actions(m['expanded_scripts']):
   target=target.strip('(),[]');dest=by_name.get(target.lower())
   if dest and dest['label']!=m['label']:
    link={'from':m['label'],'to':dest['label'],'action':action}
    if link not in links:links.append(link)
  zone=re.search(r'\bmakezone\s*\(([^)]+)\)',m['expanded_scripts'],re.I)
  if zone and m.get('category')=='TRIGGER':zones.append({'name':zone[1],'marker':m['label']})
 grouped={}
 for z in zones:
  if z['name'] not in grouped:grouped[z['name']]={**z,'markers':[]}
  grouped[z['name']]['markers'].append(z['marker'])
 zones=list(grouped.values())
 (root/'data/animations').mkdir(exist_ok=True)
 (root/'data/animations'/f'{key}.json').write_text(json.dumps(tracks,separators=(',',':')))
 return {'clips':clips,'zones':zones,'links':links,'macros':macros},model_ids
