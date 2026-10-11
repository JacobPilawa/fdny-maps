"""Capture one controlled original 00461F80 contact/effect/message sequence.

Only the iterator source, item follower/frame-record transforms and iterator
cleanup are fixture-bound. The original contact geometry, bounded collector,
mapped-message manager, identity lookup, fire handler and suppression routine
execute directly from the retail EXE.
"""
import hashlib
import json
import struct
import sys

import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import (
    UC_X86_REG_EAX,
    UC_X86_REG_ECX,
    UC_X86_REG_EBP,
    UC_X86_REG_EIP,
    UC_X86_REG_ESP,
)

OBJ = n.OBJECT
MANAGER = OBJ + 0x30000
FIRE = OBJ + 0x10000
MODEL = OBJ + 0x20000
COLLECTOR = OBJ + 0x3FC
NODE_OTHER = OBJ + 0x70000
NODE_FIRE = OBJ + 0x70010
NODE_WINDOW = OBJ + 0x70020
ID_MAP = OBJ + 0x80000
SERVICES = OBJ + 0x90000
POOL = OBJ + 0x3C8
POOL_ARRAY = OBJ + 0xA1000
BUSY_EMITTER = OBJ + 0xA2000
SPRITE = OBJ + 0xA3000
CONFIG = OBJ + 0xB0000
VTABLE = OBJ + 0x60000
SEED = 4357
DT_BITS = 0x3C888889

bits = lambda value: struct.unpack('<I', struct.pack('<f', value))[0]
float_of = lambda value: struct.unpack('<f', struct.pack('<I', value))[0]
draw_calls = []
range_calls = []
contact_calls = []
handler_calls = []
drain_steps = []
pool_returns = {}
pool_entry_states = []
operator_allocations = []
allow_operator_new = False


def hook(engine, address, size, data):
    sp = engine.reg_read(UC_X86_REG_ESP)

    if allow_operator_new and address == 0x4A5A84 and n.u32(sp + 4) == 0x168:
        # Fixture heap for original operator new in the source-empty-pool case.
        requested = n.u32(sp + 4)
        block = n.heap
        operator_allocations.append({'size': requested, 'address': block - OBJ})
        n.heap += (requested + 15) & ~15
        if n.heap >= OBJ + 0x100000:
            raise RuntimeError('Fixture heap exhausted')
        n.uc.mem_write(block, b'\0' * requested)
        engine.reg_write(UC_X86_REG_EAX, block)
        engine.reg_write(UC_X86_REG_ESP, sp + 4)
        engine.reg_write(UC_X86_REG_EIP, n.u32(sp))
        return

    if address in pool_returns:
        pool_returns[address].append(engine.reg_read(UC_X86_REG_EAX))

    if address == 0x404AC0:
        draw_calls.append({'index': len(draw_calls), 'return_address': n.u32(sp)})

    if address == 0x498B30:
        range_calls.append({
            'index': len(range_calls),
            'min_bits': n.u32(sp + 4),
            'max_bits': n.u32(sp + 8),
        })

    if address == 0x47FF30:
        ret = n.u32(sp)
        pool_returns.setdefault(ret, [])
        pool = engine.reg_read(UC_X86_REG_ECX)
        pool_entry_states.append({
            'pool_offset': pool - OBJ,
            'world': n.u32(pool + 0x28),
            'config': n.u32(pool + 0x2C) - OBJ,
            'limit': n.u32(pool + 0x30),
            'array_offset': n.u32(pool + 0x10) - OBJ,
            'entries': n.u32(pool + 0x14),
            'capacity': n.u32(pool + 0x18),
        })

    if address == 0x481C00:
        # The real candidate list is replaced with three controlled nodes:
        # one unrelated type, one FireRescue, and one WindowFire.
        iterator = engine.reg_read(UC_X86_REG_ECX)
        n.write(iterator, OBJ + 0x6F000)
        n.write(iterator + 4, NODE_OTHER)
        engine.reg_write(UC_X86_REG_EAX, iterator)
        engine.reg_write(UC_X86_REG_ESP, sp + 4)
        engine.reg_write(UC_X86_REG_EIP, n.u32(sp))
        return

    if address == 0x429FE0:
        # Skip unregistering the fixture iterator cursor.
        engine.reg_write(UC_X86_REG_ESP, sp + 8)
        engine.reg_write(UC_X86_REG_EIP, n.u32(sp))
        return

    if address in (0x45F600, 0x482430, 0x480090, 0x4612B0):
        # Supplied item pose; follower and presentation transforms have their
        # own direct fixtures and do not affect this contact/effect capture.
        pop = {0x45F600: 0, 0x482430: 4, 0x480090: 8, 0x4612B0: 8}[address]
        engine.reg_write(UC_X86_REG_ESP, sp + 4 + pop)
        engine.reg_write(UC_X86_REG_EIP, n.u32(sp))
        return

    if address == 0x49D4E0:
        receiver = engine.reg_read(UC_X86_REG_ECX)
        payload = n.u32(sp + 16)
        contact_calls.append({
            'receiver_offset': receiver - OBJ,
            'route0': n.u32(sp + 4),
            'route1': n.u32(sp + 8),
            'message': n.u32(sp + 12),
            'payload_offset': payload - COLLECTOR,
            'payload_bits': [n.u32(payload + 4 * i) for i in range(4)],
        })

    if address == 0x449A40:
        receiver = engine.reg_read(UC_X86_REG_ECX)
        payload = n.u32(sp + 8)
        handler_calls.append({
            'receiver_offset': receiver - OBJ,
            'message': n.u32(sp + 4),
            'payload_offset': payload - COLLECTOR,
            'payload_bits': [n.u32(payload + 4 * i) for i in range(4)],
        })

    if address == 0x49B675:
        record = engine.reg_read(UC_X86_REG_EBP)
        drain_steps.append({
            'record_slot': (record - (MANAGER + 0x50)) // 0x14,
            'record_active_before_clear': n.uc.mem_read(record + 0x10, 1)[0],
            'queued_count_after_pop': n.u32(MANAGER + 0x18),
            'fire_strength_bits': n.u32(FIRE + 0x128),
            'sprayed': n.uc.mem_read(FIRE + 0x104, 1)[0],
        })

    if address == 0x4B7B63:
        # The manager allocated these fixture-owned linked-list nodes.
        engine.reg_write(UC_X86_REG_ESP, sp + 4)
        engine.reg_write(UC_X86_REG_EIP, n.u32(sp))


