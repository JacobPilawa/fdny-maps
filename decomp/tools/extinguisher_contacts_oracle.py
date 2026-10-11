"""Directly compare the 00481A50/00481A60 collector and message-call boundary."""
import hashlib
import json
import struct
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP

COLLECTOR = n.OBJECT + 0x30000
FIRE = n.OBJECT + 0x10000
POINT = n.OBJECT + 0x20000
bits = lambda value: struct.unpack('<I', struct.pack('<f', value))[0]
calls = []


def gateway(engine, address, size, data):
    if address != 0x49D4E0:
        return
    sp = engine.reg_read(UC_X86_REG_ESP)
    receiver = engine.reg_read(UC_X86_REG_ECX)
    payload = n.u32(sp + 16)
    calls.append({
        'receiver_offset': receiver - n.OBJECT,
        'route0': n.u32(sp + 4),
        'route1': n.u32(sp + 8),
        'message': n.u32(sp + 12),
        'payload_offset': payload - COLLECTOR,
        'count_at_call': n.u32(COLLECTOR),
        'payload_bits': [n.u32(payload + 4 * i) for i in range(4)],
    })
    # 0049D4E0 is a thiscall wrapper with four stack arguments (RET 0x10).
    engine.reg_write(UC_X86_REG_ESP, sp + 20)
    engine.reg_write(UC_X86_REG_EIP, n.u32(sp))


n.uc.hook_add(UC_HOOK_CODE, gateway)


def read_records():
    return [[n.u32(COLLECTOR + 4 + slot * 16 + word * 4) for word in range(4)]
            for slot in range(32)]


def set_route_values(fire, route0, route1):
    n.write(fire + 4, route0)
    n.write(fire + 8, route1)


def append(fire, point_words):
    for i, word in enumerate(point_words[:3]):
        n.write(POINT + 4 * i, word)
    n.call(0x481A60, [fire, POINT, point_words[3]], this=COLLECTOR)


def generate():
    global calls
    n.uc.mem_write(n.OBJECT, b'\0' * 0x100000)
    calls = []
    fire = FIRE
    route0, route1 = 0x10203040, 0x50607080
    set_route_values(fire, route0, route1)
    n.call(0x481A50, this=COLLECTOR)

    inputs = []
    for i in range(33):
        point = [bits(i * 1.25 - 20.5), bits(13.25 - i * 0.75),
                 bits(-41.0 + i * 2.5), bits(i / 32.0)]
        inputs.append(point)

    counts_after = []
    accepted = []
    for point in inputs:
        before = n.u32(COLLECTOR)
        append(fire, point)
        counts_after.append(n.u32(COLLECTOR))
        accepted.append(n.u32(COLLECTOR) == before + 1)

    final_records = read_records()
    call_count_before_reset = len(calls)
    n.call(0x481A50, this=COLLECTOR)
    reset_count = n.u32(COLLECTOR)
    records_after_reset = read_records()

    second_fire = FIRE + 0x100
    second_routes = [0xA1B2C3D4, 0xE5F60718]
    set_route_values(second_fire, *second_routes)
    reset_input = [0x80000000, 0x00000001, 0x7F7FFFFF, 0x3F400000]
    append(second_fire, reset_input)

    out = {
        'provenance': {
            'exe_sha256': hashlib.sha256(n.raw).hexdigest(),
            'entries': ['00481A50', '00481A60', '0049D350', '0049D4E0'],
            'method': ('Direct original x86 collector/reset execution. A code hook records '
                       'the 0049D4E0 thiscall arguments and payload, then returns with the '
                       'original RET 0x10 cleanup to omit downstream message-manager delivery.'),
        },
        'input_words': inputs,
        'accepted': accepted,
        'counts_after': counts_after,
        'queue_calls': calls,
        'call_count_before_reset': call_count_before_reset,
        'final_records': final_records,
        'reset_count': reset_count,
        'records_after_reset': records_after_reset,
        'reset_append_words': reset_input,
        'reset_append_count': n.u32(COLLECTOR),
        'records_after_reset_append': read_records(),
        'reset_queue_call': calls[-1],
    }
    path = n.ROOT / 'evidence/extinguisher-contacts-oracle.json'
    path.write_text(json.dumps(out, indent=2) + '\n')
    print(f"Original collector: {len(inputs)} appends, {call_count_before_reset} queued calls, "
          f"reset count={reset_count}, second-phase count={n.u32(COLLECTOR)}")


if __name__ == '__main__':
    generate()
