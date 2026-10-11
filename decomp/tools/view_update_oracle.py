"""Execute the original 0044F340 player view-frame update on deterministic fixtures."""
import json, struct, math, sys
import fire_effects_oracle as n
from unicorn.x86_const import UC_X86_REG_EBX, UC_X86_REG_EDX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP

OBJ = n.OBJECT
TOOL = OBJ + 0x8000
ITEMS = OBJ + 0x9000
VTABLE = 0x20019100
STUB = 0x20019000
TRACE = []
ERROR_CALLER = None
ARRAY_CALLS = []
CALLS = []
STAGES = []
HEAD_OUTPUT = []
ITEM_OFFSET_UPDATES = []
ITEM_ANIM_ARRAY = OBJ + 0xb000
ITEM_ANIM_RECORDS = OBJ + 0xb100

bits = lambda f: struct.unpack('<I', struct.pack('<f', f))[0]
f32 = lambda a: struct.unpack('<f', struct.pack('<I', n.u32(a)))[0]

def hook(engine, address, size, data):
    global ERROR_CALLER
    TRACE.append(address)
    if len(TRACE) > 24: del TRACE[0]
    if address == 0x422060:
        sp = engine.reg_read(n.UC_X86_REG_ESP)
        ERROR_CALLER = n.u32(sp)
    if address == 0x48d490:
        sp = engine.reg_read(n.UC_X86_REG_ESP)
        arr = engine.reg_read(n.UC_X86_REG_ECX)
        ARRAY_CALLS.append((n.u32(sp), n.u32(arr + 4), arr, n.u32(sp + 4)))
    if address in (0x447710, 0x446390, 0x42d050):
        sp = engine.reg_read(n.UC_X86_REG_ESP)
        vals = [n.u32(sp + 4*i) for i in range(1, 5)]
        def vector(ptr):
            mapped = 0x20000000 <= ptr < 0x20020000 or OBJ <= ptr < OBJ + 0x100000
            return [f32(ptr + 4*i) for i in range(3)] if mapped else None
        CALLS.append((address, vals, [vector(v) for v in vals if v]))
    if address in (0x42bd50, 0x496210, 0x496360):
        sp = engine.reg_read(n.UC_X86_REG_ESP)
        ecx = engine.reg_read(n.UC_X86_REG_ECX)
        vals = [f32(sp + 4 + 4*i) for i in range(3)]
        CALLS.append((address, ecx, vals))
    if address == 0x496b50:
        sp = engine.reg_read(n.UC_X86_REG_ESP)
        CALLS.append((address, engine.reg_read(n.UC_X86_REG_ECX), f32(sp + 4)))
    if address in (0x496210, 0x496360, 0x496b50, 0x4964b0):
        frame = engine.reg_read(n.UC_X86_REG_ECX)
        STAGES.append(dict(address=address,
            basis=[n.u32(frame + 4 + 4*i) for i in range(9)]))
    if address == 0x44f3df:
        HEAD_OUTPUT.append([n.u32(OBJ + 0x3488 + 4*i) for i in range(3)])
    if address == 0x45fb90:
        # Keep the time boundary that the existing offset-only oracle uses.
        # Bone/model refresh itself does not affect the later offset vectors.
        sp = engine.reg_read(n.UC_X86_REG_ESP)
        obj = engine.reg_read(n.UC_X86_REG_ECX)
        previous = n.u32(obj + 0x33c)
        current = n.u32(sp + 4)
        n.write(obj + 0x340, previous)
        n.write(obj + 0x33c, current)
        engine.reg_write(n.UC_X86_REG_ESP, sp + 8)
        engine.reg_write(n.UC_X86_REG_EIP, n.u32(sp))
    if address == 0x47f1a0:
        # Both copied animation records are scratch outputs in this capture.
        sp = engine.reg_read(n.UC_X86_REG_ESP)
        engine.reg_write(n.UC_X86_REG_EAX, ITEM_ANIM_RECORDS)
        engine.reg_write(n.UC_X86_REG_ESP, sp + 4)
        engine.reg_write(n.UC_X86_REG_EIP, n.u32(sp))
    if address in (0x481c00, 0x4983e0):
        sp = engine.reg_read(n.UC_X86_REG_ESP)
        if address == 0x481c00:
            out = engine.reg_read(n.UC_X86_REG_ECX)
            engine.mem_write(out, b'\0' * 8)
        engine.reg_write(n.UC_X86_REG_EIP, n.u32(sp))
        engine.reg_write(n.UC_X86_REG_ESP, sp + 4)

n.uc.hook_add(n.UC_HOOK_CODE, hook)
n.uc.mem_write(STUB, b'\xc3\xc3\xc3')
n.uc.mem_write(VTABLE, b'\0' * 0x40)
n.write(VTABLE + 0x20, STUB)
n.write(VTABLE + 0x24, STUB + 1)

