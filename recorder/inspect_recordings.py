#!/usr/bin/env python3
"""Summarize FDNY recordings; optionally export an independent top-down SVG.

No dependencies, game attachment, or changes to the map explorer.
"""
import argparse
import html
import json
import math
from pathlib import Path

ROOT = Path.cwd()


def read_trace(path):
    records = []
    incomplete = False
    with path.open() as stream:
        for line_number, line in enumerate(stream, 1):
            if not line.strip():
                continue
            try:
                record = json.loads(line)
            except json.JSONDecodeError:
                if not line.endswith('\n'):
                    incomplete = True
                    break
                raise ValueError(f'{path.name}:{line_number}: invalid JSON')
            records.append(record)
    starts = [r for r in records if r.get('type') == 'start']
    if len(starts) != 1 or starts[0].get('schema') != 'fdny-trajectory-v1':
        raise ValueError(f'{path.name}: expected one fdny-trajectory-v1 start record')
    samples = [r for r in records if r.get('type') == 'sample']
    for i, sample in enumerate(samples):
        if sample['n'] != i:
            raise ValueError(f'{path.name}: missing/out-of-order sample {i}')
        if len(sample['position']) != 3 or not all(math.isfinite(v) for v in sample['position']):
            raise ValueError(f'{path.name}: invalid position in sample {i}')
        if i and sample['t_ms'] < samples[i - 1]['t_ms']:
            raise ValueError(f'{path.name}: timestamps out of order')
    end = next((r for r in reversed(records) if r.get('type') == 'end'), None)
    if end and end['samples'] != len(samples):
        raise ValueError(f'{path.name}: end sample count differs')
    return starts[0], samples, end, incomplete


def svg_trace(path, start, samples, output):
    if not samples:
        raise ValueError('No position samples to plot')
    points = [s['position'] for s in samples]
    xmin, xmax = min(p[0] for p in points), max(p[0] for p in points)
    zmin, zmax = min(p[2] for p in points), max(p[2] for p in points)
    scale = min(960 / max(xmax - xmin, 100), 660 / max(zmax - zmin, 100))
    cx, cz = (xmin + xmax) / 2, (zmin + zmax) / 2
    def xy(p):
        return 550 + (p[0] - cx) * scale, 430 - (p[2] - cz) * scale
    coords = ' '.join(f'{x:.2f},{y:.2f}' for x, y in map(xy, points))
    sx, sy = xy(points[0]); ex, ey = xy(points[-1])
    title = html.escape(f"{start['stage']} — {start['started_utc']}")
    content = f'''<svg xmlns="http://www.w3.org/2000/svg" width="1100" height="850" viewBox="0 0 1100 850">
<rect width="1100" height="850" fill="#111a25"/>
<g font-family="system-ui,sans-serif" fill="#e8eef5">
<text x="44" y="44" font-size="23">{title}</text>
<text x="44" y="74" font-size="14">Top-down trajectory · X right, Z up · Y height retained in JSONL · no map overlay</text>
<polyline points="{coords}" fill="none" stroke="#55c4fa" stroke-width="2.5" stroke-linejoin="round"/>
<circle cx="{sx}" cy="{sy}" r="7" fill="#61df9b"/>
<circle cx="{ex}" cy="{ey}" r="5" fill="#ffc66b"/>
<text x="44" y="803" font-size="14">Green: start · Amber: last sample · {len(samples)} samples · {samples[-1]['t_ms']/1000:.2f} seconds</text>
</g></svg>'''
    output.write_text(content)
    print(f'Plot: {output.resolve()}')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('files', nargs='*', type=Path)
    parser.add_argument('--plot', type=Path, help='Write SVG (one recording only)')
    args = parser.parse_args()
    paths = args.files or sorted((ROOT / 'Recordings').glob('*.jsonl'))
    if args.plot and len(paths) != 1:
        parser.error('--plot requires exactly one recording file')
    if not paths:
        print('No recordings yet. Launch FDNY normally and enter a stage.')
    for path in paths:
        start, samples, end, incomplete = read_trace(path)
        distance = sum(math.dist(a['position'], b['position']) for a, b in zip(samples, samples[1:]))
        duration = samples[-1]['t_ms'] / 1000 if samples else 0
        status = end['reason'] if end else 'open or interrupted (complete lines remain usable)'
        print(f'\n{path.name}\n  {start["stage"]} · {start["started_utc"]} · {len(samples)} samples · {duration:.2f}s · {distance:.1f} units traveled\n  {status}')
        if samples:
            print(f'  First position: {samples[0]["position"]}\n  Last position:  {samples[-1]["position"]}')
        with path.open() as stream:
            victim_ids = set()
            for line in stream:
                try:
                    record = json.loads(line)
                except json.JSONDecodeError:
                    continue
                if record.get('type') == 'victim_sample':
                    victim_ids.add(record['id'])
        if victim_ids:
            print(f'  Victim tracks: {len(victim_ids)}')
        if incomplete:
            print('  Ignored unfinished final JSON line.')
        if args.plot:
            svg_trace(path, start, samples, args.plot)


if __name__ == '__main__':
    main()
