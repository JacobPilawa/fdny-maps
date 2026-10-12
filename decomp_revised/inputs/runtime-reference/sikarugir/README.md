# Sikarugir/Wine runtime reference

The source bundle is `/Users/jacobpilawa/Applications/Sikarugir/FDNYFirefighterFOVFix.app`. Only the game-specific overlay files and wrapper metadata needed to describe future reference captures were copied here. The 1.4 GB Wineskin bundle, user saves, logs, caches, and its bundled Wine runtime were not copied.

The bundled `FDNYFirefighter.exe`, `Genesis.dll`, `fmod.dll`, and `Drivers/D3DDrv.dll` have the same SHA-256 values as the pinned retail files listed in `inputs/reference/raw-copy/README.md`. Thus static analysis still targets the same retail code. Runtime captures through this wrapper must be tagged as Wine/Wineskin, DXVK, and DirectX-wrapper observations.

The wrapper is configured to select the `(D3D)dgVoodoo DirectX Wrapper`, with its game configuration set to 640x480. Its copied overlay includes `dinput8.dll`, DirectX compatibility DLLs, `dgVoodoo.conf`, and `FDNYFirefighterAmericanHeroFOVFix.asi`. The copied FOV fix configuration enables a 1920x1080 output with `FOVFactor=1.4`; this changes presentation relative to an unmodified retail run. Do not use these settings as port-parity defaults.

Files under `game-overrides/` and `wrapper-metadata/` are provenance and capture-condition references, not original retail inputs and not assets to ship in the port. Keep future runtime captures separate from original-code static evidence and record the active wrapper, patch, resolution, and API translation layer.
