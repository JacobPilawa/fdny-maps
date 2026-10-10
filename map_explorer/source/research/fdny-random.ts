/** Retail FDNY random generator recovered at 0x00404930..0x00404B1F.
 *
 * This is the older MT19937 reference layout: a 624-word state, 69069 state
 * expansion, the standard MT twist/temper, and a 23-bit float assembled by
 * replacing the mantissa of 1.0f. The executable lazily seeds it with 4357.
 */
export class FdnyRandom {
  private readonly state = new Uint32Array(624);
  private index = 624;

  constructor(seed = 4357) {
    this.seed(seed);
  }

  seed(value: number): void {
    let word = (value | 1) >>> 0;
    this.state[0] = word;
    for (let i = 1; i < this.state.length; i++) {
      word = Math.imul(word, 69069) >>> 0;
      this.state[i] = word;
    }
    this.index = this.state.length;
  }

  nextUint32(): number {
    if (this.index >= this.state.length) this.twist();
    let word = this.state[this.index++];
    word ^= word >>> 11;
    word ^= (word << 7) & 0x9d2c5680;
    word ^= (word << 15) & 0xefc60000;
    word ^= word >>> 18;
    return word >>> 0;
  }

  /** Exact float path at 0x00404AC0: [1.mantissa] - 1.0f. */
  nextFloat(): number {
    floatBits.setUint32(0, (this.nextUint32() & 0x007fffff) | 0x3f800000, true);
    return floatBits.getFloat32(0, true) - 1;
  }

  /**
   * Random_Range(0,count-0.01), then x87 FISTP using round-to-nearest-even.
   * The executable's shared upper-bound epsilon is the .rdata double 0.01
   * at VA 0x004DFC78; using count-1 here incorrectly suppresses the final
   * authored alternate spot for most draws.
   */
  spotIndex(count: number): number {
    if (!Number.isInteger(count) || count < 1) throw new RangeError("spot count must be a positive integer");
    return Math.min(count - 1, roundNearestEven(this.nextFloat() * (count - 0.01)));
  }

  private twist(): void {
    for (let i = 0; i < 624; i++) {
      const joined = (this.state[i] & 0x80000000) | (this.state[(i + 1) % 624] & 0x7fffffff);
      this.state[i] = (this.state[(i + 397) % 624] ^ (joined >>> 1) ^ (joined & 1 ? 0x9908b0df : 0)) >>> 0;
    }
    this.index = 0;
  }
}

const floatBits = new DataView(new ArrayBuffer(4));

function roundNearestEven(value: number): number {
  const floor = Math.floor(value);
  const fraction = value - floor;
  if (fraction < 0.5) return floor;
  if (fraction > 0.5) return floor + 1;
  return floor % 2 === 0 ? floor : floor + 1;
}
