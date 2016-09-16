#include <iostream>
#include <chrono>
#include <random>
#include "biguint.h"

using namespace std;
using namespace simple_rsa;

void test(const BigUint& a, const BigUint& b, uint32_t n) {
  string sa = a.to_string();
  string sb = b.to_string();
  auto c = a.mod_mul_inv(n);
  if (c > 0) {
    string sc = c.to_string();
    cout<<"mod_mul_inv "<<sa<<" 0x"<<hex<<n<<" "<<sc<<endl;
  }
  if (a >= b) {
    auto d = a.mod_pow(b, n);
    string sd = d.to_string();
    cout<<"mod_pow "<<sa<<" "<<b.to_string()<<" 0x"<<hex<<n<<" "<<sd<<endl;
  }
}

int main() {
  BigUint a;
  BigUint b;
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  for (int i = 0 ; i < 1000; ++i) {
    a.random_bits((generator() % 1024) + 1);
    b.random_bits((generator() % 1024) + 1);
    if (a.is_even()) {
      a += 1;
    }
    uint32_t n = generator();
    test(a, b, n);
  }
}
