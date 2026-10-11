"""Run shipped Genesis.dll geWorld_Collision against Training's original BSP data."""
import hashlib
import json
import math
import random
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools/runtime-deps'))
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UC_HOOK_MEM_INVALID
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EAX, UC_X86_REG_EIP, UC_X86_REG_FPCW

DLL = ROOT / 'evidence/original/Genesis.dll'
ENTRY = 0x10001E30
STATE = 0x20000000
STACK = 0x20FFF000
RETURN = 0x70000100
STUBS = 0x70001000


def f32(value):
    return struct.unpack('<f', struct.pack('<f', value))[0]


class GenesisCollisionOracle:
    def __init__(self, collision):
        self.raw = DLL.read_bytes()
        self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        pe = struct.unpack_from('<I', self.raw, 0x3c)[0]
        opt = pe + 24
        self.base = struct.unpack_from('<I', self.raw, opt + 28)[0]
        image_size = struct.unpack_from('<I', self.raw, opt + 56)[0]
        headers = struct.unpack_from('<I', self.raw, opt + 60)[0]
        self.uc.mem_map(self.base, (image_size + 4095) & ~4095)
        self.uc.mem_write(self.base, self.raw[:headers])
        sections = opt + struct.unpack_from('<H', self.raw, pe + 20)[0]
        for index in range(struct.unpack_from('<H', self.raw, pe + 6)[0]):
            _, rva, size, offset = struct.unpack_from('<IIII', self.raw, sections + index * 40 + 8)
            self.uc.mem_write(self.base + rva, self.raw[offset:offset + size])
        self.uc.mem_map(STATE, 0x1000000)
        self.uc.mem_map(0x70000000, 0x10000)
        self.uc.mem_write(RETURN, b'\xf4')
        self.uc.reg_write(UC_X86_REG_FPCW, 0x037f)
        self.imports = {}
        self._replace_imports(opt)
        self.uc.hook_add(UC_HOOK_CODE, self._hook)
        self.uc.hook_add(UC_HOOK_MEM_INVALID, self._invalid_memory)
        self.next = STATE + 0x20000
        self.world = STATE
        self.bsp = STATE + 0x1000
        self.uc.mem_write(self.world + 24, struct.pack('<I', self.bsp))
        self._build_bsp(collision)
        self.io = STATE + 0xF00000

    def _u32(self, address):
        return struct.unpack('<I', self.uc.mem_read(address, 4))[0]

    def _cstring(self, address):
        value = bytearray()
        while (ch := self.uc.mem_read(address, 1)[0]) != 0:
            value.append(ch)
            address += 1
        return value.decode('ascii')

    def _replace_imports(self, opt):
        descriptor = self.base + struct.unpack_from('<I', self.raw, opt + 104)[0]
        stub_index = 0
        while True:
            original, _, _, _, first = struct.unpack('<IIIII', self.uc.mem_read(descriptor, 20))
            if not original and not first:
                break
            slot = 0
            while (item := self._u32(self.base + (original or first) + slot * 4)) != 0:
                name = f'ordinal_{item & 0xffff}' if item & 0x80000000 else self._cstring(self.base + item + 2)
                stub = STUBS + stub_index * 16
                self.uc.mem_write(stub, b'\xcc')
                self.uc.mem_write(self.base + first + slot * 4, struct.pack('<I', stub))
                self.imports[stub] = name
                stub_index += 1
                slot += 1
            descriptor += 20

    def _hook(self, uc, address, size, _):
        # This is the shipped IntersectLeafSides_r branch that commits a new
        # GlobalI/GlobalPlane contact. LeafHit can also be true without taking
        # it, in which case Genesis returns retained global contact data.
        if address == 0x10036633:
            self.recorded_contact = True
        if address in self.imports:
            raise RuntimeError(f'Unexpected imported call: {self.imports[address]}')

    def _invalid_memory(self, uc, access, address, size, value, _):
        eip = uc.reg_read(UC_X86_REG_EIP)
        self.invalid = (access, address, size, eip)
        return False

    def _alloc(self, payload):
        address = self.next
        self.uc.mem_write(address, bytes(payload))
        self.next = (address + len(payload) + 15) & ~15
        return address

    def _build_bsp(self, data):
        models_blob = bytearray()
        for model in data['models']:
            origin = model.get('origin', [0, 0, 0])
            models_blob += struct.pack('<ii18f', model['root'], -1, *model['mins'], *model['maxs'], *origin, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        models = self._alloc(models_blob)
        nodes = self._alloc(b''.join(struct.pack('<5i6f', *node['children'], node.get('num_faces', 0), node.get('first_face', 0), node['plane'], *node.get('mins', [0, 0, 0]), *node.get('maxs', [0, 0, 0])) for node in data['nodes']))
        leafs = self._alloc(b''.join(struct.pack('<i6f8i', leaf['contents'], *leaf.get('mins', [0, 0, 0]), *leaf.get('maxs', [0, 0, 0]), leaf.get('first_face', 0), leaf.get('num_faces', 0), leaf.get('first_portal', 0), leaf.get('num_portals', 0), leaf.get('cluster', 0), leaf.get('area', 0), leaf['first_side'], leaf['num_sides']) for leaf in data['leafs']))
        sides = self._alloc(b''.join(struct.pack('<ii', side['plane'], side['side']) for side in data['leaf_sides']))
        planes = self._alloc(b''.join(struct.pack('<4fi', *plane['normal'], plane['distance'], plane['type']) for plane in data['planes']))
        bsp_data = self.bsp + 200
        # The shipped DLL's GBSP_Header is 32 bytes (four bytes larger than
        # the public-source compiler layout), shifting subsequent fields.
        for offset, pointer in ((72, models), (76, nodes), (88, leafs), (104, planes), (116, sides)):
            self.uc.mem_write(bsp_data + offset, struct.pack('<I', pointer))
        counts = [len(data['models']), len(data['nodes']), 0, len(data['leafs']), 0, 0, 0, len(data['planes']), 0, 0, len(data['leaf_sides'])] + [0] * 11
        self.uc.mem_write(bsp_data + 164, struct.pack('<22i', *counts))

        runtime_models = self._alloc(bytes(212 * len(data['models'])))
        self.runtime_models = runtime_models
        self.uc.mem_write(self.bsp + 464, struct.pack('<I', runtime_models))
        identity = [1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0]
        for index, model in enumerate(data['models']):
            address = runtime_models + index * 212
            origin = model.get('origin', [0, 0, 0])
            center = [(model['mins'][axis] + model['maxs'][axis]) * 0.5 for axis in range(3)]
            payload = bytearray(212)
            struct.pack_into('<i12f18f', payload, 64, index, *identity, *model['mins'], *model['maxs'], *model['mins'], *model['maxs'], *origin, *center)
            struct.pack_into('<I', payload, 196, models + index * 80)
            struct.pack_into('<I', payload, 204, 8)
            self.uc.mem_write(address, bytes(payload))

    def trace(self, start, delta, mins, maxs, contents=0x43):
        end = [f32(start[i] + delta[i]) for i in range(3)]
        self.uc.mem_write(self.io, struct.pack('<12f', *([0,0,0] if mins is None else mins), *([0,0,0] if maxs is None else maxs), *start, *end))
        result = self.io + 0x100
        self.uc.mem_write(result, bytes(64))
        args = [self.world, (0 if mins is None else self.io), (0 if maxs is None else self.io + 12), self.io + 24, self.io + 36, contents, 2, 0, 0, 0, result]
        self.uc.reg_write(UC_X86_REG_ESP, STACK)
        self.uc.mem_write(STACK, struct.pack('<12I', RETURN, *args))
        self.invalid = None
        self.recorded_contact = False
        try:
            self.uc.emu_start(ENTRY, RETURN, count=1000000)
        except Exception as error:
            if self.invalid:
                access, address, size, eip = self.invalid
                raise RuntimeError(f'Invalid memory access {address:#x} size {size} from {eip:#x}') from error
            raise
        if self.uc.reg_read(UC_X86_REG_EAX) == 0:
            return None
        model_pointer = self._u32(result)
        model = (model_pointer - self.runtime_models) // 212 if model_pointer else -1
        impact = list(struct.unpack('<3f', self.uc.mem_read(result + 12, 12)))
        ratio = struct.unpack('<f', self.uc.mem_read(result + 24, 4))[0]
        normal = list(struct.unpack('<3f', self.uc.mem_read(result + 28, 12)))
        leaf = self._u32(0x10362F10)
        return {'model': model, 'leaf': leaf, 'point': impact, 'distance': ratio, 'normal': normal,
                'recordedContact': self.recorded_contact}


def main():
    data=json.loads((ROOT/'assets/training/collision.json').read_text())
    oracle=GenesisCollisionOracle(data)
    origins=[[824,-120,-2092],[1088,-176,-2092],[1280,-176,-2400],[568,-120,-2924],[908,-176,-2716],[616,-176,-2708],[960,-176,-3008]]
    cases=[([x,y+50,z],[0,-450,0]) for x,y,z in origins]
    rng=random.Random(12345)
    cases += [([rng.uniform(-1600,2200),100,rng.uniform(-5300,4400)],[0,-1000,0]) for _ in range(200)]
    samples=[]
    for start,delta in cases:
        start=list(map(f32,start));delta=list(map(f32,delta))
        samples.append(dict(start=start,delta=delta,expected=oracle.trace(start,delta,None,None,1)))
    (ROOT/'evidence/genesis-point-oracle.json').write_text(json.dumps(dict(provenance=dict(dll_sha256=hashlib.sha256(oracle.raw).hexdigest(),entry='10001E30 -> 10038470 -> 100363F0',method='Original DLL instructions; null hull; models only; solid contents; identity model transforms',adapted_from='Desktop fdny_reverse_engineer/reverse_engineer/tools/retail_genesis_collision_oracle.py'),samples=samples),indent=2)+'\n')
    print(len(samples),'native point traces;',sum(bool(s['expected']) for s in samples),'hits')
if __name__=='__main__':main()
