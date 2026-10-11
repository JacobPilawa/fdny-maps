"""Exercise original mapped-message enqueue, FIFO delivery and fire handling."""
import hashlib
import json
import struct
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_EBP, UC_X86_REG_EIP, UC_X86_REG_ESP

OBJ = n.OBJECT
MANAGER = OBJ + 0x30000
ID_MAP = OBJ + 0x80000
SERVICES = OBJ + 0x90000
FIRE_A = OBJ + 0x10000
FIRE_B = OBJ + 0x11000
FIRE_STALE = OBJ + 0x12000
MODEL_A = OBJ + 0x20000
MODEL_B = OBJ + 0x21000
MODEL_STALE = OBJ + 0x22000
COLLECTOR = OBJ + 0x50000
POINT = OBJ + 0x51000
VTABLE = OBJ + 0x60000
ENTRY_A = OBJ + 0x82000
ENTRY_B = OBJ + 0x82010
ID_BULK = OBJ + 0x60000
ID_BULK_COUNT = 510
DELTA = 0x3C888889  # 1/60, supplied frame-time input
bits = lambda value: struct.unpack('<I', struct.pack('<f', value))[0]
contacts = []
deliveries = []


def hook(engine, address, size, data):
    if address == 0x4B7B63:
        # Pair the fixture operator-new hook with a no-op _free for those nodes.
        sp = engine.reg_read(UC_X86_REG_ESP)
        engine.reg_write(UC_X86_REG_ESP, sp + 4)
        engine.reg_write(UC_X86_REG_EIP, n.u32(sp))
        return
    if address == 0x449A40:
        sp = engine.reg_read(UC_X86_REG_ESP)
        receiver = engine.reg_read(UC_X86_REG_ECX)
        payload = n.u32(sp + 8)
        deliveries.append({
            'receiver_offset': receiver - OBJ,
            'message': n.u32(sp + 4),
            'payload_offset': payload - COLLECTOR,
            'payload_bits': [n.u32(payload + 4 * i) for i in range(4)],
        })
    elif address == 0x49B675:
        record = engine.reg_read(UC_X86_REG_EBP)
        contacts.append({
            'record_slot': (record - (MANAGER + 0x50)) // 0x14,
            'record_active_before_clear': n.uc.mem_read(record + 0x10, 1)[0],
            'queued_count_after_pop': n.u32(MANAGER + 0x18),
            'fire_strength_bits': {
                'A': n.u32(FIRE_A + 0x128),
                'B': n.u32(FIRE_B + 0x128),
                'stale': n.u32(FIRE_STALE + 0x128),
            },
            'sprayed_flags': {
                'A': n.uc.mem_read(FIRE_A + 0x104, 1)[0],
                'B': n.uc.mem_read(FIRE_B + 0x104, 1)[0],
                'stale': n.uc.mem_read(FIRE_STALE + 0x104, 1)[0],
            },
        })


n.uc.hook_add(UC_HOOK_CODE, hook)


def word(address, value):
    n.write(address, value)


def init_fire(address, model, strength):
    word(address, VTABLE)
    word(address + 0x70, 2)
    word(address + 0x78, model)
    word(address + 0x128, bits(strength))
    n.uc.mem_write(address + 0x104, b'\0')
    word(model + 0x18, bits(12.0))


