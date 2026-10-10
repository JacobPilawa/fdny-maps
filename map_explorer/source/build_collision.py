"""Build display hulls from the copied BSP's blocking leaf-side planes.

Uses the native movement contents mask (0x43). BoundsTrigger models are
non-blocking event volumes and are excluded. Brush models stay at their
authored positions; actors, script visibility and door motion are not simulated.
The map bounds trim the unbounded exterior. Artificial trim faces are omitted.
"""
from pathlib import Path
import json
import math
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'source/tools'))
import extract_gbsp as eg

EPS = 1e-5
MASK = 0x43


def dot(a, b):
    return sum(x * y for x, y in zip(a, b))


def sub(a, b):
    return tuple(x - y for x, y in zip(a, b))


def cross(a, b):
    return (a[1]*b[2]-a[2]*b[1], a[2]*b[0]-a[0]*b[2], a[0]*b[1]-a[1]*b[0])


def unique(points):
    seen = set()
    out = []
    for p in points:
        key = tuple(round(v, 5) for v in p)
        if key not in seen:
            seen.add(key)
            out.append(p)
    return out


def box(lo, hi):
    v = [(x, y, z) for x in (lo[0], hi[0]) for y in (lo[1], hi[1]) for z in (lo[2], hi[2])]
    return [(None, [v[i] for i in ids]) for ids in
            ((0, 1, 3, 2), (4, 6, 7, 5), (0, 4, 5, 1),
             (2, 3, 7, 6), (0, 2, 6, 4), (1, 5, 7, 3))]


def clip(faces, normal, distance):
    """Clip a closed convex polyhedron to n·p <= d, retaining its real cap."""
    out, cuts = [], []
    for n, points in faces:
        polygon = []
        for a, b in zip(points, points[1:] + points[:1]):
            da, db = dot(normal, a)-distance, dot(normal, b)-distance
            if da <= EPS:
                polygon.append(a)
            if (da < -EPS and db > EPS) or (da > EPS and db < -EPS):
                t = da / (da-db)
                p = tuple(x + (y-x)*t for x, y in zip(a, b))
                polygon.append(p)
                cuts.append(p)
            elif abs(da) <= EPS:
                cuts.append(a)
        polygon = unique(polygon)
        if len(polygon) >= 3:
            out.append((n, polygon))
    cuts = unique(cuts)
    if len(cuts) >= 3:
        center = tuple(sum(p[i] for p in cuts)/len(cuts) for i in range(3))
        ref = (0, 1, 0) if abs(normal[1]) < .9 else (1, 0, 0)
        u = cross(ref, normal)
        length = math.sqrt(dot(u, u))
        u = tuple(x/length for x in u)
        v = cross(normal, u)
        cuts.sort(key=lambda p: math.atan2(dot(sub(p, center), v), dot(sub(p, center), u)))
        out.append((normal, cuts))
    return out


def split_polygon(points, n, d):
    distances = [dot(n, p)-d for p in points]
    if max(distances) <= EPS:
        return points, []
    if min(distances) >= -EPS:
        return [], points
    inside, outside = [], []
    for i, a in enumerate(points):
        b = points[(i+1) % len(points)]
        da, db = distances[i], distances[(i+1) % len(points)]
        if da <= EPS:
            inside.append(a)
        if da >= -EPS:
            outside.append(a)
        if (da < -EPS and db > EPS) or (da > EPS and db < -EPS):
            t = da/(da-db)
            p = tuple(x+(y-x)*t for x, y in zip(a, b))
            inside.append(p)
            outside.append(p)
    return unique(inside), unique(outside)


def exposed_parts(points, normal, root, nodes, leafs, sides, planes):
    """Keep only boundary patches whose outward side is outside the solid union.

    BSP partitioning and convex subtraction preserve partial overlaps, rather
    than guessing from face centers. The tiny outward probe is restored before
    export; collision vertices are never inflated.
    """
    offset = tuple(v*.01 for v in normal)
    shifted = [tuple(v+o for v, o in zip(p, offset)) for p in points]
    stack, result = [(root, shifted)], []
    while stack:
        node_id, polygon = stack.pop()
        if len(polygon) < 3:
            continue
        if node_id >= 0:
            node = nodes[node_id]
            plane = planes[node[4]]
            back, front = split_polygon(polygon, plane['normal'], plane['dist'])
            if back:
                stack.append((node[1], back))
            if front:
                stack.append((node[0], front))
            continue
        leaf = leafs[-node_id-1]
        if not leaf[0] & MASK or not leaf[-1]:
            result.append(polygon)
            continue
        # A BSP partition can include empty space outside its collision hull.
        inside = polygon
        for plane_id, side in sides[leaf[-2]:leaf[-2]+leaf[-1]]:
            plane = planes[plane_id]
            sign = -1 if side else 1
            inside, outside = split_polygon(inside, tuple(v*sign for v in plane['normal']), plane['dist']*sign)
            if len(outside) >= 3:
                result.append(outside)
            if len(inside) < 3:
                break
    return [[tuple(v-o for v, o in zip(p, offset)) for p in polygon] for polygon in result]


