// Exact exponential response: the same travel distance at different frame rates.
export const DEFAULT_FLIGHT_SPEED = 600;
export function integrateFlight(velocity, target, dt, response = 18) {
  const decay = Math.exp(-response * dt);
  return {
    velocity: velocity.map((v, i) => target[i] + (v - target[i]) * decay),
    displacement: velocity.map((v, i) => target[i] * dt + (v - target[i]) * (1 - decay) / response)
  };
}
