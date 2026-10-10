import assert from "node:assert/strict";
import test from "node:test";
import { chooseVictimEscape, RETAIL_ESCAPE_OFFSETS, RETAIL_ROAM_SPEED } from "./victim-ai.ts";

test("victim escape probes use the native four-corner order", () => {
  const seen: number[] = [];
  const result = chooseVictimEscape((offset) => { seen.push(offset.x * 10 + offset.z); return offset.x < 0; });
  assert.deepEqual(seen, [55, 45, -45]);
  assert.deepEqual(result, RETAIL_ESCAPE_OFFSETS[2]);
});

test("victim returns to downed state when all escape probes are blocked", () => {
  assert.equal(chooseVictimEscape(() => false), null);
});

test("roaming victims use the recovered native 36 units/second speed", () => {
  assert.equal(RETAIL_ROAM_SPEED, 36);
});