def build(entry):
    key = entry['key']
    _, _, p = eg.read_chunks(ROOT / 'source/maps' / (entry['stem'] + '.bsp'))
    models, planes = eg.parse_models(p[1]), eg.parse_planes(p[10])
    entities = eg.parse_entities(p[16])[1]
    model_names = {e['%name%']: int(e['Model']) for e in entities if e.get('classname') == '%Model%' and 'Model' in e}
    excluded = {model_names[e['C_Model']] for e in entities if e.get('classname') == 'RCS_BoundsTrigger' and e.get('C_Model') in model_names}
    bounds = json.loads((ROOT / 'data' / (key + '.json')).read_text())['bounds']
    display_lo = [bounds['min'+axis] for axis in 'XYZ']
    display_hi = [bounds['max'+axis] for axis in 'XYZ']
    leafs = [struct.unpack_from('<i6f8i', p[4], i) for i in range(0, len(p[4]), 60)]
    sides = [struct.unpack_from('<ii', p[8], i) for i in range(0, len(p[8]), 8)]
    nodes = [struct.unpack_from('<5i', p[2], i) for i in range(0, len(p[2]), 44)]
    polygons = {}
    solids = 0
    for model_id, model in enumerate(models):
        if model_id in excluded:
            continue
        for leaf_id in range(model['first_leaf'], model['first_leaf'] + model['num_leafs']):
            leaf = leafs[leaf_id]
            contents, first_side, num_sides = leaf[0], leaf[-2], leaf[-1]
            if not contents & MASK or not num_sides:
                continue
            lo = [max(leaf[1+i]-.1, display_lo[i]) for i in range(3)]
            hi = [min(leaf[4+i]+.1, display_hi[i]) for i in range(3)]
            if any(a >= b for a, b in zip(lo, hi)):
                continue
            hull = box(lo, hi)
            hull_planes = []
            for plane_id, side in sides[first_side:first_side+num_sides]:
                plane = planes[plane_id]
                sign = -1 if side else 1
                normal = tuple(v*sign for v in plane['normal'])
                distance = plane['dist']*sign
                hull_planes.append((normal, distance))
                hull = clip(hull, normal, distance)
                if not hull:
                    break
            if not hull:
                continue
            solids += 1
            for n, points in hull:
                if n is None:
                    continue  # No invented walls at the display crop.
                area = cross(sub(points[1], points[0]), sub(points[2], points[0]))
                if dot(area, area) < 1e-8:
                    continue
                # Validate geometry against every authored collision half-space.
                assert all(dot(nn, v)-dd < .002 for nn, dd in hull_planes for v in points), (key, leaf_id)
                for patch in exposed_parts(points, n, model['root_node'][0], nodes, leafs, sides, planes):
                    if len(patch) < 3:
                        continue
                    rounded = [[round(v, 3) for v in point] for point in patch]
                    signature = (model_id, tuple(sorted(tuple(point) for point in rounded)))
                    polygons[signature] = {'p': rounded, 'n': [round(v, 6) for v in n], 'model': model_id, 'leaf': leaf_id, 'contents': contents}
    output = {'key': key, 'contents_mask': MASK, 'solid_leaves': solids,
              'excluded_trigger_models': sorted(excluded), 'bounds': bounds,
              'polygons': [v for v in polygons.values() if v is not False]}
    path = ROOT / 'data/collision' / (key + '.json')
    path.write_text(json.dumps(output, separators=(',', ':')))
    print(key, solids, len(output['polygons']), path.stat().st_size, flush=True)


if __name__ == '__main__':
    (ROOT / 'data/collision').mkdir(exist_ok=True)
    for entry in json.loads((ROOT / 'source/manifest.json').read_text()):
        build(entry)
