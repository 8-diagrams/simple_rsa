#include <iostream>
#include "bigint.h"

using namespace std;
using namespace simple_rsa;

typedef bigint<32> uint1024;

int main() {
  uint1024 a, b;
  for (int i = 0 ; i < 10000; ++i) {
    a.random_init_all();
    b.random_init_all();
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
