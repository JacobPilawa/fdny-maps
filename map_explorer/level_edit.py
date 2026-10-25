"""Local-only retail GBSP trigger editing and mod export. Never writes game files."""
from __future__ import annotations
import hashlib,io,json,math,os,re,struct,zipfile
from pathlib import Path
from xml.etree import ElementTree as ET
from livesplit.custom_triggers import generate as generate_splitter

ROOT=Path(__file__).resolve().parent
KEY='fdny_edit'
SCHEMA='fdny-level-edit-v1'
MAX_TRIGGERS=32

def digest(b):return hashlib.sha256(b).hexdigest()
def game_directory():
    explicit=os.environ.get('FDNY_GAME_DIR')
    candidates=[Path(explicit)] if explicit else []
    for parent in [ROOT,*ROOT.parents]:
        candidates += [parent/'FDNY.app',parent/'FireFighter American Hero']
    for p in candidates:
        if p.suffix.lower()=='.app':p=p/'Contents/SharedSupport/prefix/drive_c/Program Files/FireFighter American Hero'
        if (p/'Data/ResDefs/thegame.lev').is_file() and (p/'Data/Maps').is_dir():return p.resolve()
    raise ValueError('Retail game files not found. Set FDNY_GAME_DIR to the game folder or FDNY.app, then restart start.sh.')
def child(folder,name):
    matches=[p for p in folder.iterdir() if p.name.casefold()==name.casefold() and p.is_file()]
    if len(matches)!=1:raise ValueError(f'Required game file not found or ambiguous: {name}')
    return matches[0]

def catalog(raw):
    text=raw.decode('latin1');parts=list(re.finditer(r'(?im)^level:\s*([^\s;]+)[^\r\n]*',text));records=[]
    for n,m in enumerate(parts):
        end=parts[n+1].start() if n+1<len(parts) else len(text);block=text[m.start():end]
        fields={k.lower():v.strip().strip('"') for k,v in re.findall(r'(?m)^(\w+):\s*([^\r\n]*)',block)}
        records.append(dict(key=fields['level'].lower(),fields=fields,start=m.start(),end=end))
    return text,records

def missions(game):
    _,records=catalog((game/'Data/ResDefs/thegame.lev').read_bytes())
    return [dict(key=r['key'],title=r['fields'].get('title',r['key'])) for r in records if r['fields'].get('rrf','').casefold()=='rescue.rrf' and r['key']!=KEY]

