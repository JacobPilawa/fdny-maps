from http.server import ThreadingHTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
from urllib.parse import urlparse,parse_qs
import json,sys,os
import level_edit
os.chdir(Path(__file__).resolve().parent)
class Handler(SimpleHTTPRequestHandler):
    def send_data(self,status,data,mime='application/json'):
        self.send_response(status);self.send_header('Content-Type',mime);self.send_header('Content-Length',str(len(data)));self.send_header('Cache-Control','no-store');self.send_header('X-Content-Type-Options','nosniff');self.end_headers();self.wfile.write(data)
    def authorized(self):
        host=self.headers.get('Host','')
        return host.split(':')[0] in ('127.0.0.1','localhost') and self.headers.get('Origin','http://'+host)=='http://'+host and self.headers.get('Sec-Fetch-Site','same-origin') in ('same-origin','none')
    def do_GET(self):
        path=urlparse(self.path)
        if not path.path.startswith('/api/level-edit'):return super().do_GET()
        if not self.authorized():return self.send_data(403,b'{}')
        try:
            game=level_edit.game_directory()
            if path.path=='/api/level-edit/missions':result={'missions':level_edit.missions(game)}
            elif path.path=='/api/level-edit/mission':result=level_edit.describe(game,parse_qs(path.query).get('key',[''])[0])
            else:return self.send_data(404,b'{}')
            self.send_data(200,json.dumps(result).encode())
        except (ValueError,OSError) as e:self.send_data(400,json.dumps({'error':str(e)}).encode())
    def do_POST(self):
        if self.path!='/api/level-edit/export':return self.send_data(404,b'{}')
        if not self.authorized() or self.headers.get('Content-Type')!='application/json':return self.send_data(403,b'{}')
        try:
            size=int(self.headers.get('Content-Length','0'))
            if not 0<size<=100000:raise ValueError('Project is too large or empty')
            project=json.loads(self.rfile.read(size));data=level_edit.export_project(level_edit.game_directory(),project)
            self.send_data(200,data,'application/zip')
        except (ValueError,OSError,KeyError,TypeError) as e:self.send_data(400,json.dumps({'error':str(e)}).encode())
if __name__=='__main__':
    port=int(sys.argv[1]) if len(sys.argv)>1 else 4180
    print(f'FDNY maps: http://127.0.0.1:{port}',flush=True)
    ThreadingHTTPServer(('127.0.0.1',port),Handler).serve_forever()
