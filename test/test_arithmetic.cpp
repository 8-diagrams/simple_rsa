#include <iostream>
#include <chrono>
#include <random>
#include "biguint.h"

using namespace std;
using namespace simple_rsa;

void test(const BigUint& a, const BigUint& b) {
  auto c = a + b;
  string sa = a.to_string();
  string sb = b.to_string();
  cout<<sa<<" + "<<sb<<" = "<<c.to_string()<<endl;
  if (a > b) {
    c = a - b;
    cout<<sa<<" - "<<sb<<" = "<<c.to_string()<<endl;
  } else {
    c = b - a;
    cout<<sb<<" - "<<sa<<" = "<<c.to_string()<<endl;
  }
  c = a * b;
  cout<<sa<<" * "<<sb<<" = "<<c.to_string()<<endl;
  c = a / b;
  cout<<sa<<" / "<<sb<<" = "<<c.to_string()<<endl;
}

int main() {
  BigUint a, b;
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  for (int i = 0 ; i < 10000; ++i) {
    a.random_bits((generator() % 1024) + 1);
    b.random_bits((generator() % 1024) + 1);
    uint32_t n = generator();
    test(a, n);
    test(a, b);
  }
}
