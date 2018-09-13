class RandomGenerator {
public:
  RandomGenerator(uint64_t seed = 0) : seed(seed), state(0) {}
  uint32_t next(uint32_t upper_bound = UINT32_MAX) {
    uint64_t H = seed >> 32;
    uint64_t L = seed & UINT32_MAX;

    if (state & 2) {
      seed += H * 498627377 + (L ^ UINT32_MAX) * 694379069;
    } else {
      seed += L * 498627377 + (H ^ UINT32_MAX) * 694379069;
    }
    seed += (++state) * 2644400359;
    seed ^= ((state ^ UINT32_MAX) * 3669561283);

    H = seed >> 32;
    L = seed & UINT32_MAX;
    if (state & 1) {
      seed = (H ^ UINT32_MAX) * 1257677461 + L * 864277937;
    } else {
      seed = H * 1257677461 + (L ^ UINT32_MAX) * 864277937;
    }
    seed += 16303300055571991331ULL;
    if (state & 4) {
      seed = ((seed & 0x5555555555555555LL) << 1) |
             ((seed >> 1) & 0x5555555555555555LL);
    }
    if (state & 8) {
      seed = ((seed & 0x3333333333333333L) << 2) |
             ((seed >> 2) & 0x3333333333333333LL);
    }

    return seed % upper_bound;
  }
private:
  uint64_t seed, state;
};
