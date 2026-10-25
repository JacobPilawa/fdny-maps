"""Local static viewer. Checkpoint export runs entirely in the browser."""
from http.server import ThreadingHTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
import os,sys
os.chdir(Path(__file__).resolve().parent)
if __name__=='__main__':
    port=int(sys.argv[1]) if len(sys.argv)>1 else 4180
    print(f'FDNY maps: http://127.0.0.1:{port}',flush=True)
    ThreadingHTTPServer(('127.0.0.1',port),SimpleHTTPRequestHandler).serve_forever()
