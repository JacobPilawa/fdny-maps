"""Decode original BSP palettes/UVs into lazy-loaded, deduplicated textures."""
from pathlib import Path
import hashlib
import json
import math
import struct
import sys
import zlib

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'source/tools'))
import extract_gbsp as eg
import render_annotated_maps as ra


def chunk(name, data):
    return struct.pack('>I', len(data)) + name + data + struct.pack('>I', zlib.crc32(name+data))


def png(width, height, indices, palette):
    rows = b''.join(b'\0'+indices[y*width:(y+1)*width] for y in range(height))
    header = struct.pack('>IIBBBBB', width, height, 8, 3, 0, 0, 0)
    return b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR', header)+chunk(b'PLTE', palette)+chunk(b'IDAT', zlib.compress(rows, 9))+chunk(b'IEND', b'')


def build(entry):
    key = entry['key']
    _, _, p = eg.read_chunks(ROOT / 'source/maps' / (entry['stem'] + '.bsp'))
    models = eg.parse_models(p[1])
    faces = eg.parse_faces(p[11])
    vertices = eg.vecs(p[14])
    indices = struct.unpack('<'+'i'*(len(p[13])//4), p[13])
    info, textures = eg.parse_texinfo(p[17]), eg.parse_textures(p[18])
    entities = eg.parse_entities(p[16])[1]
    model_names = {e['%name%']: models[int(e['Model'])] for e in entities if e.get('classname') == '%Model%' and 'Model' in e}
    hidden_faces = set()
    for entity in entities:
        if entity.get('classname') == 'RCS_BoundsTrigger':
            model = model_names.get(entity.get('C_Model'))
            if model:
                hidden_faces.update(range(model['first_face'], model['first_face'] + model['num_faces']))
    expected = json.loads((ROOT / 'data' / (key+'.json')).read_text())['polygons']
    output, texture_files = [], {}
    for face_id, face in enumerate(faces):
        points = eg.face_vertices(face, indices, vertices)
        if face_id in hidden_faces or len(points) < 3:
            continue
        name = ra.face_texture(face, info, textures)
        if name.startswith(('sky_', 'col_')) or any(t in name for t in ['trigger', 'clip', 'hint', 'null']):
            continue
        polygon_id = len(output)
        assert expected[polygon_id]['p'] == [[round(v, 3) for v in point] for point in points], (key, polygon_id)
        ti = info[face['texinfo']]
        texture_id = ti['texture']
        texture = textures[texture_id]
        if texture_id not in texture_files:
            w, h = texture['width'], texture['height']
            pixels = p[19][texture['offset']:texture['offset']+w*h]
            palette = p[23][texture['palette']*768:(texture['palette']+1)*768]
            assert len(pixels) == w*h and len(palette) == 768
            encoded = png(w, h, pixels, palette)
            file = hashlib.sha256(encoded).hexdigest()[:24]+'.png'
            path = ROOT / 'data/textures/images' / file
            if not path.exists():
                path.write_bytes(encoded)
            texture_files[texture_id] = {'file': file, 'name': texture['name']}
        uv = []
        for point in points:
            u = (sum(a*b for a, b in zip(point, ti['u_vec'])) / (ti['draw_scale'][0] or 1) + ti['shift'][0]) / texture['width']
            v = 1 - (sum(a*b for a, b in zip(point, ti['v_vec'])) / (ti['draw_scale'][1] or 1) + ti['shift'][1]) / texture['height']
            assert math.isfinite(u) and math.isfinite(v)
            uv.append([round(u, 6), round(v, 6)])
        output.append({'texture': texture_id, 'uv': uv})
    assert len(output) == len(expected), key
    result = {'key': key, 'textures': texture_files, 'polygons': output}
    (ROOT / 'data/textures' / (key+'.json')).write_text(json.dumps(result, separators=(',', ':')))
    print(key, len(output), len(texture_files), flush=True)


if __name__ == '__main__':
    (ROOT / 'data/textures/images').mkdir(parents=True, exist_ok=True)
    for entry in json.loads((ROOT / 'source/manifest.json').read_text()):
        build(entry)