def frame(base, position, basis=None, parent=0):
    if basis is None:
        basis = [1,0,0, 0,1,0, 0,0,1]
    n.write(base, 0x4dfc30)
    for i, value in enumerate(basis): n.write(base + 4 + 4*i, bits(value))
    for i, value in enumerate(position): n.write(base + 0x28 + 4*i, bits(value))
    for i in range(3): n.write(base + 0x34 + 4*i, bits(1.0))
    n.write(base + 0x40, parent)

def capture(sample):
    global ERROR_CALLER
    TRACE.clear(); ARRAY_CALLS.clear(); CALLS.clear(); STAGES.clear(); HEAD_OUTPUT.clear(); ITEM_OFFSET_UPDATES.clear(); ERROR_CALLER = None
    n.uc.mem_write(OBJ, b'\0' * 0x100000)
    n.write(0x502b04, bits(sample['frame_dt']))
    n.write(OBJ + 0x18c, sample.get('mode', 0))
    n.write(OBJ + 0x12c, sample.get('active', 0))
    n.write(OBJ + 0x3430, bits(sample['angle']))
    n.write(OBJ + 0x3440, 0)
    n.write(OBJ + 0x3564, ITEMS)
    n.write(OBJ + 0x3568, 1)
    n.write(OBJ + 0x357c, 0)
    n.write(ITEMS, TOOL)
    n.write(TOOL, VTABLE)
    if 'item_initial_time' in sample:
        n.write(TOOL + 0x2ec, ITEM_ANIM_ARRAY)
        n.write(TOOL + 0x2f0, 2)
        n.write(ITEM_ANIM_ARRAY, ITEM_ANIM_RECORDS)
        n.write(ITEM_ANIM_ARRAY + 4, ITEM_ANIM_RECORDS + 0x10)
        n.write(TOOL + 0x33c, bits(sample['item_initial_time']))
        n.write(TOOL + 0x340, bits(sample['item_initial_time']))
        n.write(TOOL + 0x368, 7)
        for offset in (0x344, 0x348, 0x34c, 0x350, 0x354, 0x358):
            n.write(TOOL + offset, 0)
        n.call(0x45fec0, [bits(sample.get('item_advance', 1.0))], this=TOOL)
        sample['tool_offset'] = [f32(TOOL + 0x1e4 + 4*i) for i in range(3)]
        sample['tool_rotation'] = [f32(TOOL + 0x1f0 + 4*i) for i in range(3)]
        ITEM_OFFSET_UPDATES.append(dict(current_time_bits=n.u32(TOOL + 0x33c),
            previous_time_bits=n.u32(TOOL + 0x340),
            view_offset_bits=[n.u32(TOOL + 0x1e4 + 4*i) for i in range(3)],
            aim_offset_bits=[n.u32(TOOL + 0x1f0 + 4*i) for i in range(3)]))
    else:
        for i, value in enumerate(sample['tool_offset']): n.write(TOOL + 0x1e4 + 4*i, bits(value))
        for i, value in enumerate(sample['tool_rotation']): n.write(TOOL + 0x1f0 + 4*i, bits(value))

    # The player root, local view frame and both follower targets use the
    # original frame layout: 3 basis vectors, translation, scale, parent.
    frame(OBJ + 0x24, sample['player_position'], sample['player_basis'])
    frame(OBJ + 0x3460, [0,0,0], parent=OBJ + 0x24)
    follower = OBJ + 0x34a4
    n.write(follower + 0x70, OBJ + 0x3460)
    n.write(follower + 0x6c, sample.get('follower_flags', 0x2a))
    thresholds = sample.get('follower_thresholds', [20, 1000, 20])
    settling = sample.get('follower_settling', [.05, 0, .05])
    smoothing = sample.get('follower_smoothing', [0, .05, 0])
    n.write(follower + 0x44, bits(thresholds[0])); n.write(follower + 0x48, bits(thresholds[1])); n.write(follower + 0x4c, bits(thresholds[2]))
    n.write(follower + 0x50, bits(settling[0])); n.write(follower + 0x54, bits(settling[1])); n.write(follower + 0x58, bits(settling[2]))
    n.write(follower + 0x5c, bits(smoothing[0])); n.write(follower + 0x60, bits(smoothing[1])); n.write(follower + 0x64, bits(smoothing[2]))
    for i, value in enumerate(sample['player_basis']): n.write(follower + 4 + 4*i, bits(value))
    for i in range(3):
        n.write(follower + 0x28 + 4*i, bits(sample['player_position'][i]))
        n.write(follower + 0x74 + 4*i, bits(sample['player_position'][i]))
    n.uc.mem_write(follower + 0x80, bytes([sample.get('follower_held', 0),
                                           sample.get('follower_allow', 1),
                                           sample.get('follower_resume', 0)]))

    # Head motion current-mode slot is initialized to match the branch in
    # 0044F340. Its zero amplitudes represent the stationary startup pose.
    head = OBJ + 0x198
    head_array, head_record = OBJ + 0xa000, OBJ + 0xa100
    n.write(head + 4, head_array); n.write(head + 8, 1); n.write(head_array, head_record)
    moving = sample.get('moving', 0)
    if sample.get('mode') == 5:
        head_mode = 4 if moving else 0
    elif sample.get('active'):
        head_mode = (3 if sample.get('mode') == 4 else 1) if moving else 0
    else:
        head_mode = 2
    n.write(head + 0x64, head_mode)

    position_endpoint = sample.get('position_endpoint', [0, -20, -30])
    target_endpoint = sample.get('target_endpoint', [-60, 1000, 0])
    rotation_fraction = sample.get('rotation_fraction', sample['fraction'])
    position_fraction = sample.get('position_fraction', sample['fraction'])
    args = [sample.get('moving', 0), sample.get('actions', 0), bits(rotation_fraction),
            *[bits(x) for x in target_endpoint], bits(position_fraction),
            *[bits(x) for x in position_endpoint]]
    try:
        n.call(0x44f340, args=args, this=OBJ)
    except Exception:
        ids = dict(EIP=n.UC_X86_REG_EIP, ESP=n.UC_X86_REG_ESP, EAX=n.UC_X86_REG_EAX,
                   EBX=UC_X86_REG_EBX, ECX=n.UC_X86_REG_ECX, EDx=UC_X86_REG_EDX,
                   ESI=UC_X86_REG_ESI, EDI=UC_X86_REG_EDI, EBP=UC_X86_REG_EBP)
        regs = {name: n.uc.reg_read(reg) for name,reg in ids.items()}
        print('view update trace:', ' '.join(f'{pc:08x}' for pc in TRACE))
        print('array-check caller:', f'{ERROR_CALLER:08x}' if ERROR_CALLER is not None else 'none')
        print('array accesses:', [(f'{ret:08x}', f'{ptr:08x}', size, index) for ret,size,ptr,index in ARRAY_CALLS[-8:]])
        print('view update registers:', {k: f'{v:08x}' for k,v in regs.items()})
        raise
    if '--verbose' in sys.argv and sample['frame_dt'] == .017 and sample['fraction'] == .5:
        print('helper calls:', CALLS)

    matrix = [n.u32(OBJ + 0x3460 + 4*i) for i in range(16)]
    follow_position = [n.u32(follower + 0x28 + 4*i) for i in range(3)]
    follow_basis = [n.u32(follower + 4 + 4*i) for i in range(9)]
    head_offset = [n.u32(OBJ + 0x3488 + 4*i) for i in range(3)]
    return dict(args=args, head_motion_mode=n.u32(head + 0x64), matrix_bits=matrix, follower_position_bits=follow_position,
                follower_basis_bits=follow_basis, head_offset_bits=head_offset,
                follower_state=list(n.uc.mem_read(follower + 0x80, 3)),
                head_motion_bits=HEAD_OUTPUT[-1] if HEAD_OUTPUT else [],
                orientation_stages=STAGES.copy())

