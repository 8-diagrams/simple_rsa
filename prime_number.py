#!/usr/bin/env python3

import sys

def generate_file(primes):
  print('#include "biguint.h"')
  print()
  print("namespace simple_rsa {")
  print()
  print("const uint32_t PRIME_NUMBERS[] = {", end="")
  xlen = len(primes)
  m = 10
  idx = 0
  for i in range(xlen):
    p = primes[i]
    if idx % m == 0:
      print("\n  ", end="")
    else:
      print(" ", end="")
    print("{: #7x}".format(p), end="")
    if idx != xlen - 1:
      print(",", end="")
    idx += 1
  print("\n};")
  print()
  print("const int PRIME_NUMBERS_SIZE = sizeof(PRIME_NUMBERS) / sizeof(uint32_t);")
  print()
  print("} // namespace simple_rsa")
  print()

def main(script, limit):
  limit = int(limit)
  primes = list(range(3, limit, 2))
  xlen = len(primes)
  for i in range(xlen):
    p = primes[i]
    if p == 0:
      continue
    for j in range(i + 1, xlen):
      x = primes[j]
      if x != 0 and x % p == 0:
        primes[j] = 0
  primes = [p for p in primes if p != 0]
  generate_file(primes)

if __name__ == '__main__':
  main(*sys.argv)
