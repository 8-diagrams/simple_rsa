#include <vector>
#include "biguint.h"

namespace simple_rsa {

void primer_numbers_test(BigUint& b) {
  if (b.is_even()) {
    b += 1;
  }
  std::vector<uint32_t> remainders(PRIME_NUMBERS_SIZE);
  for (int i = 0; i < PRIME_NUMBERS_SIZE; ++i) {
    remainders[i] = b % PRIME_NUMBERS[i];
  }
  bool ok = true;
  do {
    ok = true;
    for (int i = 0; i < PRIME_NUMBERS_SIZE; ++i) {
      if (remainders[i] == 0) {
        ok = false;
      }
    }
    if (!ok) {
      b += 2;
      for (int i = 0; i < PRIME_NUMBERS_SIZE; ++i) {
        remainders[i] = (remainders[i] + 2) % PRIME_NUMBERS[i];
      }
      continue;
    }
  } while (!ok);
}

} // namespace simple_rsa
