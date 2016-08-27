#include <iostream>
#include <chrono>
#include <random>
#include "biguint.h"

using namespace std;
using namespace simple_rsa;

int main() {
  BigUint a;
  a.random_bits(1);
  BigUint b(a);
  auto c = a - b;
  cout<<a.to_string()<<" - "<<b.to_string()<<" = "<<c.to_string()<<endl;

  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  for (int i = 0 ; i < 10000; ++i) {
    a.random_bits((generator() % 2048) + 1);
    b.random_bits((generator() % 2048) + 1);
    auto c = a + b;
    if (c < a || c < b) {
      continue;
    }
    cout<<a.to_string()<<" + "<<b.to_string()<<" = "<<c.to_string()<<endl;
    if (a > b) {
      c = a - b;
      cout<<a.to_string()<<" - "<<b.to_string()<<" = "<<c.to_string()<<endl;
    } else {
      c = b - a;
      cout<<b.to_string()<<" - "<<a.to_string()<<" = "<<c.to_string()<<endl;
    }
  }
}
