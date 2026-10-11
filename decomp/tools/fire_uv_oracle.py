"""Capture original sprite constructor texture coordinates."""
import json
import fire_effects_oracle as n
n.call(0x404b50,[4357]);n.call(0x44b270,[0,1],n.OBJECT)
sprites=[]
for i in range(4):
 p=n.u32(n.OBJECT+0x60+4*i)
 sprites.append(dict(sprite=i,uv_bits=[[n.u32(p+0xd8+36*v+12),n.u32(p+0xd8+36*v+16)] for v in range(4)]))
(n.ROOT/'evidence/fire-uv-oracle.json').write_text(json.dumps(dict(provenance=dict(entry='0047F850 via 0044B270',exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),method='Original constructor; fixture allocator/null world; original RNG'),sprites=sprites),indent=2)+'\n');print(sprites)