n.uc.hook_add(UC_HOOK_CODE, hook)


def write_float(address, value):
    n.write(address, bits(value))


def write_vector(address, values):
    for i, value in enumerate(values):
        write_float(address + 4 * i, value)


def init_actor(address, actor_type):
    n.write(address, VTABLE)
    n.write(address + 0x18, actor_type)
    n.write(address + 0x70, 2)
    n.write(address + 0x78, MODEL)
    n.write(address + 0x128, bits(0.91))
    n.uc.mem_write(address + 0x104, b'\0')
    n.write(MODEL + 0x18, bits(12.0))
    if actor_type == 6:
        visual = address + 0x12C
        write_vector(visual + 0x28, [0.0, -120.0, 10.0])
        write_vector(visual + 0x44, [240.0, 240.0, 240.0])
        n.write(visual + 0x60, SPRITE)
        write_float(SPRITE + 0xD0, 1.0)


def generate():
    global draw_calls, range_calls, contact_calls, handler_calls, drain_steps, pool_returns, pool_entry_states, operator_allocations, allow_operator_new
    source_empty_pool = '--source-empty-pool' in sys.argv

    n.uc.mem_write(OBJ, b'\0' * 0x100000)
    n.heap = OBJ + 0xF0000
    draw_calls, range_calls, contact_calls, handler_calls, drain_steps = [], [], [], [], []
    pool_returns = {}
    pool_entry_states = []
    operator_allocations = []
    allow_operator_new = False

    # Original service/queue and ID registry constructors; they receive their
    # real internal storage from the same fixture allocator as the message
    # component oracle.
    n.write(0x503A78, SERVICES)
    n.write(SERVICES + 0x10, ID_MAP)
    n.write(SERVICES + 0x1C, MANAGER)
    n.call(0x42B1F0, this=MANAGER)
    n.call(0x49B9E0, this=ID_MAP)
    n.write(SERVICES + 0x10, ID_MAP)
    n.write(SERVICES + 0x1C, MANAGER)

    n.call(0x404B50, [SEED])
    n.draws = 0
    n.write(0x502B04, DT_BITS)
    n.write(0x502AFC, 1000)
    n.write(0x502E60, 1)

    # Supplied active item pose: identity basis at the origin. This yields the
    # original four points (0,-30,10), (0,-90,10), (0,-150,10), (0,-210,10).
    n.write(OBJ + 0x58, 1)
    write_vector(OBJ + 0x5C, [1, 0, 0, 0, 1, 0, 0, 0, 1])
    write_vector(OBJ + 0x80, [0, 0, 0])
    n.uc.mem_write(OBJ + 0x158, b'\1')
    n.uc.mem_write(OBJ + 0x600, b'\1')
    n.write(OBJ + 0x232 * 4, 0)
    n.write(OBJ + 0x233 * 4, 0)
    n.write(OBJ + 0xEA * 4, 0)

    # One accepted fire contact actor and two type-filter controls.
    n.write(VTABLE + 8, 0x449A40)
    init_actor(OBJ + 0x4000, 5)
    init_actor(FIRE, 6)
    init_actor(OBJ + 0x14000, 7)
    n.call(0x49D450, this=FIRE)
    n.write(NODE_OTHER, OBJ + 0x4000)
    n.write(NODE_OTHER + 4, NODE_FIRE)
    n.write(NODE_FIRE, FIRE)
    n.write(NODE_FIRE + 4, NODE_WINDOW)
    n.write(NODE_WINDOW, OBJ + 0x14000)
    n.write(NODE_WINDOW + 4, 0)

    # The pool is embedded at item +0x3c8 and the contact collector begins at
    # pool +0x34 / item +0x3fc. Use the original constructor/setup in both cases.
    n.call(0x480260, this=POOL)
    n.write(CONFIG + 4, 1)       # descriptor frame count read by 0047D2B0
    n.write(CONFIG + 0x18, 0)    # descriptor loop flags read by 0047D2B0
    n.call(0x47FE70, [1, CONFIG, 0], this=POOL)
    if source_empty_pool:
        # Run original constructor/setup. The array backing block is supplied
        # up front at the source growth quantum (16), avoiding host _realloc.
        n.write(POOL + 0x10, POOL_ARRAY)
        n.write(POOL + 0x14, 0)
        n.write(POOL + 0x18, 16)
    else:
        # Valid full/busy branch fixture, unlike the source-initial pool.
        n.write(POOL + 0x10, POOL_ARRAY)
        n.write(POOL + 0x14, 1)
        n.write(POOL + 0x18, 16)
        n.write(POOL + 0x28, 1)
        n.write(POOL + 0x2C, 1)
        n.write(POOL + 0x30, 1)
        n.write(POOL_ARRAY, BUSY_EMITTER)
        n.uc.mem_write(BUSY_EMITTER + 0x30, b'\1')

    draws_before = n.draws
    allow_operator_new = source_empty_pool
    n.call(0x461F80, this=OBJ)
    allow_operator_new = False

    count = n.u32(COLLECTOR)
    records = [
        [n.u32(COLLECTOR + 4 + slot * 16 + word * 4) for word in range(4)]
        for slot in range(count)
    ]
    pending_before_drain = n.u32(MANAGER + 0x18)
    active_before_drain = [
        n.uc.mem_read(MANAGER + 0x60 + i * 0x14, 1)[0] for i in range(count)
    ]
    fire_before_drain = n.u32(FIRE + 0x128)
    effect_timer_after_contact = n.u32(OBJ + 0xEA * 4)

    # This is the original drain method called from 0042AEA0 -> 00429520.
    # Calling it here records a sequential source fixture; it does not assert
    # ordering against the extinguisher's live Think callback.
    n.call(0x49B610, [0, 0], this=MANAGER + 0x40)

    out = {
        'provenance': {
            'exe_sha256': hashlib.sha256(n.raw).hexdigest(),
            'entries': ['00461F80', '0044A7A0', '00481A50', '00481A60',
                        '0049D350', '0049D4E0', '0049B4C0', '0049B610',
                        '0049B7E0', '00449A40', '00448970', '0047FF30',
                        '00498B30', '004B9400', '00404AC0'],
            'method': (
                'Original x86 00461F80 runs with a supplied active item pose. '
                'The iterator factory at 00481C00 supplies three controlled candidates '
                '(types 5, 6 and 7); iterator cursor cleanup, item follower/frame-record '
                'transforms (0045F600, 00482430, 00480090, 004612B0) are stubbed. '
                'Original fire contact geometry, contact collector, route registration, '
                'mapped FIFO queue, generation lookup, FireRescue handler and suppression '
                'execute. ' + ('The original pool constructor/setup creates an empty pool with '
                'limit -1; its one-frame fixture descriptor has count 1 and flags 0. The '
                'pool array backing is preallocated at the source growth quantum; the original '
                'allocator and emitter constructor execute and return one emitter. '
                if source_empty_pool else 'The effect pool is an explicit one-entry full/busy fixture, '
                'so the timer draw executes and no emitter-position draws occur. ') + 'The original '
                '0049B610 drain is then called after 00461F80 as a sequential capture; '
                'this does not establish live ordering against tool/fire Think callbacks. '
                'The fire actor extents, alpha, strength, state, model divisor, frame delta, '
                'pose and seed are supplied controlled values.'
            ),
            'seed': SEED,
            'rng_draws': n.draws - draws_before,
            'draw_sites': draw_calls,
            'range_calls': range_calls,
            'pool_returns': [
                {'return_address': address, 'eax_values': values}
                for address, values in sorted(pool_returns.items())
            ],
            'pool_entry_states': pool_entry_states,
            'operator_allocations': operator_allocations,
        },
        'input': {
            'frame_dt_bits': DT_BITS,
            'game_time': 1000,
            'item_origin': [0, 0, 0],
            'item_basis_bits': [bits(x) for x in [1, 0, 0, 0, 1, 0, 0, 0, 1]],
            'candidate_types': [5, 6, 7],
            'fire': {
                'route': n.u32(FIRE + 4),
                'generation': n.u32(FIRE + 8),
                'strength_bits_before': bits(0.91),
                'visual_center_bits': [bits(x) for x in [0.0, -120.0, 10.0]],
                'visual_extents_bits': [bits(x) for x in [240.0, 240.0, 240.0]],
                'sprite_alpha_bits': bits(1.0),
                'model_divisor_bits': bits(12.0),
            },
            'effect_pool': ({
                'setup': 'original 00480260 + 0047FE70; fixture descriptor count=1 flags=0',
                'initial_entries': 0,
                'limit': -1,
                'entries_after_contact': n.u32(POOL + 0x14),
                'emitters_after_contact': [
                    {
                        'active': n.uc.mem_read(n.u32(POOL_ARRAY + 4 * i) + 0x30, 1)[0],
                        'offset_vector_bits': [n.u32(n.u32(POOL_ARRAY + 4 * i) + 0x7C + 4 * axis) for axis in range(3)],
                        'origin_vector_bits': [n.u32(n.u32(POOL_ARRAY + 4 * i) + 0x88 + 4 * axis) for axis in range(3)],
                    }
                    for i in range(n.u32(POOL + 0x14))
                ],
            } if source_empty_pool else {'capacity': 1, 'entries': 1, 'busy_entries': 1}),
        },
        'generated_points_bits': [
            [n.u32(0x502E30 + slot * 12 + axis * 4) for axis in range(3)]
            for slot in range(4)
        ],
        'contacts': {
            'count': count,
            'records_bits': records,
            'queue_calls': contact_calls,
        },
        'queue': {
            'pending_before_drain': pending_before_drain,
            'mapping_high_water_before_drain': n.u32(MANAGER + 0x4C),
            'active_before_drain': active_before_drain,
            'handler_calls': handler_calls,
            'drain_steps': drain_steps,
            'pending_after_drain': n.u32(MANAGER + 0x18),
            'fire_strength_bits_before_drain': fire_before_drain,
            'fire_strength_bits_after_drain': n.u32(FIRE + 0x128),
            'fire_sprayed_after_drain': n.uc.mem_read(FIRE + 0x104, 1)[0],
            'effect_timer_after_contact': effect_timer_after_contact,
        },
    }
    filename = 'extinguisher-contact-path-source-empty-pool-oracle.json' if source_empty_pool else 'extinguisher-contact-path-full-busy-pool-oracle.json'
    path = n.ROOT / 'evidence' / filename
    path.write_text(json.dumps(out, indent=2) + '\n')
    print(
        f"Original 00461F80 {'empty-pool' if source_empty_pool else 'full/busy-pool'} fixture: points=4, contacts={count}, "
        f"mapped={len(contact_calls)}, drained={len(drain_steps)}, "
        f"shared draws={n.draws - draws_before}"
    )


if __name__ == '__main__':
    generate()
