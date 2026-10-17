# Retail menu widget trace — 2026-10-17

## Provenance

Fresh static decompilation used the existing `analysis/ghidra/FDNYPort` project
with Ghidra 12.0.4 headless, `-readOnly -noanalysis`, and the existing
`tools/ghidra-scripts/DecompileTargetAndRefs.java` script. The retail program
was `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The
headless runs did not modify the project and did not launch a UI. The root
callback at `00430DF0` has no saved function boundary, so its instructions were
inspected in a temporary read-only Ghidra project containing the same-hash EXE.

New decompilations preserved here:

- [`0042f670_menu_trace_fresh.c`](../scenario-loading/0042f670_menu_trace_fresh.c)
- [`0042f600_menu_trace_fresh.c`](../scenario-loading/0042f600_menu_trace_fresh.c)
- [`0042d460_menu_trace_fresh.c`](../scenario-loading/0042d460_menu_trace_fresh.c)
- [`0042d3e0_menu_trace_fresh.c`](../scenario-loading/0042d3e0_menu_trace_fresh.c)
- [`0042cf10_menu_trace_fresh.c`](../scenario-loading/0042cf10_menu_trace_fresh.c)
- [`00430f00_menu_widget_fresh.c`](../scenario-loading/00430f00_menu_widget_fresh.c)
- [`00435a10_menu_widget_fresh.c`](../scenario-loading/00435a10_menu_widget_fresh.c)
- [`00435b30_menu_widget_fresh.c`](../scenario-loading/00435b30_menu_widget_fresh.c)
- [`004357d0_menu_widget_fresh.c`](../scenario-loading/004357d0_menu_widget_fresh.c)
- [`00435990_menu_widget_fresh.c`](../scenario-loading/00435990_menu_widget_fresh.c)
- [`00435df0_menu_widget_fresh.c`](../scenario-loading/00435df0_menu_widget_fresh.c)
- [`00436630_menu_widget_fresh.c`](../scenario-loading/00436630_menu_widget_fresh.c)
- [`00435840_menu_widget_fresh.c`](../scenario-loading/00435840_menu_widget_fresh.c)
- [`004358d0_menu_widget_fresh.c`](../scenario-loading/004358d0_menu_widget_fresh.c)
- [`00435780_menu_widget_fresh.c`](../scenario-loading/00435780_menu_widget_fresh.c)
- [`00435d80_menu_widget_fresh.c`](../scenario-loading/00435d80_menu_widget_fresh.c)

Related already-reviewed functions: `0042DEC0`, `0042F280`, `0042F500`, and
`0042D1B0`; see the menu resource-load, level-selection, and bitmap-display
evidence linked from
[`retail_main_menu_art_port_2026-10-17.md`](retail_main_menu_art_port_2026-10-17.md).

## Supported menu behavior

- Root page constructor `00430F00` loads background bitmap ID 10 and control
  alpha texture ID 100. Its four controls use action values 0–3 and select
  quarter-height rows of `mcontrols.bmp`: GAME, SCORES, OPTIONS, and EXIT. In
  the 640x480 menu surface their hit widths are `{100,116,128,70}` and their
  origins are `{50,190,346,514}`; they share authored top-origin y range
  440–472. The `menu.rrf` texture record pairs `mcontrols.bmp` with
  `mcontrols_a.bmp`.
- The root's vtable callback at `00430DF0` maps action 0 to menu child 1,
  action 1 to child 4, action 2 to child 3, and action 3 to child 8 through
  `0042D3E0`. These target child indices are confirmed; the child page contents
  and every transition remain only partly traced. The root child is constructed
  with the career title bitmap, and child 3 is the options page.
- Generic button click handler `00435780` requests playback from the menu
  sound object at parent offset `+0x208`, then forwards its authored action
  code to the parent callback. Exact sound resource, playback timing, and
  keyboard/mouse event producer are still unresolved. Button activation and
  update functions `004358D0` and `00435840` initialize and advance an alpha
  layer using global frame delta; the precise highlight appearance and timing
  constants have not been runtime-compared.
- `0042DEC0` loads `data\\resdefs\\menu.rrf` and establishes the retail
  640x480 menu coordinate surface. Its child index 2 is constructed by
  `0042F670`, the level-selector widget.
- `0042F670` initializes controls that use RRF bitmap IDs 51, 52, 53, 54, and
  55, and the NY map bitmap ID 23. The RRF names these resources `gobtn`,
  `resumebtn`, `restartbtn`, `leftarrow`, `mapdot`, and `nymap`. The first
  button hit rectangle is `(510,420)` to `(638,484)` in the authored menu
  coordinates; the next two button rectangles are offset left by 128 pixels.
  The left/right arrow child controls emit event values 5 and 6.
- `0042F500` handles those events: 5 decrements the selected catalog ordinal;
  6 increments it, clamps to the catalog range, and probes for a valid record.
  Event values 0 and 1 pass the selected level record to
  `ProcessLevelLoadRequest`; value 1 also switches the menu child to index 3.
  The precise distinction between 0 and 1 is still unresolved.
- `0042F280` resolves the selected record, loads its authored briefing bitmap,
  formats its title text, and places the selection marker using record fields.
  `0042F600` activates the widget by restoring the active record when the
  global state allows it, otherwise searching backward for a valid selection.
- `0042D3E0` deactivates the current menu child and activates the requested
  child. `0042D460` moves the current child selection backward by two, bounded
  at zero. The caller-specific meaning of that navigation remains open.

## Port impact and remaining trace work

The native title now uses original `main.bmp` and the paired `mcontrols.bmp` /
`mcontrols_a.bmp` assets at the retail authored control origins. The title
control labels, action codes, and child-index routing are source-backed. The
port still bypasses retail child 1's career page and enters its eight-row
rescue list directly from GAME. SCORES and OPTIONS labels are visible but
their retail pages are not yet ported; the current button highlight treatment
and arrow-key navigation are provisional. EXIT and Escape leave the port.

The separate eight-row mission list still does not reproduce the retail
single-record selector, authored briefing image, title, map, marker, Go/Resume/
Restart controls, or back-state behavior. Existing trace supports those source
elements and catalog actions, but the event meaning distinction between 0 and
1 remains open. Exact selector layout/visibility, page order for remaining
children, keyboard/mouse/controller event production, fade timing, and audio
playback still need more tracing. Static disassembly does not establish runtime
appearance or timing; the current menu is closer to retail at its root page but
is not a full retail front-end match.
