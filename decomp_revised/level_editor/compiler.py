"""Compile editable projects into the native port's world mesh and BSP collision schema."""
import hashlib
import json
import math
import re
import shutil
import struct
import zlib
from pathlib import Path
from asset_library import EXPLORER, EXPORTS, catalog, source_model, texture_info

ROOT = Path(__file__).resolve().parents[1]
LEVELS = ROOT / 'level_editor/levels'
KINDS = {'floor', 'wall', 'object', 'ramp', 'source_model', 'fire', 'survivor', 'spawn'}


def validate(doc):
    if not isinstance(doc, dict) or doc.get('format') != 'fdny-editor-v1':
        raise ValueError('Unsupported level project')
    if not re.fullmatch(r'custom_[a-z0-9_]{1,48}', doc.get('key', '')):
        raise ValueError('Key must start with custom_ and use lowercase letters, digits or underscores')
    if not isinstance(doc.get('title'), str) or not 1 <= len(doc['title']) <= 48:
        raise ValueError('Title must have 1–48 characters')
    items=doc.get('items')
    if not isinstance(items,list) or not 1 <= len(items) <= 1000 or any(not isinstance(i,dict) for i in items):
        raise ValueError('A level must contain 1–1000 items')
    if sum(i.get('kind')=='spawn' for i in items)!=1: raise ValueError('Place exactly one player spawn')
    library=catalog();allowed_maps={m['key'] for m in library['maps']};allowed_models={(m['map'],m['model']) for m in library['models']};allowed_textures={t['id'] for t in catalog()['textures']}
    for item in items:
        if item.get('kind') not in KINDS:raise ValueError('Unknown item type')
        for name in ('position','size'):
            values=item.get(name)
            if not isinstance(values,list) or len(values)!=3 or any(type(v) not in (int,float) or not math.isfinite(v) or abs(v)>100000 for v in values):
                raise ValueError('Position and size require three finite coordinates within 100000')
        if any(v<1 for v in item['size']):raise ValueError('Dimensions must be at least 1 unit')
        angle=item.get('rotation',0)
        if type(angle) not in (int,float) or not math.isfinite(angle) or abs(angle)>36000:raise ValueError('Rotation must be a finite angle')
        scale=item.get('scale',[1,1,1])
        if not isinstance(scale,list) or len(scale)!=3 or any(type(v) not in (int,float) or not math.isfinite(v) or not .05<=v<=20 for v in scale):raise ValueError('Scale axes must be between .05 and 20')
        if item['kind']=='source_model':
            if item.get('sourceMap') not in allowed_maps:raise ValueError('Unknown model source map')
            model=item.get('sourceModel')
            if type(model) is not int or model<1:raise ValueError('Unknown brush model')
            if (item['sourceMap'],model) not in allowed_models:raise ValueError('Brush model is not in the decoded asset library')
        texture=item.get('texture')
        if texture and texture not in allowed_textures:raise ValueError('Selected texture is unavailable')
        if item['kind']=='survivor' and item.get('survivorType','man') not in {'man','woman','girl','boy','boy1','dog','fatman'}:raise ValueError('Unknown survivor actor type')
        if item['kind']=='survivor' and (type(item.get('roam',0)) not in (int,float) or not 0<=item.get('roam',0)<=10000):raise ValueError('Roaming radius must be between 0 and 10000')
        if item['kind']=='fire':
            if type(item.get('radius',item['size'][0]/2)) not in (int,float) or not 1<=item.get('radius',item['size'][0]/2)<=10000:raise ValueError('Fire radius must be between 1 and 10000')
            if type(item.get('fireHeight',item['size'][1])) not in (int,float) or not 1<=item.get('fireHeight',item['size'][1])<=10000:raise ValueError('Fire height must be between 1 and 10000')
            if type(item.get('strength',1)) not in (int,float) or not math.isfinite(item.get('strength',1)) or not 0<=item.get('strength',1)<=1:raise ValueError('Initial fire strength must be between 0 and 1')
    return doc


