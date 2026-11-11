#ifndef BITS_H
#define BITS_H

#include <stdint.h>

static inline uint32_t set_bit(uint32_t reg, unsigned n) {
    return reg | (1u << n);
}

static inline uint32_t clear_bit(uint32_t reg, unsigned n) {
    return reg & ~(1u << n);
}

static inline uint32_t toggle_bit(uint32_t reg, unsigned n) {
    return reg ^ (1u << n);
}

static inline unsigned test_bit(uint32_t reg, unsigned n) {
    return (reg >> n) & 1u;
}

static inline uint32_t set_field(uint32_t reg, uint32_t v, unsigned shift, unsigned width) {
   uint32_t mask = (width >= 32u) ? 0xFFFFFFFFu : ((1u << width) - 1u);
   reg &= ~(mask << shift);
   v = (v & mask) << shift;
   return reg | v;
}

#endif
