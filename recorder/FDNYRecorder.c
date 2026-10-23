/* FDNY Recorder v1.1. Single game-thread observer; no gameplay-state writes.
 * Hooks validated Player/Victim vtable entries, preserving original calls and FP state.
 * Addresses refer to the supplied PE32 game, based at 0x00400000.
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define VERSION "1.1"
#define PATH_CAP 1024
typedef uintptr_t (__attribute__((thiscall)) *GameMethod)(void *);
static uintptr_t base;
static HMODULE module;
static GameMethod original_init, original_update;
static int victim_tracking;
static void reset_victims(void);
static void sample_victims(void);
static HANDLE trace = INVALID_HANDLE_VALUE, log_file = INVALID_HANDLE_VALUE;
static char output_dir[PATH_CAP], ini_path[PATH_CAP];
static char buffer[65536];
static DWORD buffered, last_flush, run_tick, sequence, samples;
static void *run_player;
static int enabled = 1, write_failed;
static char run_map[80];

static int read_at(uintptr_t address, void *out, SIZE_T size) {
    SIZE_T got = 0;
    return address && ReadProcessMemory(GetCurrentProcess(), (void *)address, out, size, &got) && got == size;
}
static uintptr_t pointer_at(uintptr_t address) {
    uintptr_t result = 0;
    read_at(address, &result, sizeof result);
    return result;
}
static void log_line(const char *message) {
    if (log_file != INVALID_HANDLE_VALUE) {
        DWORD written;
        WriteFile(log_file, message, (DWORD)strlen(message), &written, NULL);
        WriteFile(log_file, "\r\n", 2, &written, NULL);
    }
}
static int flush_trace(void) {
    if (trace == INVALID_HANDLE_VALUE || !buffered) return 1;
    DWORD offset = 0, written;
    while (offset < buffered) {
        if (!WriteFile(trace, buffer + offset, buffered - offset, &written, NULL) || !written) {
            log_line("ERROR: recording write failed; this run is disabled. Check free disk space and output permissions.");
            CloseHandle(trace); trace = INVALID_HANDLE_VALUE;
            buffered = 0; write_failed = 1;
            return 0;
        }
        offset += written;
    }
    buffered = 0;
    last_flush = GetTickCount();
    return 1;
}
static void append_line(const char *line) {
    DWORD n = (DWORD)strlen(line);
    if (trace == INVALID_HANDLE_VALUE || n >= sizeof buffer) return;
    if (buffered + n > sizeof buffer && !flush_trace()) return;
    memcpy(buffer + buffered, line, n); buffered += n;
    if (GetTickCount() - last_flush >= 1000) flush_trace();
}
static void end_run(const char *reason) {
    if (trace != INVALID_HANDLE_VALUE) {
        char line[256];
        snprintf(line, sizeof line,
            "{\"type\":\"end\",\"reason\":\"%s\",\"samples\":%lu,\"elapsed_ms\":%lu}\n",
            reason, (unsigned long)samples, (unsigned long)(GetTickCount() - run_tick));
        append_line(line); flush_trace();
        if (trace != INVALID_HANDLE_VALUE) { CloseHandle(trace); trace = INVALID_HANDLE_VALUE; }
    }
    buffered = 0; run_player = NULL;
}
static int resolve_map(char *out, size_t cap) {
    uintptr_t session = pointer_at(base + 0x102aac);
    uintptr_t level = pointer_at(session ? session + 0x688 : 0);
    uintptr_t string = pointer_at(level);
    char raw[80] = {0};
    /* Read one byte at a time: do not reject a valid string at a page boundary. */
    if (!string) return 0;
    for (unsigned i = 0; i < sizeof raw - 1; ++i) {
        if (!read_at(string + 12 + i, raw + i, 1)) return 0;
        if (!raw[i]) break;
    }
    if (!raw[0]) return 0;
    const char *name = raw;
    for (unsigned i = 0; raw[i]; ++i) if (raw[i] == '/' || raw[i] == '\\') name = raw + i + 1;
    size_t j = 0;
    for (; name[j] && name[j] != '.' && j + 1 < cap; ++j) {
        char c = name[j];
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
        out[j] = ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '-') ? c : '_';
    }
    out[j] = 0;
    return j != 0;
}
static void start_run(void *player) {
    end_run("next_player_initialization");
    reset_victims();
    write_failed = 0;
    if (!resolve_map(run_map, sizeof run_map)) strcpy(run_map, "unknown_stage");
    SYSTEMTIME utc; GetSystemTime(&utc);
    char filename[PATH_CAP + 180];
    snprintf(filename, sizeof filename,
        "%s\\%s_%04u-%02u-%02uT%02u-%02u-%02u.%03uZ_pid%lu_run%04lu.jsonl",
        output_dir, run_map, utc.wYear, utc.wMonth, utc.wDay, utc.wHour, utc.wMinute,
        utc.wSecond, utc.wMilliseconds, (unsigned long)GetCurrentProcessId(), (unsigned long)++sequence);
    trace = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (trace == INVALID_HANDLE_VALUE) { log_line("ERROR: could not create a run file (never overwrites existing recordings)."); return; }
    run_player = player; samples = 0; buffered = 0; run_tick = last_flush = GetTickCount();
    char line[1400];
    snprintf(line, sizeof line,
        "{\"type\":\"start\",\"schema\":\"fdny-trajectory-v1\",\"recorder_version\":\"" VERSION "\","
        "\"stage\":\"%s\",\"started_utc\":\"%04u-%02u-%02uT%02u:%02u:%02u.%03uZ\","
        "\"run\":%lu,\"victim_tracking\":%s,\"coordinate_system\":\"Genesis3D original units; Y up\","
        "\"sampling\":\"after player root update callback\",\"time_basis\":\"wall clock milliseconds\","
        "\"input_basis\":\"foreground Windows virtual keys, sampled; not resolved game actions\"}\n",
        run_map, utc.wYear, utc.wMonth, utc.wDay, utc.wHour, utc.wMinute, utc.wSecond, utc.wMilliseconds, (unsigned long)sequence, victim_tracking ? "true" : "false");
    append_line(line); flush_trace();
    log_line(filename);
}
static unsigned key_mask(void) {
    DWORD pid = 0;
    GetWindowThreadProcessId(GetForegroundWindow(), &pid);
    if (pid != GetCurrentProcessId()) return 0;
    const int keys[] = {'W','A','S','D',VK_UP,VK_LEFT,VK_DOWN,VK_RIGHT,'C',VK_CONTROL,VK_SPACE,VK_SHIFT,VK_LBUTTON,VK_RBUTTON,'1','2','3'};
    unsigned mask = 0;
    for (unsigned i = 0; i < sizeof keys / sizeof keys[0]; ++i)
        if (GetAsyncKeyState(keys[i]) & 0x8000) mask |= 1u << i;
    return mask;
}
static void sample_player(void *player) {
    if (write_failed) return;
    if (run_player != player) start_run(player); /* Covers late plugin initialization. */
    if (trace == INVALID_HANDLE_VALUE) return;
    uintptr_t p = (uintptr_t)player;
    float position[3], velocity[3], forward[3], pitch, dt, minimum[3], maximum[3];
    unsigned state, slot;
    unsigned char grounded;
    if (!read_at(p + 0x4c, position, sizeof position) ||
        !read_at(p + 0xfc, velocity, sizeof velocity) ||
        !read_at(p + 0x40, forward, sizeof forward) ||
        !read_at(p + 0x3430, &pitch, 4) || !read_at(p + 0x18c, &state, 4) ||
        !read_at(p + 0x357c, &slot, 4) || !read_at(p + 0x12c, &grounded, 1) ||
        !read_at(base + 0x102b04, &dt, 4)) return;
    for (unsigned i = 0; i < 3; ++i)
        if (!isfinite(position[i]) || !isfinite(velocity[i]) || !isfinite(forward[i])) return;
    if (!isfinite(pitch) || !isfinite(dt)) return;
    char bounds[300] = "";
    if (read_at(p + 0x78, minimum, 12) && read_at(p + 0x84, maximum, 12)) {
        int valid = 1;
        for (unsigned i = 0; i < 3; ++i) if (!isfinite(minimum[i]) || !isfinite(maximum[i]) || minimum[i] >= maximum[i]) valid = 0;
        if (valid) snprintf(bounds, sizeof bounds,
            ",\"bounds_min\":[%.9g,%.9g,%.9g],\"bounds_max\":[%.9g,%.9g,%.9g]",
            (double)minimum[0], (double)minimum[1], (double)minimum[2],
            (double)maximum[0], (double)maximum[1], (double)maximum[2]);
    }
    char line[1500];
    snprintf(line, sizeof line,
        "{\"type\":\"sample\",\"n\":%lu,\"t_ms\":%lu,\"game_dt_s\":%.9g,"
        "\"position\":[%.9g,%.9g,%.9g],\"velocity\":[%.9g,%.9g,%.9g],"
        "\"body_forward\":[%.9g,%.9g,%.9g],\"pitch_deg\":%.9g,"
        "\"player_state_raw\":%u,\"ground_contact_raw\":%u,\"tool_slot_raw\":%u,\"keys_mask\":%u%s}\n",
        (unsigned long)samples, (unsigned long)(GetTickCount() - run_tick), (double)dt,
        (double)position[0], (double)position[1], (double)position[2],
        (double)velocity[0], (double)velocity[1], (double)velocity[2],
        (double)forward[0], (double)forward[1], (double)forward[2], (double)pitch,
        state, (unsigned)grounded, slot, key_mask(), bounds);
    append_line(line); ++samples;
    if (victim_tracking) sample_victims();
}
#include "victim-recorder.h"

