import assert from "node:assert/strict";
import test from "node:test";
import { FdnyRandom } from "./fdny-random.ts";

test("retail lazy seed 4357 reproduces the original MT19937 words", () => {
  const random = new FdnyRandom();
  assert.deepEqual(
    Array.from({ length: 5 }, () => random.nextUint32()),
    [3510405877, 4290933890, 2191955339, 564929546, 152112058],
  );
});

test("retail victim selection uses 23-bit float plus nearest-even rounding", () => {
  const random = new FdnyRandom();
  assert.deepEqual([random.spotIndex(3), random.spotIndex(3)], [1, 2]);
});

test("a three-spot picker has the executable's epsilon-bounded distribution", () => {
  const random = new FdnyRandom();
  const counts = [0, 0, 0];
  for (let i = 0; i < 200_000; i++) counts[random.spotIndex(3)]++;
  const fractions = counts.map((count) => count / 200_000);
  assert.ok(Math.abs(fractions[0] - (0.5 / 2.99)) < 0.005, String(fractions));
  assert.ok(Math.abs(fractions[1] - (1 / 2.99)) < 0.005, String(fractions));
  assert.ok(Math.abs(fractions[2] - (1.49 / 2.99)) < 0.005, String(fractions));
});
