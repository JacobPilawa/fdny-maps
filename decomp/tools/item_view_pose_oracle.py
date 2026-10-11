"""Capture the time-driven item view/aim offsets produced by 0045FEC0."""
import json
import struct
import hashlib

import fire_effects_oracle as n
from unicorn.x86_const import (
    UC_X86_REG_EAX,
    UC_X86_REG_ECX,
    UC_X86_REG_EIP,
    UC_X86_REG_ESP,
)

OBJ = n.OBJECT
ARRAY = OBJ + 0x8000
RECORDS = OBJ + 0x9000
FRAME_SCALE = 0.017

bits = lambda value: struct.unpack('<I', struct.pack('<f', value))[0]


def hook(engine, address, size, data):
    if address == 0x45fb90:
        # Preserve the actual animation time writes. Bone/model refresh is
        # outside this offset-only fixture and does not feed 0045FEC0's vectors.
        sp = engine.reg_read(UC_X86_REG_ESP)
        obj = engine.reg_read(UC_X86_REG_ECX)
        previous = n.u32(obj + 0x33c)
        current = n.u32(sp + 4)
        n.write(obj + 0x340, previous)
        n.write(obj + 0x33c, current)
        engine.reg_write(UC_X86_REG_ESP, sp + 8)  # return address + thiscall arg
        engine.reg_write(UC_X86_REG_EIP, n.u32(sp))
    elif address == 0x47f1a0:
        # 0045FEC0 copies two animation bone vectors into these scratch
        # records. They are not read by its subsequent offset calculation.
        sp = engine.reg_read(UC_X86_REG_ESP)
        engine.reg_write(UC_X86_REG_EAX, RECORDS)
        engine.reg_write(UC_X86_REG_ESP, sp + 4)
        engine.reg_write(UC_X86_REG_EIP, n.u32(sp))


n.uc.hook_add(n.UC_HOOK_CODE, hook)


def capture(initial_time, frame_dt, advance):
    n.uc.mem_write(OBJ, b'\0' * 0x10000)
    n.write(0x502b04, bits(frame_dt))
    n.write(OBJ + 0x2ec, ARRAY)
    n.write(OBJ + 0x2f0, 2)
    n.write(ARRAY, OBJ + 0x7000)
    n.write(ARRAY + 4, OBJ + 0x7010)
    n.write(OBJ + 0x33c, bits(initial_time))
    n.write(OBJ + 0x340, bits(initial_time))
    n.write(OBJ + 0x368, 7)
    n.call(0x45fec0, [bits(advance)], this=OBJ)
    return dict(
        input=dict(initial_time_bits=bits(initial_time), frame_dt_bits=bits(frame_dt),
                   advance_bits=bits(advance)),
        result=dict(
            current_time_bits=n.u32(OBJ + 0x33c),
            previous_time_bits=n.u32(OBJ + 0x340),
            animation_flag=n.u32(OBJ + 0x368),
            aim_offset_bits=[n.u32(OBJ + 0x1f0 + 4*i) for i in range(3)],
            view_offset_bits=[n.u32(OBJ + 0x1e4 + 4*i) for i in range(3)],
        ),
    )


samples = []
for time in [0.0, .01, .08, .15, .22, .30, .37, .45, .52, .60, .66,
             .75, .90, .983, .99, 1.0, 1.001]:
    samples.append(capture(time, FRAME_SCALE, 1.0))
for time, dt, advance in [
    (.99, .017, 1.0), (.75, .5, .5), (.25, .5, 2.0), (0.0, 0.0, 1.0),
    (.5, .1, 0.0),
]:
    samples.append(capture(time, dt, advance))

fixture = dict(
    provenance=dict(
        entry='0045FEC0',
        exe_sha256=hashlib.sha256(n.raw).hexdigest(),
        method=('Original x86 0045FEC0 and 00447710 execute. 0045FB90 is stubbed '
                'to preserve its current/previous time writes; 0047F1A0 returns '
                'scratch bone records whose vectors are copied but do not feed '
                'the view/aim offset calculation. This captures time and offset '
                'arithmetic, not actor animation or live tool state transitions.'),
    ),
    samples=samples,
)
(n.ROOT / 'evidence/item-view-pose-oracle.json').write_text(
    json.dumps(fixture, indent=2) + '\n'
)
print(len(samples), 'original 0045FEC0 item-offset captures')
