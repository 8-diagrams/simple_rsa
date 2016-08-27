#include <cassert>
#include <chrono>
#include <iomanip>
#include <random>
#include <sstream>

#include "biguint.h"

namespace simple_rsa {

std::string BigUint::to_string() const {
  std::stringstream ss;
  ss<<"0x";
  for (int i = _data.size() - 1; i >= 0; --i) {
    ss<<std::setfill('0')<<std::setw(8)<<std::hex<<_data[i];
  }
  return ss.str();
}

void BigUint::shrink_to_fit() {
  _data.shrink_to_fit();
}

void BigUint::random_bits(uint bits) {
  assert(bits > 0);
  uint n = (bits + 31) >> 5;
  _data.resize(n);
  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  for (uint i = 0; i < n; ++i) {
    _data[i] = generator();
  }
  // make sure the most significant uint32_t > 0
  while (_data[n - 1] == 0) {
    _data[n - 1] = generator();
  }
}

int BigUint::_compare_(const BigUint& b) const {
  uint la = _data.size();
  uint lb = b._data.size();
  if (la > lb) {
    return 1;
  } else if (la < lb) {
    return -1;
  } else {
    for (int i = la - 1; i >= 0 ; --i) {
      if (_data[i] > b._data[i]) {
        return 1;
      } else if (_data[i] < b._data[i]) {
        return -1;
      }
    }
    return 0;
  }
}

BigUint& BigUint::operator+=(uint32_t n) {
  union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
  _u.u32 = {0, n};
  for (uint i = 0; i < _data.size(); ++i) {
    _u.u64 = (uint64_t)_data[i] + _u.u32.h;
    _data[i] = _u.u32.l;
    if (_u.u32.h == 0) {
      break;
    }
  }
  if (_u.u32.h > 0) {
    _data.push_back(_u.u32.h);
  }
  return *this;
}

BigUint& BigUint::operator-=(uint32_t n) {
  assert(*this >= n);
  union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
  _u.u32 = {0, -n};
  for (uint i = 0; i < _data.size(); ++i) {
    _u.u64 = (int64_t)_data[i] + (int32_t)_u.u32.h;
    _data[i] = _u.u32.l;
    if (_u.u32.h == 0) {
      break;
    }
  }
  if (_data.size() > 1 && *_data.end() == 0) {
    _data.pop_back();
  }
  return *this;
}

BigUint& BigUint::operator*=(uint32_t n) {
  return *this;
}

BigUint& BigUint::operator/=(uint32_t n) {
  return *this;
}

BigUint& BigUint::operator+=(const BigUint& b) {
  union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
  _u.u64 = 0;
  uint n = _data.size() >= b._data.size() ? _data.size() : b._data.size();
  if (n > _data.size()) {
    _data.resize(n);
  }
  for (uint i = 0; i < n; ++i) {
    uint32_t x = i < b._data.size() ? b._data[i] : 0;
    _u.u64 = (uint64_t)_data[i] + x + _u.u32.h;
    _data[i] = _u.u32.l;
  }
  if (_u.u32.h > 0) {
    _data.push_back(_u.u32.h);
  }
  return *this;
}

BigUint& BigUint::operator-=(const BigUint& b) {
  assert(*this >= b);
  union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
  _u.u64 = 0;
  for (int i = 0; i < _data.size(); ++i) {
    uint32_t x = i < b._data.size() ? b._data[i] : 0;
    _u.u64 = (int64_t)_data[i] -(int64_t)x + (int32_t)_u.u32.h;
    _data[i] = _u.u32.l;
  }
  return *this;
}

BigUint& BigUint::operator*=(const BigUint& b) {
  return *this;
}

BigUint& BigUint::operator/=(const BigUint& b) {
  return *this;
}


} // namespace simple_rsa
