# Additional map volumes

`entity-volumes.js` provides static overlays derived from the copied entities.

- Pickup contact: native `0x0045A070`, copied in `../research/victim-runtime/0045a070_FUN_0045a070.c`, reads `J_Radius`, uses 60 for nonpositive values and checks inclusive XYZ overlap against the player's body hull. Victim states 5/6 are excluded. Display boxes are centered on authored base and linked alternate candidate positions; the game instead checks the current victim position. This does not predict roaming, eligibility, or live spawning.
- Brush bounds: `build_data.py` resolves every authored `C_AnimModel*` reference through the BSP `%Model%` entities, retaining each model's min/max bounds and name. These envelopes include doors, ladders and breakable/animated brushes. They do not reproduce shape, animation, solidity, or reach checks.
- Fire `H_Radius`/`H_Height` and prop `F_TouchRadius` exist in the raw entity fields. Their precise runtime contact shapes are not established by these fields alone, so no new exact collision overlays are inferred from them.

All new overlay selections are session-only and reset off on map load or Reset.
