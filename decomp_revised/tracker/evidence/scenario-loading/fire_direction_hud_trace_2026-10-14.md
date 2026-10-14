# Retail fire-direction HUD widgets

Audited with Ghidra against the copied retail executable:

- Binary: `inputs/original/binaries/FDNYFirefighter.exe`
- SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`
- Resource table: `inputs/original/Data/ResDefs/rescue.rrf`

## Retail setup and sector mapping

HUD construction calls `004567D0`, which creates four GUI sprite children in
the Player HUD object. It assigns each the shared `ROC_ALPHA_TEXTURE` resource
50, links the child to that sprite actor, sets a 128×128 rectangle initially at
`(256,0)-(384,128)`, and rotates its corners about `(320,240)`. The angles are
`0, -90, -180, -270` degrees in child-construction order. Rectangle setup
`0047EA10` adds the current GUI viewport origin; rotation `0047EB60` uses
`0047E970`'s standard degree-to-radian 2D rotation.

The four direction bytes are not in child-construction order for the middle
two children. `00456090` updates the following visibility fields:

| HUD child field | Player byte | Rotation | Ordered corners in 640×480 UI coordinates |
|---|---|---:|---|
| `HUD +0x3DC` | `Player +0x353C` | 0° | TL `(256,0)`, TR `(384,0)`, BR `(384,128)`, BL `(256,128)` |
| `HUD +0x500` | `Player +0x353E` | −90° | TL `(80,304)`, TR `(80,176)`, BR `(208,176)`, BL `(208,304)` |
| `HUD +0x624` | `Player +0x353D` | −180° | TL `(384,480)`, TR `(256,480)`, BR `(256,352)`, BL `(384,352)` |
| `HUD +0x748` | `Player +0x353F` | −270° | TL `(560,176)`, TR `(560,304)`, BR `(432,304)`, BL `(432,176)` |

The UI coordinates use x-right/y-down. Thus the icons occupy the top, left,
bottom, and right edges; the displayed art itself rotates with each quad. The
port indexes its source-backed Player sectors as front, rear, positive-angle
side, and negative-angle side, so the renderer uses angles `0, -180, -90,
-270` in that order.

## Texture evidence and rendering limits

`rescue.rrf` declares `ROC_ALPHA_TEXTURE 50` with
`data\\sprites\\fireprox.bmp` and `data\\sprites\\fireprox_a.bmp`. Both
copied files are 128×128 8-bit BMPs. The color image has a color palette; the
alpha image has a grayscale palette. Retail's alpha-resource path (`0049AFE0`
→ `00495040` → `00494AD0`) loads the optional alpha bitmap and attaches it via
`Genesis.dll!1001B890` (`geBitmap_SetAlpha`). This establishes the paired
resource and alpha attachment, not the exact mask-channel interpretation in
the renderer, filtering, blend state, or final displayed pixel values.

The copied color BMP SHA-256 is
`3d7d99e6ffd2d35e8c29c52a95b4592eee7c0b09122ea0afaf52669cced035eb`; the
copied alpha BMP SHA-256 is
`e4564bfe54c3357051d7b6c262f95507102e222306b08526c3913993157b2524`.
The project source manifest records both files under `inputs/original/`.

## Port status

`FireDirectionRenderer` now packages the retail pair, combines the grayscale
alpha map with the color pixels, and draws one icon for every active sector
using the recovered viewport geometry. Its standard source-alpha blend and
mask-channel sampling are explicit port approximations pending the Genesis/D3D
GUI render-state trace and same-view retail capture. Runtime activation uses
the current frame's source-mapped indicator bytes; viewer exposure inputs and
callback cadence remain provisional. The viewer scales the 640×480 logical
coordinates independently to drawable width and height; the retail viewport's
resize/stretch convention remains open.
