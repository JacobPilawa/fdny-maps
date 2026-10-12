#!/usr/bin/env python3
"""Re-embed the maintained viewer snapshot and refresh native file sizes."""
import json
from pathlib import Path
root = Path(__file__).resolve().parents[1]
metadata = root / 'viewer-progress.json'
data = json.loads(metadata.read_text())
known = {f['path'] for f in data['files']}
for source in sorted((root / 'src').glob('*')):
    if source.suffix not in ('.h', '.cpp'):
        continue
    path = str(source.relative_to(root))
    if path not in known:
        data['files'].append(dict(path=path, group='Inventory', summary='Original-function coverage not classified in this viewer.', evidence='CHECKLIST.md', units=[['UNMAPPED', source.name, 'open', 'Inventory entry awaiting classification.']]))
for entry in data['files']:
    source = root / entry['path']
    entry['bytes'] = source.stat().st_size if source.is_file() else 0
serialized = json.dumps(data, indent=2)
metadata.write_text(serialized + '\n')
page = root / 'training-progress.html'
html = page.read_text()
start = html.index('const data=') + len('const data=')
end = html.index(';\nconst names=', start)
page.write_text(html[:start] + serialized + html[end:])
print(f"Refreshed {len(data['files'])} viewer entries. Statuses are maintained in viewer-progress.json.")
