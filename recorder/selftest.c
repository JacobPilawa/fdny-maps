/* Synthetic integration test for Windows/Wine; never attaches to the game. */
#define RECORDER_SELFTEST
#include "FDNYRecorder.c"
#include <stdlib.h>

static unsigned init_calls, update_calls;
static unsigned victim_spawns, victim_updates, victim_destroys;
static uintptr_t __attribute__((thiscall)) fake_victim_spawn(void *p) {
    (void)p; ++victim_spawns; SetLastError(111); return 111;
}
static uintptr_t __attribute__((thiscall)) fake_victim_update(void *p) {
    ++victim_updates; *(float *)((char *)p + 0x4c) += 2; SetLastError(222); return 222;
}
static uintptr_t __attribute__((thiscall)) fake_victim_destroy(void *p, unsigned flags) {
    (void)p; if (flags != 73) abort(); ++victim_destroys; SetLastError(333); return 333;
}
static uintptr_t __attribute__((thiscall)) fake_init(void *p) {
    (void)p; ++init_calls; SetLastError(1234); return 0x12345678;
}
static uintptr_t __attribute__((thiscall)) fake_update(void *p) {
    ++update_calls;
    *(float *)((char *)p + 0x4c) += 1.0f;
    SetLastError(4321); return 0x87654321;
}
static int fail(const char *s) { fprintf(stderr, "FAIL: %s\n", s); return 1; }
int main(int argc, char **argv) {
    if (argc != 2) return fail("expected Windows output directory");
    snprintf(output_dir, sizeof output_dir, "%s", argv[1]);
    if (!CreateDirectoryA(output_dir, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) return fail("output directory");
    base = (uintptr_t)VirtualAlloc(NULL, 0x110000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (!base) return fail("fixture allocation");
    memcpy((void *)(base + 0x4d550), "\x55\x8b\xec\x83\xe4\xf8", 6);
    memcpy((void *)(base + 0x4fe20), "\x83\xec\x68\x56\x8b\xf1", 6);
    uintptr_t *islot = (uintptr_t *)(base + 0xe1c68), *uslot = (uintptr_t *)(base + 0xe1c5c);
    *islot = base + 0x4d550; *uslot = base + 0x4fe20;
    *(uintptr_t *)(base + 0xe1c40) = base + 0x52570;
    /* Wrong-build guard must leave both entries untouched. */
    *(unsigned char *)(base + 0x4fe20) = 0;
    if (install_hooks() || *uslot != base + 0x4fe20 || *islot != base + 0x4d550) return fail("signature guard");
    *(unsigned char *)(base + 0x4fe20) = 0x83;
    if (!install_hooks()) return fail("valid fixture install");
    memcpy((void *)(base + 0x5d5b0), "\x6a\xff\x68\x8e\x10\x4d\x00", 7);
    memcpy((void *)(base + 0x59990), "\x8a\x41\x10\x84\xc0\x75\x0b", 7);
    memcpy((void *)(base + 0x5df60), "\x56\x8b\xf1\xe8\x68\xfc\xff\xff", 8);
    uintptr_t *vs = (uintptr_t *)(base + 0xe25dc), *vu = (uintptr_t *)(base + 0xe25cc), *vd = (uintptr_t *)(base + 0xe25b8);
    *vs = base + 0x5d5b0; *vu = base + 0x59990; *vd = base + 0x5df60;
    if (!install_victim_hooks()) return fail("victim hook installation");
    victim_tracking = 1;
    original_victim_spawn = fake_victim_spawn; original_victim_update = fake_victim_update; original_victim_destroy = fake_victim_destroy;
    original_init = fake_init; original_update = fake_update;
    char *player = calloc(1, 0x3580), *session = calloc(1, 0x700), *level = calloc(1, 32), *name = calloc(1, 96);
    *(uintptr_t *)(base + 0x102aac) = (uintptr_t)session;
    *(uintptr_t *)(session + 0x688) = (uintptr_t)level;
    *(uintptr_t *)level = (uintptr_t)name;
    strcpy(name + 12, "training");
    *(float *)(base + 0x102b04) = 0.017f;
    *(float *)(player + 0x4c) = -1268;
    *(float *)(player + 0x50) = -172;
    *(float *)(player + 0x54) = 4692;
    *(float *)(player + 0x48) = 1;
    *(float *)(player + 0x3430) = 15;
    *(unsigned *)(player + 0x18c) = 3;
    *(unsigned char *)(player + 0x12c) = 1;
    const float minimum[3] = {-30,-30,-30}, maximum[3] = {30,85,30};
    memcpy(player + 0x78, minimum, 12); memcpy(player + 0x84, maximum, 12);
    char *victim = calloc(1, 0x400), *blob = calloc(1, 64);
    *(uintptr_t *)victim = base + 0xe25b8;
    *(uintptr_t *)(victim + 0x150) = (uintptr_t)blob; *(uintptr_t *)(blob + 0x10) = (uintptr_t)"victim1";
    const float victim_min[3] = {-24,-24,-24}, victim_max[3] = {24,24,24};
    memcpy(victim + 0x78, victim_min, 12); memcpy(victim + 0x84, victim_max, 12);
    *(unsigned *)(victim + 0x120) = 7;
    unsigned short original_cw, after_cw, test_cw = 0x077f;
    __asm__ volatile("fnstcw %0" : "=m"(original_cw));
    __asm__ volatile("fldcw %0" : : "m"(test_cw));
    for (unsigned run = 0; run < 3; ++run) {
        if (run == 2) strcpy(name + 12, "bank");
        *(unsigned *)(victim + 0x120) = 7;
        if (((GameMethod)*islot)(player) != 0x12345678 || GetLastError() != 1234) return fail("init forwarding/last error");
        if (((GameMethod)*vs)(victim) != 111 || GetLastError() != 111) return fail("victim spawn forwarding");
        for (unsigned i = 0; i < 5; ++i) {
            *(unsigned *)(victim + 0x120) = 4 + (i % 2); /* Record state transitions even inside the rate limit. */
            if (((GameMethod)*vu)(victim) != 222 || GetLastError() != 222) return fail("victim update forwarding");
            *(float *)(player + 0x88) = i % 2 ? 25 : 85;
            *(unsigned *)(player + 0x18c) = i % 2 ? 4 : 3;
            if (((GameMethod)*uslot)(player) != 0x87654321 || GetLastError() != 4321) return fail("update forwarding/last error");
            __asm__ volatile("fnstcw %0" : "=m"(after_cw));
            if (after_cw != test_cw) return fail("floating-point control word preservation");
        }
        victims[0].last_tick = GetTickCount() - 100;
        *(float *)(victim + 0x4c) += 5;
        if (((GameMethod)*uslot)(player) != 0x87654321) return fail("player-driven victim sampling");
        if (GetTickCount() - victims[0].last_tick >= 100) return fail("victim sampled independently of AI callbacks");
        if (((DestroyMethod)*vd)(victim, 73) != 333 || GetLastError() != 333) return fail("victim destructor forwarding/stack arguments");
    }
    end_run("selftest_complete");
    __asm__ volatile("fldcw %0" : : "m"(original_cw));
    if (init_calls != 3 || update_calls != 18) return fail("original callback count");
    if (victim_spawns != 3 || victim_updates != 15 || victim_destroys != 3) return fail("victim original callback count");
    puts("PASS: signatures, player/victim hook forwarding, destructor stack arguments, return values, last-error, FP control, same-stage restarts, stage changes and JSONL output");
    return 0;
}