samples = []
cases = [
    (.001, .017, -.9), (.25, .017, -.6), (.5, .017, -.3),
    (.75, .017, 0), (1, .017, .3), (0, .017, .7),
    (.1, .001, -.8), (.9, .001, .8), (.33, .033, -.2),
    (.67, .033, .2), (.5, .1, 1.1), (.5, .25, -1.1),
]
for i, (fraction, frame_dt, angle) in enumerate(cases):
    yaw = [-.8, -.4, 0, .35, .7, 1.0][i % 6]
    c = struct.unpack('<f', struct.pack('<f', math.cos(yaw)))[0]
    s = struct.unpack('<f', struct.pack('<f', math.sin(yaw)))[0]
    sample = dict(mode=0, active=0, fraction=fraction, frame_dt=frame_dt, angle=angle,
                  player_position=[-1268 + i*31, -172 + i*7, 4692 - i*53],
                  player_basis=[c,0,-s, 0,1,0, s,0,c],
                  tool_offset=[0,0,0] if i == 0 else [i*1.25, -i*.5, i*2.0],
                  tool_rotation=[0,0,0] if i == 0 else [i*.01, -i*.02, i*.005])
    result = capture(sample)
    samples.append(dict(input=sample, result=result))

out = dict(provenance=dict(entry='0044F340', exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),
    method='Original x86 update, frame vtable, head-motion routine, parent composition and followers executed; inputs use the 00451080 transition vectors (-60,1000,0) and (0,-20,-30), with independent frame dt and transition fraction; entity animation callbacks and empty global entity-list services are fixtures; initial head-motion amplitudes are zero.'),
    samples=samples)