class Bsp:
    def __init__(self,raw):
        self.raw=raw;self.chunks=[];self.payload={};pos=0
        while pos+12<=len(raw):
            typ,size,count=struct.unpack_from('<3i',raw,pos);pos+=12
            if size<0 or count<0 or typ in self.payload:raise ValueError('Invalid BSP chunk header')
            length=size*count
            if pos+length>len(raw):raise ValueError('Truncated BSP')
            data=raw[pos:pos+length];pos+=length;self.chunks.append((typ,size,count));self.payload[typ]=data
            if typ==65535:break
        if pos!=len(raw) or not self.chunks or self.chunks[-1][0]!=65535:raise ValueError('Invalid BSP end marker')
        if self.payload.get(0,b'')[:5]!=b'GBSP\0' or struct.unpack_from('<i',self.payload[0],8)[0]!=15:raise ValueError('Only GBSP v15 maps are supported')
        self.entities=self.read_entities(self.payload[16])
    @staticmethod
    def read_entities(data):
        pos=0
        def integer():
            nonlocal pos
            if pos+4>len(data):raise ValueError('Truncated entity data')
            n=struct.unpack_from('<i',data,pos)[0];pos+=4
            if n<0:raise ValueError('Negative entity length')
            return n
        def string():
            nonlocal pos
            n=integer()
            if n>len(data)-pos:raise ValueError('Truncated entity string')
            value=data[pos:pos+n];pos+=n;return value
        es=[]
        for _ in range(integer()):
            pairs=[]
            for _ in range(integer()):pairs.append((string(),string()))
            es.append(pairs)
        if pos!=len(data):raise ValueError('Trailing entity bytes')
        return es
    @staticmethod
    def pairs(entity):return {k.split(b'\0')[0].decode('latin1'):v.split(b'\0')[0].decode('latin1') for k,v in entity}
    @staticmethod
    def make_entity(fields):return [(k.encode('latin1')+b'\0',v.encode('latin1')+b'\0') for k,v in fields.items()]
    @staticmethod
    def set_value(entity,key,value):
        for i,(k,v) in enumerate(entity):
            if k.split(b'\0')[0].decode('latin1')==key:entity[i]=(k,value.encode('latin1')+b'\0');return
        entity.append((key.encode('latin1')+b'\0',value.encode('latin1')+b'\0'))
    def entity_bytes(self):
        result=bytearray(struct.pack('<i',len(self.entities)))
        for entity in self.entities:
            result+=struct.pack('<i',len(entity))
            for k,v in entity:
                for value in (k,v):result+=struct.pack('<i',len(value))+value
        return bytes(result)
    def write(self):
        self.payload[16]=self.entity_bytes();out=bytearray()
        for typ,size,count in self.chunks:
            data=self.payload[typ]
            if typ==65535:out+=struct.pack('<3i',typ,size,count);continue
            if len(data)%size:raise ValueError('Misaligned BSP chunk')
            out+=struct.pack('<3i',typ,size,len(data)//size)+data
        return bytes(out)
    def record(self,typ,index,fmt):return struct.unpack_from(fmt,self.payload[typ],index*struct.calcsize(fmt))
    def append(self,typ,fmt,values):
        index=len(self.payload[typ])//struct.calcsize(fmt);self.payload[typ]+=struct.pack(fmt,*values);return index
    def model_index(self,name):
        for e in self.entities:
            d=self.pairs(e)
            if d.get('classname')=='%Model%' and d.get('%name%','').casefold()==name.casefold():return int(d['Model'])
        raise ValueError('Trigger model link is unresolved')
    def bounds(self,model):
        m=self.record(1,model,'<2i9f8iI');n=self.record(2,m[0],'<5i6f')
        # Verified retail 004818C0 uses 004E2928 = binary32 128.0.
        f=lambda x:struct.unpack('<f',struct.pack('<f',x))[0]
        return [f(v+128) for v in n[5:8]],[f(v-128) for v in n[8:11]]
    def add_box(self,lo,hi):
        """Clone a nonblocking trigger brush into entirely new, affine-mapped records.
        No existing topology or indices are changed. Independent planes, nodes,
        leaves, faces and vertices avoid moving a shared model or changing walls.
        """
        donor=None
        for e in self.entities:
            d=self.pairs(e)
            if d.get('classname')!='RCS_BoundsTrigger' or not d.get('C_Model'):continue
            try:
                index=self.model_index(d['C_Model']);m=self.record(1,index,'<2i9f8iI')
                if m[12]>0 and m[12]<=64 and m[14]>0 and m[14]<=64 and all(m[5+i]>m[2+i] for i in range(3)):
                    leafs=[self.record(4,i,'<i6f8i') for i in range(m[13],m[13]+m[14])]
                    # Donor interiors must be nonblocking and have no portals,
                    # leaf faces or collision sides (retail trigger topology).
                    if all(not (r[0]&0x43) and r[8]==0 and r[10]==0 and r[14]==0 for r in leafs):donor=(m,leafs);break
            except (ValueError,struct.error):continue
        if donor is None:raise ValueError('No supported nonblocking trigger brush in this map. Existing-trigger text edits are still available.')
        m,leafs=donor;scale=[(hi[i]-lo[i])/(m[5+i]-m[2+i]) for i in range(3)];shift=[lo[i]-scale[i]*m[2+i] for i in range(3)]
        point=lambda p:[p[i]*scale[i]+shift[i] for i in range(3)]
        planes={};vertices={};nodes={};new_leaf=len(self.payload[4])//60;first_face=len(self.payload[11])//36
        def plane(old):
            if old not in planes:
                p=self.record(10,old,'<4fi');n=[p[i]/scale[i] for i in range(3)];length=math.sqrt(sum(v*v for v in n));dist=(p[3]+sum(n[i]*shift[i] for i in range(3)))/length;n=[v/length for v in n]
                typ=next((i for i in range(3) if abs(abs(n[i])-1)<1e-6 and all(abs(n[j])<1e-6 for j in range(3) if i!=j)),3+max(range(3),key=lambda i:abs(n[i])))
                planes[old]=self.append(10,'<4fi',[*n,dist,typ])
            return planes[old]
        for old in range(m[11],m[11]+m[12]):
            face=list(self.record(11,old,'<8i4B'));first=len(self.payload[13])//4
            for vi in range(face[0],face[0]+face[1]):
                v=self.record(13,vi,'<i')[0]
                if v not in vertices:vertices[v]=self.append(14,'<3f',point(self.record(14,v,'<3f')))
                self.append(13,'<i',[vertices[v]]);self.payload[15]+=self.payload[15][vi*12:vi*12+12]
            face[0]=first;face[2]=plane(face[2]);face[5]=-1;face[6]=face[7]=0;face[8:12]=[255]*4
            self.append(11,'<8i4B',face)
        for leaf in leafs:
            r=list(leaf);r[1:4]=point(leaf[1:4]);r[4:7]=point(leaf[4:7]);r[7]=len(self.payload[12])//4;r[11]=-1;r[12]=0;r[13]=-1
            self.append(4,'<i6f8i',r)
        def clone_node(old):
            if old<0:
                leaf=-old-1
                if not m[13]<=leaf<m[13]+m[14]:raise ValueError('Donor leaf lies outside model')
                return -(new_leaf+leaf-m[13]+1)
            if old in nodes:return nodes[old]
            r=list(self.record(2,old,'<5i6f'));new=len(self.payload[2])//44;nodes[old]=new;self.append(2,'<5i6f',[0]*11)
            r[0]=clone_node(r[0]);r[1]=clone_node(r[1]);r[3]=first_face+r[3]-m[11] if r[2] else first_face;r[4]=plane(r[4])
            # Conservative node boxes; root box contracts exactly to authored bounds.
            r[5:8]=[v-128 for v in lo];r[8:11]=[v+128 for v in hi]
            buf=bytearray(self.payload[2]);struct.pack_into('<5i6f',buf,new*44,*r);self.payload[2]=bytes(buf);return new
        root=clone_node(m[0]);center=[(lo[i]+hi[i])/2 for i in range(3)]
        return self.append(1,'<2i9f8iI',[root,0,*lo,*hi,*center,first_face,m[12],new_leaf,m[14],-1,0,0,0,0])

def stage_finishes(key):
    return {'boat_a':['boat_a','boat_b'],'subway_a':['subway_a','subway_b'],'oil_rig_a':['oil_rig_a','oil_rig_b','oil_rig_c','oil_rig_d']}.get(key,[key])

def describe(game,key):
    if key not in {m['key'] for m in missions(game)}:raise ValueError('Choose an existing rescue mission')
    raw=child(game/'Data/Maps',key+'.bsp').read_bytes();b=Bsp(raw);triggers=[];spawn=[0,0,0]
    for i,e in enumerate(b.entities):
        d=b.pairs(e)
        if d.get('classname')=='RCS_PlayerRescue':spawn=[float(v) for v in d['ZZ_location'].split()]
        if d.get('classname')!='RCS_BoundsTrigger' or not d.get('C_Model'):continue
        try:lo,hi=b.bounds(b.model_index(d['C_Model']))
        except (ValueError,struct.error):continue
        if any(hi[a]<=lo[a] for a in range(3)):continue
        scripts=[d.get('S_Script%02d'%n,'') for n in range(1,11)]
        triggers.append(dict(entity=i,name=d.get('A_Name',str(i)),center=[(lo[a]+hi[a])/2 for a in range(3)],size=[hi[a]-lo[a] for a in range(3)],scripts=scripts))
    return dict(key=key,source_sha256=digest(raw),spawn=spawn,triggers=triggers,stage_finishes=stage_finishes(key))

def validate(project):
    if not isinstance(project,dict) or project.get('schema')!=SCHEMA:raise ValueError('Unsupported level project')
    records=project.get('triggers')
    if not isinstance(records,list) or not 1<=len(records)<=MAX_TRIGGERS:raise ValueError('Add between 1 and 32 trigger edits')
    names=set();existing=set()
    for t in records:
        if not isinstance(t,dict):raise ValueError('Invalid trigger')
        if not isinstance(t.get('name'),str) or not t['name'] or (t.get('existing_entity') is None and not re.fullmatch(r'[A-Za-z][A-Za-z0-9_]{0,39}',t['name'])):raise ValueError('Trigger names must start with a letter and contain only letters, digits and underscores (40 max)')
        if t['name'].casefold() in names:raise ValueError('Trigger names must be unique')
        names.add(t['name'].casefold())
        for key in ['center','size']:
            v=t.get(key)
            if not isinstance(v,list) or len(v)!=3 or any(type(x) not in (int,float) or not math.isfinite(x) or abs(x)>100000 for x in v):raise ValueError('Position and size require three finite numbers within 100000')
        if any(x<8 or x>10000 for x in t['size']):raise ValueError('Trigger dimensions must be between 8 and 10000 units')
        message=t.get('message','')
        if not isinstance(message,str) or not 1<=len(message)<=240 or any(ord(c)<32 or ord(c)>126 or c in '()' for c in message):raise ValueError('Use 1–240 plain ASCII characters without parentheses for the message')
        seconds=t.get('seconds')
        if type(seconds) not in (int,float) or not math.isfinite(seconds) or not 1<=seconds<=30:raise ValueError('Message duration must be 1–30 seconds')
        if type(t.get('checkpoint',False)) is not bool:raise ValueError('Checkpoint must be true or false')
        if 'split_label' in t and (not isinstance(t['split_label'],str) or not 1<=len(t['split_label'].strip())<=80 or any(ord(c)<32 for c in t['split_label'])):raise ValueError('Split labels require 1–80 characters without control characters')
        if t.get('existing_entity') is not None:
            n=t['existing_entity']
            if type(n) is not int or n<0 or n in existing:raise ValueError('An existing trigger can only be edited once')
            existing.add(n)
    return records

def checkpoint_variable(trigger):
    # Independent of label/order; safe script identifier even for authored names.
    return 'FDNY_CP_'+digest(trigger['name'].casefold().encode())[:16]

def split_files(project,edits,map_bytes):
    checkpoints=[dict(id=checkpoint_variable(t),label=t.get('split_label',t['name']),trigger=t['name'],variable=checkpoint_variable(t),from_value='0',to_value='1',center=t['center'],size=t['size']) for t in edits if t.get('checkpoint',False)]
    # The selected map is cloned; subsequent stage exits still load stock maps.
    stages=stage_finishes(project['source'])
    profile=dict(schema='fdny-checkpoints-v1',status='generated-asl-live-validation-pending',source_map=project['source'],runtime_map=KEY,map_sha256=digest(map_bytes),activation='game-script-variable',reset='Variables set to 0 by exported map EVS on each load; checkpoints set their variable to 1 on touch.',order='listed; once per attempt; out-of-order first activations invalidate checkpoint timing until restart',checkpoints=checkpoints,default_stage_finishes=stages,finish_detection='Preserve upstream stage/win detection; apply the source map guard to the cloned runtime map. Final-stage end-sound slots on clones need live verification.',timing='Retain upstream Game Time and load removal. Variable polling is not a frame-exact event timestamp.')
    run=ET.Element('Run',version='1.7.0');ET.SubElement(run,'GameIcon');ET.SubElement(run,'GameName').text='FDNY Firefighter'
    ET.SubElement(run,'CategoryName').text=project['source']+' custom checkpoints'
    metadata=ET.SubElement(run,'Metadata');ET.SubElement(metadata,'Run',id='');ET.SubElement(metadata,'Platform',usesEmulator='False');ET.SubElement(metadata,'Region');ET.SubElement(metadata,'Variables')
    ET.SubElement(run,'Offset').text='00:00:00';ET.SubElement(run,'AttemptCount').text='0';ET.SubElement(run,'AttemptHistory');segments=ET.SubElement(run,'Segments')
    for label in [c['label'] for c in checkpoints]+[s+' complete' for s in stages]:
        segment=ET.SubElement(segments,'Segment');ET.SubElement(segment,'Name').text=label;ET.SubElement(segment,'Icon');times=ET.SubElement(segment,'SplitTimes');ET.SubElement(times,'SplitTime',name='Personal Best');ET.SubElement(segment,'BestSegmentTime');ET.SubElement(segment,'SegmentHistory')
    ET.SubElement(run,'AutoSplitterSettings')
    directory=ROOT/'livesplit'
    files={output:generate_splitter(profile,(directory/source).read_text()).encode() for output,source in [('timing/FDNY_Custom_ILs.asl','FDNY_ILs.asl'),('timing/FDNY_Custom_rescue%.asl','FDNY_rescue%.asl')]}
    files.update({'timing/checkpoints.json':json.dumps(profile,indent=2).encode(),'timing/custom-splits.lss':ET.tostring(run,encoding='utf-8',xml_declaration=True),'timing/README.txt':b'''Custom checkpoint timing\n\n1. Install the level using the ZIP's main README.\n2. In LiveSplit, open custom-splits.lss (File -> Open Splits -> From File).\n3. Edit Layout -> + -> Control -> Scriptable Auto Splitter. Choose\n   FDNY_Custom_ILs.asl in that component's settings. Use only one autosplitter.\n4. Select Game Time for load removal. Start Custom trigger test in the game.\n\nThe IL script resets at the menu; the rescue% variant requires manual resets\nand a run-wide split list if you use it across several missions. Both are\nstandalone ASLs: Python, checkpoints.json and FDNYCore are not needed at runtime.\nDo not select the stock ASL for this custom split list.\n\nCheckpoints read the real touch-script variables in listed order, once per\nattempt. Repeated touches do not split twice. Out-of-order or missing markers\nstop splitting until you restart. A checkpoint and finish observed together\nare emitted in checkpoint-first order. Default stage transitions and load\nremoval come from the upstream v2.2 scripts; cloned-map wins use the player\nmission-result value because custom names have no stock end-sound slot.\n\nGenerated C# and memory-layout fixtures can be tested without the game.\nReal-game touch timing, the cloned-map finish and your Wine/LiveSplit setup\nstill require gameplay validation. Keep this ZIP to restore the game.\n'''})
    return files

def export_project(game,project):
    if (game/'.fdny-level-edit-backup').exists():raise ValueError('Restore the previous Level Edit install before exporting another version (use install.py --restore from that export).')
    edits=validate(project);key=project.get('source');info=describe(game,key)
    raw=child(game/'Data/Maps',key+'.bsp').read_bytes()
    if project.get('source_sha256')!=digest(raw):raise ValueError('Source map changed. Reopen the mission before exporting.')
    bsp=Bsp(raw);original_names={bsp.pairs(e).get('A_Name','').casefold() for e in bsp.entities}
    for t in edits:
        script=f"touch: hint {t['seconds']:g} ({t['message']})"
        if t.get('checkpoint',False):script=f"touch: cmd [set {checkpoint_variable(t)} 1, hint {t['seconds']:g} ({t['message']})]"
        if t.get('existing_entity') is not None:
            i=t['existing_entity'];known=next((x for x in info['triggers'] if x['entity']==i),None)
            if not known or t['name']!=known['name']:raise ValueError('Existing trigger is not valid')
            if t['center']!=known['center'] or t['size']!=known['size']:raise ValueError('Existing trigger geometry cannot be moved; add a custom trigger instead')
            e=bsp.entities[i];d=bsp.pairs(e)
            slots=['S_Script%02d'%n for n in range(1,11)]
            slot=next((s for s in slots if re.match(r'^\s*touch:\s*hint\b',d.get(s,''),re.I)),None)
            slot=slot or next((s for s in slots if d.get(s,'').strip() in ('','<null>')),None)
            if not slot:raise ValueError('Existing trigger has no free script slot')
            bsp.set_value(e,slot,script)
        else:
            if t['name'].casefold() in original_names:raise ValueError('Custom trigger name conflicts with an existing object')
            original_names.add(t['name'].casefold());center=t['center'];lo=[center[a]-t['size'][a]/2 for a in range(3)];hi=[center[a]+t['size'][a]/2 for a in range(3)]
            model=bsp.add_box(lo,hi);name='fdnyedit_'+t['name'];origin=' '.join(f'{v:g}' for v in center)
            bsp.entities.append(bsp.make_entity({'Model':str(model),'classname':'%Model%','%name%':name,'origin':origin}))
            fields={'A_Name':t['name'],'C_Model':name,'classname':'RCS_BoundsTrigger','E_Reach':'0','ZZ_location':origin,'ZZ_spawn_message':'10000','%name%':name+'_trigger'}
            fields.update({'S_Script%02d'%n:script if n==1 else '<null>' for n in range(1,11)})
            bsp.entities.append(bsp.make_entity(fields))
    files={};scripts={}
    def rewrite(text):
        text=re.sub(r'(?i)(\bif(?:not)?\s+LEVEL\s+)'+re.escape(key)+r'\b',lambda m:m[1]+KEY,text)
        def include(m):
            name=m[1];safe=Path(name).name
            if safe!=name or not re.fullmatch(r'[\w.-]+\.evs',name,re.I):raise ValueError('Unsupported script include')
            output='fdnyedit_'+name.lower()
            if output not in scripts:
                scripts[output]=None
                scripts[output]=rewrite(child(game/'Data/Scripts',name).read_text(encoding='latin1'))
            return 'exec '+output
        return re.sub(r'(?i)\bexec\s+([\w./\\-]+\.evs)',include,text)
    for e in bsp.entities:
        for k,v in list(e):
            name=k.split(b'\0')[0].decode('latin1')
            if name.startswith('S_Script'):bsp.set_value(e,name,rewrite(v.split(b'\0')[0].decode('latin1')))
    files['Data/Maps/'+KEY+'.bsp']=bsp.write()
    for suffix in ['evs','brf','bmp']:
        try:p=child(game/'Data/Maps',key+'.'+suffix)
        except ValueError:
            if suffix=='evs':continue
            raise
        files['Data/Maps/'+KEY+'.'+suffix]=rewrite(p.read_text(encoding='latin1')).encode('latin1') if suffix=='evs' else p.read_bytes()
    if any(t.get('checkpoint',False) for t in edits):
        evs='Data/Maps/'+KEY+'.evs'
        resets=''.join('set '+checkpoint_variable(t)+' 0\r\n' for t in edits if t.get('checkpoint',False))
        files[evs]=resets.encode()+files.get(evs,b'')
    for name,text in scripts.items():files['Data/Scripts/'+name]=text.encode('latin1')
    catalog_raw=(game/'Data/ResDefs/thegame.lev').read_bytes();text,records=catalog(catalog_raw)
    source=next(r for r in records if r['key']==key);slot=next((r for r in records if r['key'] in ('dock_h',KEY)),None)
    if not slot:raise ValueError('First Hose slot not found in mission catalog')
    f=source['fields'];lines=[f'level: {KEY}','title: "Custom trigger test"','rrf: "rescue.rrf"']
    for field in ['music','time']:
        if field in f:lines.append(field+': '+f[field])
    lines.append('mapcoords: '+slot['fields'].get('mapcoords','109,11'))
    files['Data/ResDefs/thegame.lev']=(text[:slot['start']]+'\r\n'.join(lines)+'\r\n\r\n'+text[slot['end']:]).encode('latin1')
    manifest={'schema':SCHEMA,'source':key,'source_sha256':digest(raw),'files':{}}
    for name,value in files.items():
        target=game/name
        manifest['files'][name]={'sha256':digest(value),'original_sha256':digest(target.read_bytes()) if target.exists() else None}
    files['manifest.json']=json.dumps(manifest,indent=2).encode();files['project.json']=json.dumps(project,indent=2).encode()
    files['install.py']=(ROOT/'level_install.py').read_bytes()
    files.update(split_files(project,edits,files['Data/Maps/'+KEY+'.bsp']))
    files['README.txt']=b'''FDNY retail trigger proof of concept\n\nQuit the game before installing or restoring. Unzip this entire archive.\nRequires Python 3. Run from the extracted folder:\n\n  python3 install.py "/path/to/FDNY.app"\n\nThis is a dry run. To apply after checking the output:\n\n  python3 install.py "/path/to/FDNY.app" --install\n\nYou can also supply the FireFighter American Hero game folder.\nSelect Custom trigger test in the first Hose mission slot (formerly Pier Fire).\nThe slot uses rescue resources and the selected source mission. Walk into the\nnew box to see its touch hint. Existing mission triggers still operate.\nMulti-stage exits retain their original destination maps.\n\nTo restore the original catalog and remove only the added mod files:\n\n  python3 install.py "/path/to/FDNY.app" --restore\n\nBackups live in .fdny-level-edit-backup inside the game folder.\nDo not delete that backup until you restore. Restore before installing another\nexport. Installation refuses changed files rather than overwriting them.\nExport files are structurally validated; live gameplay testing is still needed.\nNew boxes are nonblocking hint volumes, not walls or new level geometry.\nDo not redistribute original BSPs, textures or other retail assets.\n'''
    files['README.txt']+=b'\nTiming: timing/custom-splits.lss contains your split names; timing/checkpoints.json\ndescribes trigger markers. Both custom ASLs are bundled. Read timing/README.txt\nfor LiveSplit setup. Gameplay validation is still required.\n'
    files['README.txt']+=('\nGame-relative files copied by install.py:\n'+'\n'.join(('ADD ' if meta['original_sha256'] is None else 'REPLACE ')+name for name,meta in manifest['files'].items())+'\n\nOnly these Data/ files are installed. timing/ and project.json stay in this export.\n').encode()
    out=io.BytesIO()
    with zipfile.ZipFile(out,'w',zipfile.ZIP_DEFLATED) as z:
        for name,value in files.items():z.writestr(name,value)
    return out.getvalue()
