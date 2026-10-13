/** FireRescueEnt::Spray at 0x00448970. Health starts at one. The particle
 * influence is nonlinear: 1-(1-influence)^2. A terminal particle douses the
 * fire once health is strictly below 0.1. A zero authored spray time therefore
 * reaches zero on its first positive hit, matching Training's small fires.
 */
export function applyFireSpray(
  health: number, dt: number, sprayTime: number, influence = 1, terminalParticle = true,
): { health: number; doused: boolean } {
  const f = Math.fround;
  if (dt <= 0 || health <= 0) return { health: Math.max(0, health), doused: health < 0.1 && terminalParticle };
  // The x87 expression in FireRescueEnt::Spray has no intermediate stores:
  //   health -= (dt / sprayTime) * (1 - (1 - influence) * (1 - influence))
  // Only the member write rounds to float32. Keep the JS expression intact and
  // perform that one explicit store rather than rounding each subexpression.
  const strength = 1 - (1 - influence) * (1 - influence);
  const loss = sprayTime === 0 ? Infinity : (dt / sprayTime) * strength;
  const next = Math.max(0, f(health - loss));
  return { health: next, doused: terminalParticle && next < f(0.1) };
}
