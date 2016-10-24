#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <stdint.h>

#define bit(x) (1ll << (x))
#define get_bits(x, l, r) (((x) >> (l)) & (bit((r) - (l)) - 1)) //[l, r)
#define get_bit(x, b) get_bits(x, b, (b) + 1)
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

void* align(void* pointer, uint64_t alignment);
uint64_t ceil(uint64_t a, uint64_t b);

#endif