def brush(item):
    w,h,d=item['size'];verts=[[-w/2,0,-d/2],[w/2,0,-d/2],[w/2,0,d/2],[-w/2,0,d/2],[-w/2,h,-d/2],[w/2,h,-d/2],[w/2,h,d/2],[-w/2,h,d/2]]
    faces=[[0,3,2,1],[0,1,5,4],[1,2,6,5],[2,3,7,6],[3,0,4,7],[4,5,6,7]]
    if item['kind']=='ramp':verts[4][1]=verts[5][1]=0;faces=[[0,3,2,1],[1,2,6],[2,3,7,6],[3,0,7],[0,1,6,7]]
    a=math.radians(item.get('rotation',0));c,s=math.cos(a),math.sin(a);x,y,z=item['position']
    verts=[[x+c*u+s*t,y+v,z-s*u+c*t] for u,v,t in verts];return verts,faces


def rotate_point(v,angle):
    a=math.radians(angle);c,s=math.cos(a),math.sin(a);return [c*v[0]+s*v[2],v[1],-s*v[0]+c*v[2]]


def png_to_bmp(source,target):
    """Convert map-explorer 8-bit PNG textures to BMP using Python's stdlib."""
    raw=source.read_bytes()
    if raw[:8]!=b"\x89PNG\r\n\x1a\n":raise ValueError(f"Not a PNG texture: {source}")
    offset=8;compressed=bytearray();palette=b"";transparency=b"";width=height=depth=ctype=interlace=None
    while offset+12<=len(raw):
        size=struct.unpack_from('>I',raw,offset)[0];kind=raw[offset+4:offset+8];data=raw[offset+8:offset+8+size];offset+=size+12
        if kind==b'IHDR':width,height,depth,ctype,_,_,interlace=struct.unpack('>IIBBBBB',data)
        elif kind==b'IDAT':compressed.extend(data)
        elif kind==b'PLTE':palette=data
        elif kind==b'tRNS':transparency=data
        elif kind==b'IEND':break
    if depth!=8 or interlace!=0 or ctype not in (0,2,3,4,6):raise ValueError(f"Unsupported PNG texture format in {source}")
    channels={0:1,2:3,3:1,4:2,6:4}[ctype];stride=width*channels;pixels=zlib.decompress(compressed)
    if len(pixels)!=(stride+1)*height:raise ValueError(f"Invalid PNG image data in {source}")
    rows=[];prior=bytearray(stride);offset=0
    for _ in range(height):
        mode=pixels[offset];offset+=1;scan=bytearray(pixels[offset:offset+stride]);offset+=stride
        for i in range(stride):
            a=scan[i-channels] if i>=channels else 0;b=prior[i];c=prior[i-channels] if i>=channels else 0
            if mode==1:scan[i]=(scan[i]+a)&255
            elif mode==2:scan[i]=(scan[i]+b)&255
            elif mode==3:scan[i]=(scan[i]+((a+b)//2))&255
            elif mode==4:
                p=a+b-c;pa=abs(p-a);pb=abs(p-b);pc=abs(p-c);scan[i]=(scan[i]+(a if pa<=pb and pa<=pc else b if pb<=pc else c))&255
            elif mode!=0:raise ValueError(f"Unsupported PNG filter {mode} in {source}")
        rows.append(scan);prior=scan
    output=bytearray();pad=(-width*3)%4
    for row in reversed(rows):
        for x in range(width):
            if ctype==3:
                idx=row[x];base=idx*3
                if base+2>=len(palette):raise ValueError(f"Invalid PNG palette in {source}")
                r,g,b=palette[base:base+3]
            elif ctype==0:r=g=b=row[x]
            elif ctype==2:r,g,b=row[x*3:x*3+3]
            elif ctype==4:r=g=b=row[x*2]
            else:r,g,b=row[x*4:x*4+3]
            output.extend((b,g,r))
        output.extend(b'\x00'*pad)
    size=54+len(output);header=b'BM'+struct.pack('<IHHI',size,0,0,54)+struct.pack('<IiiHHIIiiII',40,width,height,1,24,0,len(output),2835,2835,0,0)
    target.write_bytes(header+output)


def copy_texture(out,ref,dest):
    info=texture_info(ref.split(':',1)[0],int(ref.split(':',1)[1]))
    source=EXPORTS/info['map']/'textures'/info['nativeFile'] if info.get('nativeFile') else None
    target=out/'textures'/dest;target.parent.mkdir(parents=True,exist_ok=True)
    if source and source.is_file():shutil.copy2(source,target)
    else:png_to_bmp(((EXPLORER / 'data' / 'textures' / 'images')/info['file']),target)
    return 'textures/'+dest,info['name']


def compile_level(doc):
    validate(doc);key=doc['key'];out=EXPORTS/key;out.mkdir(parents=True,exist_ok=True);(out/'textures').mkdir(exist_ok=True)
    collision={'contents_mask':67,'models':[],'nodes':[],'leafs':[{'contents':0,'num_sides':0,'first_side':0}],'leaf_sides':[],'planes':[]};groups=[];entities=[]
    def entity(cls,props):
        props={'classname':cls,**props};entities.append({'index':len(entities),'class':cls,'name':props.get('A_Name',''),'properties':props,'epairs':[{'key':k,'value':str(v)} for k,v in props.items()]})
    def add_hull(verts,faces,model_index):
        center=[sum(v[k] for v in verts)/len(verts) for k in range(3)];planes=[]
        for face in faces:
            p,q,r=[verts[i] for i in face[:3]];u=[q[k]-p[k] for k in range(3)];v=[r[k]-p[k] for k in range(3)]
            n=[u[1]*v[2]-u[2]*v[1],u[2]*v[0]-u[0]*v[2],u[0]*v[1]-u[1]*v[0]];length=math.sqrt(sum(x*x for x in n))
            if length<1e-7:continue
            n=[x/length for x in n]
            if sum(n[k]*(center[k]-p[k]) for k in range(3))>0:n=[-x for x in n]
            planes.append((n,sum(n[k]*p[k] for k in range(3))))
        root=len(collision['nodes']);leaf=len(collision['leafs']);first=len(collision['leaf_sides']);collision['leafs'].append({'contents':1,'num_sides':len(planes),'first_side':first})
        for j,(n,d) in enumerate(planes):
            pi=len(collision['planes']);collision['planes'].append({'normal':n,'distance':d,'type':3});collision['leaf_sides'].append({'plane':pi,'side':0});collision['nodes'].append({'plane':pi,'children':[-1,root+j+1 if j+1<len(planes) else -leaf-1]})
        collision['models'].append({'index':model_index,'root':root,'origin':[0,0,0],'mins':[min(v[k] for v in verts) for k in range(3)],'maxs':[max(v[k] for v in verts) for k in range(3)]})
    def add_mesh(model,positions,uvs,texture_ref,tag):
        dest,name=copy_texture(out,texture_ref,tag+'.bmp')
        groups.append({'model':model,'texture':name,'texture_file':dest,'positions':positions,'uvs':uvs})
    for item_index,item in enumerate(doc['items']):
        kind=item['kind'];pos=item['position'];loc=' '.join(str(v) for v in pos);angle=float(item.get('rotation',0));scale=item.get('scale',[1,1,1]);override=item.get('texture')
        if kind=='spawn':entity('RCS_PlayerRescue',{'ZZ_location':loc,'C_Heading':str(angle),'S_Script01':'equip axe','S_Script02':'equip extinguisher','S_Script03':'equip goggles','S_Script04':'setashdensity 0','S_Script05':'setrescuebot 0'})
        elif kind=='fire':
            spread=bool(item.get('spread',False));entity('RCS_FireRescue',{'A_Name':f'editor_fire_{item_index}' if spread else '', 'ZZ_location':loc,'H_Radius':str(item.get('radius',item['size'][0]/2)),'H_Height':str(item.get('fireHeight',item['size'][1])),'K_SprayTime':str(item.get('sprayTime',4)),'L_FloorMe':'1' if item.get('floorMe',True) else '0','M_Smoking':'1' if item.get('smoking',True) else '0','FDNY_InitialStrength':str(item.get('strength',1)),'N_IsAlive':'1'})
        elif kind=='survivor':
            props={'A_Name':f'editor_survivor_{item_index}','location':loc,'D_Type':item.get('survivorType','man'),'G_Heading':str(angle/30),'J_Radius':str(item.get('pickupRadius',60)),'%name%':f'editor_survivor_{item_index}'}
            if item.get('roam',0):props['S_Script01']=f"roam {float(item['roam']):g}"
            entity('RCS_VictimRescue',props)
        else:
            if kind=='source_model':
                source=source_model(item['sourceMap'],item['sourceModel']);origin=source['origin'];mins=source['min'];maxs=source['max'];positions=[];uvs=[]
                center=[(mins[k]+maxs[k])/2-origin[k] for k in range(3)];center=[center[k]*scale[k] for k in range(3)];offset=rotate_point(center,angle);hull_center=[pos[k]+offset[k] for k in range(3)]
                size=[(maxs[k]-mins[k])*scale[k] for k in range(3)];hull_bottom=[hull_center[0],hull_center[1]-size[1]/2,hull_center[2]];temp={'kind':'object','position':hull_bottom,'size':size,'rotation':angle};hull,faces=brush(temp);model_index=len(collision['models']);add_hull(hull,faces,model_index)
                for group in source['groups']:
                    for i in range(0,len(group['positions']),3):
                        local=[group['positions'][i+k]*scale[k] for k in range(3)];turned=rotate_point(local,angle);positions.extend([pos[k]+turned[k] for k in range(3)])
                    if override:ref=override
                    else:
                        candidates=[t['id'] for t in catalog()['textures'] if t['map']==item['sourceMap'] and t['texture']==group['texture']]
                        ref=candidates[0] if candidates else next(t['id'] for t in catalog()['textures'] if t['map']==item['sourceMap'])
                    add_mesh(model_index,positions,group['uvs'],ref,f'item_{item_index}_tex_{group["texture"]}')
                    positions=[]
            else:
                verts,faces=brush(item);model_index=len(collision['models']);add_hull(verts,faces,model_index)
                default={'floor':'training:0','wall':'training:8','object':'training:3','ramp':'training:0'}[kind];ref=override or default
                positions=[];uvs=[]
                for face in faces:
                    for j in range(1,len(face)-1):
                        for index in (face[0],face[j],face[j+1]):
                            v=verts[index];positions.extend(v);uvs.extend([v[0]/128+v[2]/256,v[1]/128+v[2]/128])
                add_mesh(model_index,positions,uvs,ref,f'item_{item_index}_material')
    if not groups:raise ValueError('Place at least one floor, wall, ramp, object, or brush model')
    docs={'report':{'format':'fdny-gbsp-v15-scene-v2','source_file':key+'.editor.json','source_sha256':hashlib.sha256(json.dumps(doc,sort_keys=True).encode()).hexdigest(),'authored_custom_level':True},'geometry':{},'world_mesh':{'groups':groups},'collision':collision,'entities':entities,'texture_manifest':[]}
    for name,data in docs.items():(out/(name+'.json')).write_text(json.dumps(data)+'\n')
    return out


def save(doc):
    compile_level(doc);LEVELS.mkdir(parents=True,exist_ok=True);path=LEVELS/(doc['key']+'.json');tmp=path.with_suffix('.tmp');tmp.write_text(json.dumps(doc,indent=2)+'\n');tmp.replace(path);install();return path


def install():
    resources=ROOT/'.local/FDNY Firefighters.app/Contents/Resources'
    if not resources.is_dir():return
    entries=[]
    for path in sorted(LEVELS.glob('custom_*.json')):
        doc=validate(json.loads(path.read_text()));package=compile_level(doc);shutil.copytree(package,resources/'scenes'/doc['key'],dirs_exist_ok=True);entries.append({'key':doc['key'],'title':'CUSTOM: '+doc['title']})
    manifest=resources/'custom_missions.json';tmp=manifest.with_suffix('.tmp');tmp.write_text(json.dumps({'entries':entries},indent=2));tmp.replace(manifest)

if __name__=='__main__':
    import sys
    if len(sys.argv)>1:save(json.loads(Path(sys.argv[1]).read_text()))
    else:install()
