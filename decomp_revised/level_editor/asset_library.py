"""Read-only adapter for the copied FDNY map explorer's decoded asset library."""
import json
from collections import defaultdict
from functools import lru_cache
from pathlib import Path

EXPLORER = Path(__file__).resolve().parents[2] / 'map_explorer'
EXPORTS = Path(__file__).resolve().parents[1] / '.local/exports'


def _read(path):
    return json.loads(path.read_text(encoding='utf-8'))

def _number(value, fallback):
    try:return float(value)
    except (TypeError,ValueError):return fallback


@lru_cache(maxsize=1)
def catalog():
    maps = _read(EXPLORER/'source/manifest.json')
    result={'maps':[],'models':[],'actors':[],'textures':[],'types':['OBJECT','DOOR','TRIGGER','EXIT','VALVE','FIRE','VICTIM','HOSE','REFILL','START','ALT','EMITTER','BONUS']}
    for row in maps:
        key=row['key']; data_path=EXPLORER/'data'/f'{key}.json'
        if not data_path.is_file(): continue
        data=_read(data_path)
        result['maps'].append({'key':key,'name':data.get('name',key.replace('_',' ').title()),'modelCount':len(data.get('models',[]))})
        model_markers=defaultdict(list)
        for marker in data.get('markers',[]):
            for model_id in marker.get('models',[]) or []:
                model_markers[int(model_id)].append({'category':marker.get('category','OBJECT'),'name':marker.get('name',''),'label':marker.get('label',''),'class':marker.get('class',''),'entity':marker.get('entity_index')})
        tri_counts=defaultdict(int)
        for poly in data.get('polygons',[]):
            mid=int(poly.get('model',0))
            if mid: tri_counts[mid]+=max(0,len(poly.get('p',[]))-2)
        for marker in data.get('markers',[]):
            raw=marker.get('raw',{})
            if marker.get('class')=='RCS_VictimRescue':
                roam=0
                for value in raw.values():
                    match=__import__('re').fullmatch(r'\s*roam\s+([0-9.]+)\s*',value or '',__import__('re').I)
                    if match:roam=float(match.group(1));break
                result['actors'].append({'id':f'{key}:victim:{marker.get("entity_index")}','map':key,'mapName':result['maps'][-1]['name'],'kind':'survivor','label':marker.get('name') or marker.get('label') or 'Survivor','actor':raw.get('D_Type','man'),'roam':roam,'radius':60,'height':100})
            elif marker.get('class')=='RCS_FireRescue':
                try:radius=float(raw.get('H_Radius','50'));height=float(raw.get('H_Height','100'))
                except ValueError:radius,height=50,100
                result['actors'].append({'id':f'{key}:fire:{marker.get("entity_index")}','map':key,'mapName':result['maps'][-1]['name'],'kind':'fire','label':marker.get('name') or marker.get('label') or 'Fire','radius':radius,'height':height,'sprayTime':_number(raw.get('K_SprayTime','4'),4),'smoking':raw.get('M_Smoking','1')!='0','spread':bool(raw.get('A_Name') and raw.get('A_Name')!='<null>')})
        for model in data.get('models',[]):
            mid=int(model.get('id',0))
            if not mid or not tri_counts.get(mid): continue
            refs=model_markers[mid]
            category=next((x['category'] for x in refs if x['category'] in ('DOOR','OBJECT','TRIGGER','EXIT','VALVE')), 'BRUSH')
            label=next((x['name'] for x in refs if x['name'] and not x['name'].startswith('RCS_')), '')
            result['models'].append({'kind':'source_model','id':f'{key}:{mid}','map':key,'mapName':result['maps'][-1]['name'],'model':mid,'label':label or f'Brush model {mid}','category':category,'triangles':tri_counts[mid],'min':model.get('min',[0,0,0]),'max':model.get('max',[0,0,0]),'origin':model.get('origin',[0,0,0]),'entities':refs[:5]})
        tex_path=EXPLORER/'data'/'textures'/f'{key}.json'
        native=EXPORTS/key/'texture_manifest.json'
        if not tex_path.is_file(): continue
        texdata=_read(tex_path); native_names={str(x.get('name','')).casefold():x for x in _read(native)} if native.is_file() else {}
        for tid,info in texdata.get('textures',{}).items():
            name=info.get('name') or f'texture_{tid}'
            native_entry=native_names.get(name.casefold())
            image=EXPLORER/'data'/'textures'/'images'/info.get('file','')
            if image.is_file():
                result['textures'].append({'kind':'texture','id':f'{key}:{tid}','map':key,'mapName':result['maps'][-1]['name'],'texture':int(tid),'name':name,'file':info['file'],'nativeFile':native_entry.get('file') if native_entry else None})
    result['actors'].sort(key=lambda x:(x['kind'],x['label'].casefold(),x['mapName'].casefold()))
    result['models'].sort(key=lambda x:(x['mapName'].casefold(),x['category'],x['label'].casefold(),x['model']))
    result['textures'].sort(key=lambda x:(x['name'].casefold(),x['mapName'].casefold()))
    return result


@lru_cache(maxsize=4)
def map_data(map_key):
    return _read(EXPLORER/'data'/f'{map_key}.json')

@lru_cache(maxsize=4)
def texture_data(map_key):
    return _read(EXPLORER/'data'/'textures'/f'{map_key}.json')


@lru_cache(maxsize=256)
def source_model(map_key, model_id):
    allowed={m['key'] for m in catalog()['maps']}
    if map_key not in allowed: raise ValueError('Unknown source map')
    data=map_data(map_key)
    model=next((m for m in data['models'] if int(m['id'])==int(model_id)),None)
    if not model or int(model_id)==0: raise ValueError('Unknown source brush model')
    textures=texture_data(map_key)
    index=textures.get('polygons',[])
    materials=textures.get('textures',{})
    groups={}
    origin=model.get('origin',[0,0,0])
    for i,poly in enumerate(data.get('polygons',[])):
        if int(poly.get('model',0))!=int(model_id): continue
        points=poly.get('p',[]); tex=index[i] if i<len(index) else {}
        uv=tex.get('uv',[]); tid=str(tex.get('texture',0))
        bucket=groups.setdefault(tid,{'texture':int(tid),'textureName':materials.get(tid,{}).get('name',''),'positions':[],'uvs':[]})
        for j in range(1,len(points)-1):
            for v in (0,j,j+1):
                p=points[v]; t=uv[v] if v<len(uv) else [0,0]
                bucket['positions'].extend([float(p[k])-float(origin[k]) for k in range(3)])
                bucket['uvs'].extend([float(t[0]),float(t[1])])
    return {'map':map_key,'model':int(model_id),'origin':origin,'min':model['min'],'max':model['max'],'groups':list(groups.values())}


def texture_info(map_key, texture_id):
    item=next((t for t in catalog()['textures'] if t['id']==f'{map_key}:{texture_id}'),None)
    if not item: raise ValueError('Texture is unavailable in the native asset packages')
    return item