(n.ROOT / 'evidence/view-update-oracle.json').write_text(json.dumps(out, indent=2) + '\n')
print(len(samples), 'original 0044F340 view updates captured')

# The ordinary 00452570 caller uses zero transition endpoints and full
# fractions. Its follower is configured by 0044D550 and related helpers.
movement_cases = [
    dict(name='standing_idle', mode=3, active=1, moving=0),
    dict(name='standing_moving', mode=3, active=1, moving=1),
    dict(name='crouching_moving', mode=4, active=1, moving=1),
    dict(name='inactive_moving', mode=3, active=0, moving=1),
]
movement_samples = []
for i, sample in enumerate(movement_cases):
    yaw = [-.65, -.2, .35, .8][i]
    c = struct.unpack('<f', struct.pack('<f', math.cos(yaw)))[0]
    s = struct.unpack('<f', struct.pack('<f', math.sin(yaw)))[0]
    sample.update(fraction=1.0, rotation_fraction=1.0, position_fraction=1.0,
                  angle=0.0, frame_dt=.017,
                  player_position=[-1220 + i*37, -160 + i*5, 4580 - i*61],
                  player_basis=[c,0,-s, 0,1,0, s,0,c],
                  tool_offset=[0,0,0], tool_rotation=[0,0,0],
                  position_endpoint=[0,0,0], target_endpoint=[0,0,0],
                  actions=0, follower_flags=0x2e,
                  follower_held=1, follower_allow=0, follower_resume=0,
                  follower_thresholds=[20,1000,20],
                  follower_settling=[.05,0,.05], follower_smoothing=[0,.05,0])
    result = capture(sample)
    movement_samples.append(dict(input=sample, result=result))

movement_out = dict(provenance=dict(entry='0044F340 called from 00452570 at 00453860',
    exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),
    method='Original x86 movement view update with ordinary caller flags, full interpolation fractions, zero transition endpoints, state 3 or crouch state 4, and the player follower settings written by 0044D550. The 00491140 head routine executes in the selected mode with zeroed initial amplitudes; tool callbacks and global list services are fixtures. Frame and follower captures do not establish whole-caller or Training parity.'),
    samples=movement_samples)
(n.ROOT / 'evidence/view-movement-oracle.json').write_text(json.dumps(movement_out, indent=2) + '\n')
print(len(movement_samples), 'ordinary 0044F340 movement view updates captured')

# Chain the original time-driven item offset writer into the ordinary view
# update. The item Think vtable slot remains a return stub during 0044F340;
# 0045FEC0 is executed immediately beforehand with the same frame delta.
chain_samples = []
for i, initial_time in enumerate([0.0, .08, .15, .22, .37, .52, .66, .75, .90, .983, .99, 1.001]):
    dt = [.001, .017, .0333333, .1][i % 4]
    yaw = [-.8, -.4, 0, .35, .7, 1.0][i % 6]
    c = struct.unpack('<f', struct.pack('<f', math.cos(yaw)))[0]
    s = struct.unpack('<f', struct.pack('<f', math.sin(yaw)))[0]
    sample = dict(mode=0, active=0, moving=0, actions=0, fraction=1.0,
                  frame_dt=dt, angle=0.0,
                  item_initial_time=initial_time, item_advance=1.0,
                  player_position=[-1268 + i*29, -172 + i*5, 4692 - i*47],
                  player_basis=[c,0,-s, 0,1,0, s,0,c],
                  tool_offset=[0,0,0], tool_rotation=[0,0,0],
                  position_endpoint=[0,0,0], target_endpoint=[0,0,0],
                  follower_flags=0x2e, follower_held=1, follower_allow=0,
                  follower_resume=0, follower_thresholds=[20,1000,20],
                  follower_settling=[.05,0,.05], follower_smoothing=[0,.05,0])
    result = capture(sample)
    chain_samples.append(dict(input=sample, item_offset_update=ITEM_OFFSET_UPDATES[-1], result=result))

chain_out = dict(provenance=dict(
    entries=['0045FEC0', '0044F340'],
    exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),
    method=('Original x86 0045FEC0 computes the view/aim offsets from the item '
            'animation clock, then original 0044F340 consumes those stored '
            'offsets with ordinary full fractions and zero endpoints. 0045FB90 '
            'time writes and 0047F1A0 scratch bone records are fixtures; during '
            '0044F340 the item Think/animation callbacks and global entity-list '
            'services are return-stubbed. This verifies the offset producer to '
            'view-frame consumer data path, not item activation or gameplay.'),
    ), samples=chain_samples)
(n.ROOT / 'evidence/view-item-offset-chain-oracle.json').write_text(json.dumps(chain_out, indent=2) + '\n')
print(len(chain_samples), 'original 0045FEC0 → 0044F340 item-offset/view chains captured')
