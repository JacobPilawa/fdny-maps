# FDNY map explorer

All 17 April 30 maps, original labels, complete entity tables, source BSPs, 34 original PDFs, scripts, and relevant AI/RNG research are copied into this folder. The app loads map data on demand and uses local JavaScript dependencies; it needs no internet connection.

Run `./start.sh`, then open http://127.0.0.1:4180 . Requires Python 3. Set `FDNY_MAP_PORT` to use another port.

- Select a map at the top. All sidebar groups start collapsed on opening or switching maps.
- Drag to pan in 2D; scroll to zoom. Click or hover a marker for details. Expand a category to Show all, Hide all, turn Labels on/off for that category, or adjust individual objects and labels with Aa. Search includes scripts and raw fields.
- Map data → Entity table contains all ten original PDF table columns, without shortened scripts. Click a label to locate it. Original CSV, source scripts, and full extracted data are available there.
- In 3D, drag to orbit, right-drag to pan, and scroll to zoom. Fly starts above the player spawn or selected entity: WASD moves, Q/E descends/ascends, Shift speeds up, drag looks around, and scroll changes speed. Speed starts at 600 engine units/second, uses smooth acceleration/braking, and stays the same across maps. The slider/readout and scroll adjust one shared speed; ↺ resets it. F toggles Fly in 3D (press again to return to orbit). The Fit button restores the overall map view; F still fits in 2D. Escape exits Fly.
- Right-click a 3D wall/surface to hide it; right-drag still pans. The visible Undo button or Ctrl/⌘ Z restores the last hidden surface. You can also left-click a surface and choose Hide surface (or H). The map tools menu retains Undo hidden wall and Show all walls. Cut roof hides roofs; Y sliders isolate heights.
- Beside the right-click hint, **Collisions** shows transparent red blocking BSP hulls and outlines; **Textures** applies original BSP textures with neutral lighting. **Triggers** beside them shows yellow authored bounding boxes. Use each trigger row’s ▧ button (or Box in its inspector) to select individual boxes; the overall checkbox shows a partial state when only some are enabled. Trigger boxes work in both 2D and 3D, start off on opening/switching maps, and follow object visibility and Y filtering. One Oil Rig B trigger has no authored model bounds and its box button is disabled. Collisions and Textures are off on every fresh page load and load their data only when enabled. They work independently and with height/roof filtering. Hiding a rendered surface leaves its collision overlay visible; Undo restores the surface in either material mode.
- The tools menu also holds trigger volumes, illustrative roam guides, spawn connections, measurements, image export, original PDF, and Reset.

The collapsed **More volumes** menu beside the 3D overlay controls adds green **Pickup boxes** and purple **Brush bounds**. Both start off and reset off on changing maps. Each relevant entity's inspector has an individual toggle, also usable in 2D. Pickup boxes use the victim's `J_Radius` on all three axes (fallback 60) at the base and alternate spawn candidates: 116 candidates across all maps. The native contact check is copied at `source/research/victim-runtime/0045a070_FUN_0045a070.c`; it tests player-body overlap and excludes victim states 5/6. These are static candidate positions, not a live actor simulation or roam radius. Brush bounds include all linked `C_AnimModel*` pieces, rather than only the main door model: 203 boxes for 137 entities. They show authored model envelopes, not exact collision geometry or interaction reach. Fire radius/height fields are available in the inspector's raw data, but their exact damage-volume shape has not been verified and is not presented as a collision volume.

Collision hulls use the original convex leaf-side planes and movement mask 0x43. Non-blocking event trigger models are excluded, internal solid faces are removed, and the unbounded exterior is cropped to the displayed map. Brush models stay at their authored positions: door movement, script-dependent solidity, player hull expansion, and actor collision are not simulated. Fly remains a free camera.

Geometry is decoded from the retail BSP with its actual heights, displayed without game textures or live simulation. Marker visibility is independent of walls so hidden victims remain discoverable. Surface hiding affects only 3D and can be undone. Visibility and label choices are saved per map in this browser. Measurements use engine units; roam circles are illustrative, not walkable-area predictions.

