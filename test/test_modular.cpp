#include <iostream>
#include <chrono>
#include <random>
#include "biguint.h"

using namespace std;
using namespace simple_rsa;

void test(const BigUint& a, uint32_t b) {
  auto c = a.mod_mul_inv(b);
  if (c > 0) {
    string sa = a.to_string();
    string sc = c.to_string();
    cout<<"mod_mul_inv "<<sa<<" 0x"<<hex<<b<<" "<<sc<<endl;
  }
}

int main() {
  BigUint a;
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  for (int i = 0 ; i < 10000; ++i) {
    a.random_bits((generator() % 1024) + 1);
    uint32_t n = generator();
    test(a, n);
  }
}
