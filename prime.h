#ifndef _PRIME_H__
#define _PRIME_H__ 1

#include <cstdint>
#include "biguint.h"

namespace simple_rsa {

extern const uint32_t PRIME_NUMBERS[];
extern const int PRIME_NUMBERS_SIZE;

// make b and all PRIME_NUMBERS are relatively prime
void primer_numbers_test(BigUint& b);

bool miller_rabin_test(const BigUint& b);

} // namespace simple_rsa

#endif // _PRIME_H__