`source/notes/VICTIM_AI_RNG.md` explains roaming, ordering, RNG uncertainties, and `settimescale`, linking copied native evidence. Each map's source directory has a victim roster. Earlier research is preserved as evidence; the summary identifies findings superseded by later work.

`source/build_collision.py` rebuilds collision overlays and `source/build_textures.py` decodes palettes and authored UVs into deduplicated PNGs. `source/build_data.py` rebuilds compact data from the copied source maps. Original annotation files and PDFs are kept intact. `vendor/THREE_LICENSE.txt` contains the Three.js license. All dependencies and source assets are copied locally; the shareable app is also published below.

## Cloudflare publication

Public site: https://fdny-maps.piececounter.workers.dev/

`wrangler.json` deploys only Workers Static Assets, with no custom Worker code, database, KV, R2, Durable Objects, scheduled jobs, or paid cache product. Static asset requests and storage are free under Cloudflare's current pricing. The build rejects packages above the Free limit of 20,000 files or 25 MiB per file.

Run `python3 deploy/build_static.py` from this folder, then `npx wrangler deploy --config wrangler.json` using your Cloudflare login. The build uses an explicit public file list and content-versioned URLs with one-year browser caching. The root page revalidates so future deployments load the current version. Original BSPs, conversation archives, local research notes, deployment credentials, and local tools stay outside the public package. GitHub is not required for direct CLI deployment.

Pricing reference: https://developers.cloudflare.com/workers/static-assets/billing-and-limitations/
Limits reference: https://developers.cloudflare.com/workers/platform/limits/

### Optional inspection tools

The left sidebar has a small **Rooms / zones** selector and a collapsed **Motion preview** panel. Choose a motion to switch to 3D and focus it; drag its time slider, play/pause, or reset. Uncheck Preview to restore the authored geometry. Major events include warehouse catwalks, tower stairs, club rafters, bank collapses, and the boat cargo-door/hook lift; the other moving controllers are available individually. Motion tracks are decoded directly from BSP chunk 24, including compressed timing, model pivots and authored playback rates. Textures, collisions and linked brush bounds follow the same transforms. This inspects object motion rather than running a whole stage; camera shaking, sound, particles, open-away direction and conditional gameplay are not simulated.

**Object inventory**, **Bonus objects**, **Refills**, and **Emitters** are separate collapsed groups, off by default. Existing PDF labels stay intact; supplemental inventory uses O/B/FX labels. Selecting a brush in 3D opens its controller when its category is enabled or its motion is being previewed. Non-brush props and emitters are selectable locations with resource IDs and scripts, rather than newly rendered prop models or particle effects.

The map-tools menu has **Trigger / target connections**, off by default. Select an object to isolate incoming/outgoing arrows; its inspector lists clickable targets. These are possible authored `tell`/`action` links after macro expansion, not a claim that every branch runs. Player actions point to the authored player start marker.

In the inspector, hover a macro button for a short explanation, click it for a command table, or use **Explain** for the entire object's scripts. **Conditions / consequences** also opens a compact table. Original and expanded definitions remain available alongside the plain-language meanings; conditions belong to their own branches.

The 3D **Victim models** checkbox loads original ACT geometry and appearance textures on demand. Bodies use the authored type, heading, skin command and initial pose, at the base spawn or the explicitly selected alternate spot. This does not choose a random spawn, roam, or simulate rescue AI. All new overlays and previews reset off on map changes/reloads; panels start collapsed.

Rebuild supplemental map data with `python3 source/build_data.py`, rebuild victim assets from the copied ACTs with `python3 source/build_actor_assets.py`, and run the small data/motion check with `node source/check_features.mjs`. Both decoders and their source assets are included in this repository. The deployment remains static-only Cloudflare assets; animation and actor files are fetched only when needed, with no R2, KV, database or Worker execution added.
