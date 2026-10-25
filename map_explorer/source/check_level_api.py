"""Check the local HTTP export API; no installed game writes."""
import threading,urllib.request,urllib.error,json,io,zipfile,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from http.server import ThreadingHTTPServer
from serve import Handler
server=ThreadingHTTPServer(('127.0.0.1',0),Handler);thread=threading.Thread(target=server.serve_forever,daemon=True);thread.start();base='http://127.0.0.1:'+str(server.server_port)
try:
    missions=json.load(urllib.request.urlopen(base+'/api/level-edit/missions'));assert len(missions['missions'])==8
    info=json.load(urllib.request.urlopen(base+'/api/level-edit/mission?key=training'))
    project={'schema':'fdny-level-edit-v1','source':'training','source_sha256':info['source_sha256'],'triggers':[{'name':'http_hint','center':info['spawn'],'size':[120,160,120],'message':'HTTP export test','seconds':5}]}
    request=urllib.request.Request(base+'/api/level-edit/export',data=json.dumps(project).encode(),headers={'Content-Type':'application/json'},method='POST')
    with urllib.request.urlopen(request) as response:
        assert response.headers['Content-Type']=='application/zip';z=zipfile.ZipFile(io.BytesIO(response.read()));assert {'Data/Maps/fdny_edit.bsp','FDNY_Custom_ILs.asl','custom-splits.lss','README.txt'}<=set(z.namelist());assert not any(name.endswith(('.py','.json','.lev')) for name in z.namelist())
    for headers in [{'Origin':'https://example.com'},{'Host':'example.com'},{'Sec-Fetch-Site':'cross-site'}]:
        try:urllib.request.urlopen(urllib.request.Request(base+'/api/level-edit/missions',headers=headers));raise AssertionError('Cross-site request accepted')
        except urllib.error.HTTPError as e:assert e.code==403
    print('PASS: local missions, source description, ZIP export, and cross-site request rejection')
finally:server.shutdown();server.server_close()
