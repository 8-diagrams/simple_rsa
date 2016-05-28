#include <iostream>
#include "bigint.h"

using namespace std;
using namespace simple_rsa;

typedef bigint<32> uint1024;

int main() {
  uint1024 a, b;
  for (int i = 0 ; i < 1000; ++i) {
    a.random_init();
    b.random_init();
    auto c = a + b;
    auto d = a < b ? b - a : a - b;
    cout<<a.to_string()<<","<<b.to_string()<<","
        <<c.to_string()<<","<<d.to_string()<<std::endl;
  }
}
