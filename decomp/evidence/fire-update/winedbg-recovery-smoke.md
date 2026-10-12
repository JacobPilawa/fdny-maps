# WineDbg bounded-recovery smoke test

Date: 2026-10-11. This validates the recorder's WineDbg command/recovery path on a harmless process; it is not a game capture.

## Setup and result

- Cloned the installed app's Wine prefix into disposable `/private/tmp/fdny-winedbg-safe-66gvqi9y`; did not launch or change the user's original prefix or game.
- Started the bundled Wine `ping.exe` in that cloned prefix. WineDbg `info proc` identified it as Windows PID `0xC4`.
- Launched WineDbg without a positional PID, reached its prompt, then attached with the explicit hexadecimal command `attach 0xc4`. The command returned to a prompt and `info reg` exposed the target's `RIP`.
- Forced the debugger continuation to time out after two seconds. WineDbg received Ctrl-C, stopped the debuggee, and returned to a prompt. `detach` returned to a prompt, then `quit` exited WineDbg.
- A post-detach process listing still showed the ping target alive. The smoke harness then stopped that disposable target and its cloned-prefix Wine server and removed the temporary prefix.

The machine-readable result was `deadline_interrupt=true`, `detach_confirmed=true`, `target_survived_detach=true`, PID `000000c4`. Raw `result.json`, WineDbg transcript, and ping output remain local under `evidence/live/winedbg-safe-20261011-233125/`; live debugger outputs remain excluded from the public mirror.

## What this proves

The bundled WineDbg console accepts CR-terminated commands; attach works when the PID has an explicit `0x` hexadecimal prefix; the 64-bit harmless target provides a register dump; and this forced timeout can be interrupted, detached, and left running. The recorder's recovery sequence has therefore been exercised independently of the game.

This does not prove that attaching to the PE32 FDNY game is stable, that the game's modules and fixed breakpoints are correct at runtime, or that any Training callback runs. The Fire Think caller, cadence and frame order are still unmeasured. The next capture requires the matching game at its main menu and user-driven entry into Training; it is capped at five minutes with 30 seconds reserved for detach recovery.

## PE32 follow-up attempt

A follow-up used the cloned prefix's `SysWOW64/ping.exe` (confirmed PE32 / Intel 80386) to validate the target architecture. The ping launch produced no useful error, but `winedbg.exe --command "info proc"` did not return within 15 seconds, before a PID was identified or any attach was attempted. The process and prefix were stopped/removed with `wineserver -k` scoped to that disposable prefix. No game process or original prefix was touched. Raw output is under `evidence/live/winedbg-safe-20261011-pe32-234623/` locally.

Therefore the recovery path is demonstrated only for the 64-bit harmless process. This PE32 ping attempt is inconclusive about WineDbg attach itself; previous user-game attempts did enumerate the FDNY process but then stalled at interactive commands. The bounded recorder's actual PE32-game behavior still needs the user-driven menu capture.

WineDbg command reference: [WineDbg manual](https://github.com/wine-mirror/wine/blob/master/programs/winedbg/winedbg.man.in).
