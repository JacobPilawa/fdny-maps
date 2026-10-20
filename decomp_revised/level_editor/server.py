#!/usr/bin/env python3
"""Loopback-only editor, project persistence, compile and native playtest."""
import json
import os
import subprocess
import sys
import threading
import webbrowser
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import urlparse
from compiler import ROOT, LEVELS, compile_level, save
from asset_library import EXPLORER, catalog, source_model, texture_info

EDITOR=Path(__file__).resolve().parent
ACTOR_ASSETS=ROOT/'.local/actor_exports'
ACTOR_TYPES={'man','woman','girl','boy','dog'}

def actor_data(actor_type):
    """Return the game's extracted ACT bind pose, faces and UVs for previews."""
    if actor_type not in ACTOR_TYPES: raise ValueError('Unknown actor type')
    data=json.loads((ACTOR_ASSETS/f'vic_{actor_type}.json').read_text())
    vertices=[{'position':v.get('bindPosition',v['position']),'uv':v.get('uv',[0,0])} for v in data['vertices']]
    return {'type':actor_type,'vertices':vertices,'faces':data['lods'][0],
            'materials':data.get('materials',[]),'boundsMin':data.get('boundsMin'),
            'boundsMax':data.get('boundsMax')}

class Handler(SimpleHTTPRequestHandler):
    def __init__(self,*args,**kwargs): super().__init__(*args,directory=str(EDITOR),**kwargs)
    def reply(self,data,status=200):
        body=json.dumps(data).encode();self.send_response(status);self.send_header('Content-Type','application/json');self.send_header('Content-Length',str(len(body)));self.end_headers();self.wfile.write(body)
    def do_GET(self):
        if self.path=='/api/inventory':
            try:return self.reply(catalog())
            except (OSError,ValueError,KeyError) as exc:return self.reply({'error':str(exc)},500)
        if self.path.startswith('/api/model?'):
            query={k:v[-1] for k,v in __import__('urllib.parse',fromlist=['parse_qs']).parse_qs(urlparse(self.path).query).items()}
            try:return self.reply(source_model(query.get('map',''),int(query.get('id','-1'))))
            except (OSError,ValueError,KeyError,TypeError) as exc:return self.reply({'error':str(exc)},400)
        if self.path.startswith('/api/texture?'):
            query={k:v[-1] for k,v in __import__('urllib.parse',fromlist=['parse_qs']).parse_qs(urlparse(self.path).query).items()}
            try:
                info=texture_info(query.get('map',''),int(query.get('id','-1')))
                path=(EXPLORER/'data'/'textures'/'images'/info['file']).resolve()
                if not path.is_relative_to((EXPLORER/'data'/'textures'/'images').resolve()):raise ValueError('Invalid texture path')
                body=path.read_bytes();self.send_response(200);self.send_header('Content-Type','image/png');self.send_header('Cache-Control','public, max-age=3600');self.send_header('Content-Length',str(len(body)));self.end_headers();return self.wfile.write(body)
            except (OSError,ValueError,KeyError,TypeError) as exc:return self.reply({'error':str(exc)},404)
        if self.path.startswith('/api/actor?'):
            query={k:v[-1] for k,v in __import__('urllib.parse',fromlist=['parse_qs']).parse_qs(urlparse(self.path).query).items()}
            try:return self.reply(actor_data(query.get('type','')))
            except (OSError,ValueError,KeyError,TypeError) as exc:return self.reply({'error':str(exc)},404)
        if self.path.startswith('/api/actor-texture?'):
            query={k:v[-1] for k,v in __import__('urllib.parse',fromlist=['parse_qs']).parse_qs(urlparse(self.path).query).items()}
            try:
                actor=query.get('type','');index=int(query.get('id','-1'))
                if actor not in ACTOR_TYPES or index < 0 or index > 7:raise ValueError('Actor texture unavailable')
                path=(ACTOR_ASSETS/f'vic_{actor}_{index}.png').resolve()
                if not path.is_relative_to(ACTOR_ASSETS.resolve()) or not path.is_file():raise ValueError('Actor texture unavailable')
                body=path.read_bytes();self.send_response(200);self.send_header('Content-Type','image/png');self.send_header('Cache-Control','public, max-age=3600');self.send_header('Content-Length',str(len(body)));self.end_headers();return self.wfile.write(body)
            except (OSError,ValueError,KeyError,TypeError) as exc:return self.reply({'error':str(exc)},404)
        if self.path.startswith('/api/fire-sprite?'):
            query={k:v[-1] for k,v in __import__('urllib.parse',fromlist=['parse_qs']).parse_qs(urlparse(self.path).query).items()}
            name=query.get('name','lflame-00.bmp')
            allowed_sprites={f'lflame-{frame:02d}.bmp' for frame in range(19)}|{f'fflame{frame:04d}.bmp' for frame in range(19)}
            if name not in allowed_sprites:
                return self.reply({'error':'Unknown fire sprite'},404)
            path=(ACTOR_ASSETS/'fire-sprites'/name).resolve()
            if not path.is_relative_to((ACTOR_ASSETS/'fire-sprites').resolve()) or not path.is_file():return self.reply({'error':'Fire sprite unavailable'},404)
            body=path.read_bytes();self.send_response(200);self.send_header('Content-Type','image/bmp');self.send_header('Cache-Control','public, max-age=3600');self.send_header('Content-Length',str(len(body)));self.end_headers();return self.wfile.write(body)
        if self.path=='/api/levels':
            return self.reply([json.loads(p.read_text()) for p in sorted(LEVELS.glob('custom_*.json'))])
        return super().do_GET()
    def do_POST(self):
        # Reject cross-origin browser writes to the local editor.
        if self.headers.get('Origin') != 'http://'+self.headers.get('Host',''):
            return self.reply({'error':'Open the editor from its local URL'},403)
        if self.path not in ('/api/save','/api/test'):
            return self.reply({'error':'Unknown operation'},404)
        try:
            size=int(self.headers.get('Content-Length','0'))
            if not 0<size<=2000000: raise ValueError('Invalid project size')
            doc=json.loads(self.rfile.read(size));path=save(doc)
            if self.path=='/api/test':
                binary=ROOT/'port/spikes/training-native-prototype/.local/build-scene-viewer/fdny-scene-viewer'
                if not binary.is_file(): raise ValueError('Build the game first with Open FDNY Firefighters.command')
                log=open(ROOT/'.local/editor-playtest.log','w')
                env={**os.environ,'FDNY_ACTOR_ASSETS_DIR':str(ROOT/'.local/actor_exports')}
                subprocess.Popen([str(binary),str(compile_level(doc))],cwd=ROOT,env=env,stdout=log,stderr=log,start_new_session=True);log.close()
            self.reply({'message':'Saved and compiled '+str(path)+(' — playtest launched' if self.path.endswith('test') else ' — restart the game to refresh Rescue missions')})
        except (ValueError,KeyError,OSError,TypeError) as exc: self.reply({'error':str(exc)},400)

if __name__=='__main__':
    port=int(os.environ.get('FDNY_EDITOR_PORT','4190'))
    try: server=ThreadingHTTPServer(('127.0.0.1',port),Handler)
    except OSError as exc:
        print(f'Cannot start editor on port {port}: {exc}. Set FDNY_EDITOR_PORT to another port.');sys.exit(1)
    url=f'http://127.0.0.1:{port}/';print('FDNY Level Editor: '+url,flush=True)
    threading.Timer(.5,lambda:webbrowser.open(url)).start()
    try: server.serve_forever()
    except KeyboardInterrupt: server.server_close()