/* FXSAVE preserves the game's x87 control/status, registers, and SSE state.
 * Windows last-error is preserved too. The callbacks have no stack arguments.
 */
static uintptr_t __attribute__((fastcall)) on_init(void *player, void *unused) {
    (void)unused;
    uintptr_t result = original_init(player);
    unsigned char fp[512] __attribute__((aligned(16)));
    __asm__ volatile("fxsave %0" : "=m"(fp));
    DWORD error = GetLastError();
    start_run(player);
    SetLastError(error);
    __asm__ volatile("fxrstor %0" : : "m"(fp));
    return result;
}
static uintptr_t __attribute__((fastcall)) on_update(void *player, void *unused) {
    (void)unused;
    uintptr_t result = original_update(player);
    unsigned char fp[512] __attribute__((aligned(16)));
    __asm__ volatile("fxsave %0" : "=m"(fp));
    DWORD error = GetLastError();
    sample_player(player);
    SetLastError(error);
    __asm__ volatile("fxrstor %0" : : "m"(fp));
    return result;
}
static int install_hooks(void) {
    const unsigned char init_code[] = {0x55,0x8b,0xec,0x83,0xe4,0xf8};
    const unsigned char update_code[] = {0x83,0xec,0x68,0x56,0x8b,0xf1};
    unsigned char bytes[6];
    uintptr_t *init_slot = (uintptr_t *)(base + 0xe1c68);
    uintptr_t *update_slot = (uintptr_t *)(base + 0xe1c5c);
    if (pointer_at((uintptr_t)init_slot) != base + 0x4d550 ||
        pointer_at((uintptr_t)update_slot) != base + 0x4fe20 ||
        pointer_at(base + 0xe1c40) != base + 0x52570 ||
        !read_at(base + 0x4d550, bytes, 6) || memcmp(bytes, init_code, 6) ||
        !read_at(base + 0x4fe20, bytes, 6) || memcmp(bytes, update_code, 6)) {
        log_line("ERROR: game signatures/vtables differ; recorder installed NO hooks."); return 0;
    }
    /* Both slots are in the same page; one protection change, then two aligned stores. */
    DWORD old;
    if (!VirtualProtect(update_slot, 16, PAGE_READWRITE, &old)) {
        log_line("ERROR: could not make vtable writable; recorder installed NO hooks."); return 0;
    }
    original_init = (GameMethod)*init_slot;
    original_update = (GameMethod)*update_slot;
    InterlockedExchange((volatile LONG *)init_slot, (LONG)(uintptr_t)on_init);
    InterlockedExchange((volatile LONG *)update_slot, (LONG)(uintptr_t)on_update);
    DWORD ignored;
    VirtualProtect(update_slot, 16, old, &ignored);
    log_line("READY: validated player initialization/update observers installed. Automatic stage recording enabled.");
    return 1;
}
static DWORD WINAPI initialize(void *unused) {
    (void)unused;
    char game_dir[PATH_CAP], log_path[PATH_CAP];
    GetModuleFileNameA(module, game_dir, sizeof game_dir);
    char *slash = strrchr(game_dir, '\\'); if (!slash) return 0; *slash = 0;
    snprintf(log_path, sizeof log_path, "%s\\FDNYRecorder.log", game_dir);
    snprintf(ini_path, sizeof ini_path, "%s\\FDNYRecorder.ini", game_dir);
    log_file = CreateFileA(log_path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    log_line("FDNY Recorder " VERSION " initializing");
    enabled = GetPrivateProfileIntA("Recorder", "Enabled", 1, ini_path);
    if (!enabled) { log_line("Disabled in FDNYRecorder.ini; no hooks installed."); return 0; }
    GetPrivateProfileStringA("Recorder", "OutputDirectory", "", output_dir, sizeof output_dir, ini_path);
    if (!output_dir[0]) { log_line("ERROR: OutputDirectory missing in FDNYRecorder.ini; no hooks installed."); return 0; }
    if (!CreateDirectoryA(output_dir, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
        log_line("ERROR: cannot create output directory; no hooks installed."); return 0;
    }
    base = (uintptr_t)GetModuleHandleA(NULL);
    if (*(WORD *)base != IMAGE_DOS_SIGNATURE) return 0;
    if (install_hooks() && GetPrivateProfileIntA("Recorder", "RecordVictims", 1, ini_path)) victim_tracking = install_victim_hooks();
    return 0;
}
#ifndef RECORDER_SELFTEST
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        module = instance;
        DisableThreadLibraryCalls(instance);
        HANDLE thread = CreateThread(NULL, 0, initialize, NULL, 0, NULL);
        if (thread) CloseHandle(thread);
    } else if (reason == DLL_PROCESS_DETACH && reserved) {
        /* Process is ending: game thread stopped, no worker lock or join needed. */
        end_run("process_exit");
        if (log_file != INVALID_HANDLE_VALUE) CloseHandle(log_file);
    }
    return TRUE;
}
#endif
