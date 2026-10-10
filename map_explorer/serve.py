from http.server import ThreadingHTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
import sys,os
os.chdir(Path(__file__).resolve().parent)
port=int(sys.argv[1]) if len(sys.argv)>1 else 4180
print(f'FDNY maps: http://127.0.0.1:{port}',flush=True)
ThreadingHTTPServer(('127.0.0.1',port),SimpleHTTPRequestHandler).serve_forever()
