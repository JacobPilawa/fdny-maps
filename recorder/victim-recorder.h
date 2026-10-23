/* Included after the player sampler. A lifetime registry is maintained by the
 * spawn/destruction observers. Player updates also sample live registered
 * victims, using guarded reads and a vtable check, independent of AI cadence. */
typedef uintptr_t (__attribute__((thiscall)) *DestroyMethod)(void *, unsigned);
static GameMethod original_victim_spawn, original_victim_update;
static DestroyMethod original_victim_destroy;
typedef struct { void *pointer; unsigned id; DWORD last_tick; int state; int alive; char name[96]; } VictimEntry;
static VictimEntry victims[128];
static unsigned victim_count;
static void reset_victims(void) { memset(victims, 0, sizeof victims); victim_count = 0; }
static void observe_victim(void *pointer, int force) {
    if (trace == INVALID_HANDLE_VALUE || write_failed) return;
    uintptr_t p = (uintptr_t)pointer;
    float position[3], forward[3], minimum[3], maximum[3]; int state;
    if (!read_at(p + 0x120, &state, 4)) return;
    DWORD now = GetTickCount();
    VictimEntry *entry = NULL;
    for (unsigned i = 0; i < victim_count; ++i) if (victims[i].pointer == pointer && victims[i].alive) { entry = victims + i; break; }
    if (entry && !force && state == entry->state && now - entry->last_tick < 50) return;
    if (!read_at(p + 0x4c, position, 12) || !read_at(p + 0x40, forward, 12) ||
        !read_at(p + 0x78, minimum, 12) || !read_at(p + 0x84, maximum, 12)) return;
    for (unsigned i = 0; i < 3; ++i)
        if (!isfinite(position[i]) || !isfinite(forward[i]) || !isfinite(minimum[i]) || !isfinite(maximum[i]) || minimum[i] >= maximum[i]) return;
    if (!entry) {
        if (victim_count >= 128) return;
        entry = victims + victim_count++; entry->pointer = pointer; entry->id = victim_count; entry->alive = 1;
        uintptr_t blob = pointer_at(p + 0x150), name = pointer_at(blob ? blob + 0x10 : 0);
        unsigned i = 0; char c;
        for (; name && i < sizeof entry->name - 1; ++i) {
            if (!read_at(name + i, &c, 1) || !c) break;
            entry->name[i] = ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                (c >= '0' && c <= '9') || c == ' ' || c == '_' || c == '-') ? c : '_';
        }
        entry->name[i] = 0;
        if (!entry->name[0]) snprintf(entry->name, sizeof entry->name, "Victim %u", entry->id);
        force = 1;
    }
    entry->last_tick = now; entry->state = state;
    char line[1200];
    snprintf(line, sizeof line,
        "{\"type\":\"victim_sample\",\"id\":\"v%03u\",\"name\":\"%s\",\"t_ms\":%lu,"
        "\"position\":[%.9g,%.9g,%.9g],\"body_forward\":[%.9g,%.9g,%.9g],\"state_raw\":%d,"
        "\"bounds_min\":[%.9g,%.9g,%.9g],\"bounds_max\":[%.9g,%.9g,%.9g]}\n",
        entry->id, entry->name, (unsigned long)(now - run_tick),
        (double)position[0], (double)position[1], (double)position[2],
        (double)forward[0], (double)forward[1], (double)forward[2], state,
        (double)minimum[0], (double)minimum[1], (double)minimum[2],
        (double)maximum[0], (double)maximum[1], (double)maximum[2]);
    append_line(line);
}
static void end_victim(void *pointer) {
    for (unsigned i = 0; i < victim_count; ++i) if (victims[i].pointer == pointer && victims[i].alive) {
        char line[180]; victims[i].alive = 0;
        snprintf(line, sizeof line, "{\"type\":\"victim_end\",\"id\":\"v%03u\",\"t_ms\":%lu}\n",
            victims[i].id, (unsigned long)(GetTickCount() - run_tick));
        append_line(line); return;
    }
}
static void sample_victims(void) {
    for (unsigned i = 0; i < victim_count; ++i) if (victims[i].alive) {
        void *p = victims[i].pointer;
        if (pointer_at((uintptr_t)p) != base + 0xe25b8) end_victim(p);
        else observe_victim(p, 0);
    }
}
static uintptr_t __attribute__((fastcall)) on_victim_spawn(void *victim, void *unused) {
    (void)unused; uintptr_t result = original_victim_spawn(victim);
    unsigned char fp[512] __attribute__((aligned(16))); __asm__ volatile("fxsave %0" : "=m"(fp));
    DWORD error = GetLastError(); observe_victim(victim, 1); SetLastError(error);
    __asm__ volatile("fxrstor %0" : : "m"(fp)); return result;
}
static uintptr_t __attribute__((fastcall)) on_victim_update(void *victim, void *unused) {
    (void)unused; uintptr_t result = original_victim_update(victim);
    unsigned char fp[512] __attribute__((aligned(16))); __asm__ volatile("fxsave %0" : "=m"(fp));
    DWORD error = GetLastError(); observe_victim(victim, 0); SetLastError(error);
    __asm__ volatile("fxrstor %0" : : "m"(fp)); return result;
}
static uintptr_t __attribute__((fastcall)) on_victim_destroy(void *victim, void *unused, unsigned flags) {
    (void)unused;
    unsigned char fp[512] __attribute__((aligned(16))); __asm__ volatile("fxsave %0" : "=m"(fp));
    DWORD error = GetLastError(); end_victim(victim); SetLastError(error);
    __asm__ volatile("fxrstor %0" : : "m"(fp));
    return original_victim_destroy(victim, flags);
}
static int install_victim_hooks(void) {
    const unsigned char spawn_code[] = {0x6a,0xff,0x68,0x8e,0x10,0x4d,0x00};
    const unsigned char update_code[] = {0x8a,0x41,0x10,0x84,0xc0,0x75,0x0b};
    const unsigned char destroy_code[] = {0x56,0x8b,0xf1,0xe8,0x68,0xfc,0xff,0xff};
    unsigned char bytes[8];
    uintptr_t *spawn = (uintptr_t *)(base + 0xe25dc), *update = (uintptr_t *)(base + 0xe25cc), *destroy = (uintptr_t *)(base + 0xe25b8);
    if (pointer_at((uintptr_t)spawn) != base + 0x5d5b0 || pointer_at((uintptr_t)update) != base + 0x59990 ||
        pointer_at((uintptr_t)destroy) != base + 0x5df60 ||
        !read_at(base + 0x5d5b0, bytes, sizeof spawn_code) || memcmp(bytes, spawn_code, sizeof spawn_code) ||
        !read_at(base + 0x59990, bytes, sizeof update_code) || memcmp(bytes, update_code, sizeof update_code) ||
        !read_at(base + 0x5df60, bytes, sizeof destroy_code) || memcmp(bytes, destroy_code, sizeof destroy_code)) {
        log_line("Victim signatures differ; victim tracking disabled. Player recording remains enabled."); return 0;
    }
    DWORD old, ignored;
    if (!VirtualProtect(destroy, 40, PAGE_READWRITE, &old)) return 0;
    original_victim_spawn = (GameMethod)*spawn; original_victim_update = (GameMethod)*update; original_victim_destroy = (DestroyMethod)*destroy;
    InterlockedExchange((volatile LONG *)spawn, (LONG)(uintptr_t)on_victim_spawn);
    InterlockedExchange((volatile LONG *)update, (LONG)(uintptr_t)on_victim_update);
    InterlockedExchange((volatile LONG *)destroy, (LONG)(uintptr_t)on_victim_destroy);
    VirtualProtect(destroy, 40, old, &ignored);
    log_line("Victim spawn/update/destruction observers installed (up to 20 Hz, plus state transitions)."); return 1;
}
