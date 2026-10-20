# Axe time-zero frame-record consumers (2026-10-20)

Read-only review of retained Ghidra evidence for the pinned retail executable:

- `FDNYFirefighter.exe` SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`
- `GENESIS.DLL` SHA-256: `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`

## Helper path

Player action status 3 calls `00455BC0` on Player `+0xA94`. That helper enables the ActorInfo, stores the current tick, samples four values at time `0.0` through `00484940`, then passes them to `0047EDF0` and takes the `0047E6D0` record-copy path. The saved sampler export `00484940_vector_keyframe_sampler_fresh.c` returns four integer fields from its keyframe data.

Separate saved widget traces show menu constructors `00435A10` and `00435B30` passing rectangle coordinates through `0047EDF0`; the latter initializes rectangle records and calls `0047EA10`. The fire-direction HUD trace identifies `0047EA10` as 2D quad setup that adds the GUI viewport origin. Together these consumers identify the helper family as GUI rectangle/frame operations. They do not establish that the particular Player `+0xA94` record is drawn by a GUI owner.

## Limits

No Axe ACT mesh consumer or 3D pose-reset path is established for the sampled values. No direct draw consumer for the specific Player `+0xA94` frame record was found in retained evidence. This review used saved hash-pinned Ghidra exports and bounded searches of `tracker/evidence` and `reference`; `mcp__ghidra__list_instances` reported no running instance, so this is not a fresh complete binary-wide xref census. Runtime visibility, helper cadence, and retail/port parity remain unverified.

Supporting evidence: [`player_bmodel_action_status_fresh_2026-10-20.c`](player_bmodel_action_status_fresh_2026-10-20.c), [`00484940_vector_keyframe_sampler_fresh.c`](00484940_vector_keyframe_sampler_fresh.c), [`00435a10_menu_widget_fresh.c`](00435a10_menu_widget_fresh.c), [`00435b30_menu_widget_fresh.c`](00435b30_menu_widget_fresh.c), and [`fire_direction_hud_trace_2026-10-14.md`](fire_direction_hud_trace_2026-10-14.md).