def generate():
    global contacts, deliveries
    n.uc.mem_write(OBJ, b'\0' * 0x100000)
    n.heap = OBJ + 0xF0000
    contacts, deliveries = [], []
    draws_before = n.draws

    n.call(0x42B1F0, this=MANAGER)
    word(0x503A78, SERVICES)
    word(SERVICES + 0x10, ID_MAP)
    word(SERVICES + 0x1C, MANAGER)
    word(0x502B04, DELTA)
    n.call(0x49B9E0, this=ID_MAP)

    init_fire(FIRE_A, MODEL_A, 0.91)
    init_fire(FIRE_B, MODEL_B, 0.73)
    word(VTABLE + 8, 0x449A40)
    n.call(0x49D450, this=FIRE_A)
    n.call(0x49D450, this=FIRE_B)
    route_a, generation_a = n.u32(FIRE_A + 4), n.u32(FIRE_A + 8)
    route_b, generation_b = n.u32(FIRE_B + 4), n.u32(FIRE_B + 8)
    # Reuse route A's identity with an older generation; the resolver drops it.
    init_fire(FIRE_STALE, MODEL_STALE, 0.62)
    word(FIRE_STALE + 4, route_a)
    word(FIRE_STALE + 8, generation_a - 1)

    n.call(0x481A50, this=COLLECTOR)
    messages = [
        (FIRE_A, route_a, generation_a, [bits(12.5), bits(-4.25), bits(91.75), bits(0.125)]),
        (FIRE_B, route_b, generation_b, [bits(-16.0), bits(32.5), bits(-8.75), bits(0.375)]),
        (FIRE_STALE, route_a, generation_a - 1, [bits(2.0), bits(3.0), bits(4.0), bits(0.625)]),
        (FIRE_A, route_a, generation_a, [bits(44.0), bits(5.5), bits(1.25), bits(0.875)]),
    ]
    for fire, route0, route1, words in messages:
        for i, value in enumerate(words[:3]):
            word(POINT + i * 4, value)
        n.call(0x481A60, [fire, POINT, words[3]], this=COLLECTOR)

    pending_before = n.u32(MANAGER + 0x18)
    active_before = [n.uc.mem_read(MANAGER + 0x60 + i * 0x14, 1)[0]
                     for i in range(4)]
    # 0049B610 is the embedded message queue's flush/delivery method.
    n.call(0x49B610, [0, 0], this=MANAGER + 0x40)
    active_after = [n.uc.mem_read(MANAGER + 0x60 + i * 0x14, 1)[0]
                    for i in range(4)]
    # Exercise the original registry's complete initial FIFO, release, and
    # reuse boundary only after message delivery has captured both live fires.
    id_allocations = [{'route': route_a, 'generation': generation_a},
                      {'route': route_b, 'generation': generation_b}]
    for i in range(ID_BULK_COUNT):
        object_address = ID_BULK + i * 0x10
        n.call(0x49D450, this=object_address)
        id_allocations.append({'route': n.u32(object_address + 4),
                               'generation': n.u32(object_address + 8)})
    n.call(0x49B780, [FIRE_A + 4], this=ID_MAP)
    reused_address = ID_BULK + ID_BULK_COUNT * 0x10
    n.call(0x49D450, this=reused_address)
    reused_id = {'route': n.u32(reused_address + 4),
                 'generation': n.u32(reused_address + 8)}
    out = {
        'provenance': {
            'exe_sha256': hashlib.sha256(n.raw).hexdigest(),
            'entries': ['0042B1F0', '0042B1A0', '00481A50', '00481A60',
                        '0049D4E0', '0049B4C0', '0049B610', '0049B7E0',
                        '00449A40', '00448970', '0049B9E0', '0049B810',
                        '0049B780', '0049D450'],
            'method': ('Original x86 message-manager constructor, contact collector, '
                       'mapped-message enqueue, queue drain, generation lookup, fire '
                       'message handler, suppression update, ID-service constructor, '
                       'registration and release. Fire routes are assigned by the original '
                       'ID service; one stale generation is deliberately unresolved. '
                       'Frame delta, fire initial strengths and model divisors are supplied. '
                       'A fixture allocator supplies registry and queue heap blocks, and '
                       '_free is bypassed for those fixture-owned blocks; registration, '
                       'delivery and fire routines remain original.'),
            'rng_draws': n.draws - draws_before,
        },
        'input': {'frame_dt_bits': DELTA, 'model_divisor_bits': bits(12.0),
                  'fire_strength_bits': {'A': bits(0.91), 'B': bits(0.73),
                                         'stale': bits(0.62)},
                  'fire_state': 2},
        'messages': [{'fire_offset': fire - OBJ, 'route0': route0,
                      'route1': route1, 'record_bits': words}
                     for fire, route0, route1, words in messages],
        'pending_before_drain': pending_before,
        'mapping_high_water_before_drain': n.u32(MANAGER + 0x4C),
        'active_before_drain': active_before,
        'handler_calls': deliveries,
        'drain_steps': contacts,
        'active_after_drain': active_after,
        'pending_after_drain': n.u32(MANAGER + 0x18),
        'fire_results': {
            'A': {'strength_bits': n.u32(FIRE_A + 0x128),
                  'sprayed': n.uc.mem_read(FIRE_A + 0x104, 1)[0]},
            'B': {'strength_bits': n.u32(FIRE_B + 0x128),
                  'sprayed': n.uc.mem_read(FIRE_B + 0x104, 1)[0]},
            'stale': {'strength_bits': n.u32(FIRE_STALE + 0x128),
                      'sprayed': n.uc.mem_read(FIRE_STALE + 0x104, 1)[0]},
        },
        'id_registry': {'allocations': id_allocations,
                        'released_A_generation': n.u32(FIRE_A + 8),
                        'reused_after_pool_exhaustion': reused_id,
                        'remaining_free_count': n.u32(ID_MAP + 0x101C)},
    }
    path = n.ROOT / 'evidence/fire-message-oracle.json'
    path.write_text(json.dumps(out, indent=2) + '\n')
    print(f"Original mapped fire messages: pending={pending_before}, "
          f"handler calls={len(deliveries)}, drained={len(contacts)}, "
          f"remaining={n.u32(MANAGER + 0x18)}")


if __name__ == '__main__':
    generate()
