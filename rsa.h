#ifndef _RSA_H__
#define _RSA_H__ 1

#include <bigint.h>

namespace simple_rsa {

class rsa {
public:
  rsa() {}
  rsa(const rsa&) = delete;
  rsa(rsa&&) = delete;
  ~rsa() = default;

  void init();
private:

};

} // simple_rsa

#endif // _RSA_H__
