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

void BigUint::_set_uint32_(uint32_t n) {
  _data.resize(1);
  _data[0] = n;
}

int BigUint::_compare_uint32_(const uint32_t *a, const uint32_t *b, int n) const {
  for (int i = n - 1; i >= 0 ; --i) {
    if (a[i] > b[i]) {
      return 1;
    } else if (a[i] < b[i]) {
      return -1;
    }
  }
  return 0;
}

int BigUint::_compare_(const BigUint& b) const {
  uint la = _data.size();
  uint lb = b._data.size();
  if (la > lb) {
    return 1;
  } else if (la < lb) {
    return -1;
  } else {
    return _compare_uint32_(_data.data(), b._data.data(), la);
  }
}

void BigUint::_left_shift32_(uint s) {
  if (*this == 0 || s == 0) {
    return;
  }
  uint old_size = _data.size();
  _data.resize(old_size + s);
  for (int i = old_size - 1; i >= 0; --i) {
    _data[i + s] = _data[i];
  }
  for (uint i = 0; i < s; ++i) {
    _data[i] = 0;
  }
}

void BigUint::_div_and_mod_(const BigUint& b, BigUint& q, BigUint& r) const {
  assert(*this > b);
  r = *this;
  uint m = r._data.size(), n = b._data.size();
  q._data.resize(m - n + 1);
  r._data.push_back(0);
  for (int i = m - n; i >= 0; --i) {
    union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
    _u.u32.h = r._data[i + n];
    _u.u32.l = r._data[i + n - 1];
    uint32_t t = _u.u64 / b._data.back();
    BigUint bb = b * t;
    while (_compare_uint32_(bb._data.data(), r._data.data() + i, bb._data.size()) > 0) {
      --t;
      bb = b * t;
    }
    q._data[i] = t;
    bb._left_shift32_(i);
    r -= bb;
  }
  if (q._data.back() == 0 && q._data.size() > 1) {
    q._data.pop_back();
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
  union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
  _u.u32 = {0, n};
  for (uint i = 0; i < _data.size(); ++i) {
    _u.u64 = (uint64_t)_data[i] * (int32_t)_u.u32.h;
    _data[i] = _u.u32.l;
  }
  if (_u.u32.h > 0) {
    _data.push_back(_u.u32.h);
  }
  return *this;
}

BigUint& BigUint::operator/=(uint32_t n) {
  assert(n > 0);
  union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
  _u.u64 = 0;
  for(int i = _data.size() - 1; i >= 0; --i) {
    _u.u32.l = _data[i];
    _data[i] = _u.u64 / n;
    _u.u32.h = _u.u64 % n;
  }
  if (_data.size() > 1 && *_data.end() == 0) {
    _data.pop_back();
  }
  return *this;
}

BigUint& BigUint::operator%=(uint32_t n) {
  uint32_t k = UINT32_MAX % n + 1;
  uint32_t r = 0;
  for (int i = (int)_data.size(); i >= 0; ++i) {
    r = ((uint64_t)r * k + _data[i]) % n;
  }
  _data.resize(1);
  _data.front() = r;
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
  for (int i = 0; i < (int)_data.size(); ++i) {
    uint32_t x = i < (int)b._data.size() ? b._data[i] : 0;
    _u.u64 = (int64_t)_data[i] -(int64_t)x + (int32_t)_u.u32.h;
    _data[i] = _u.u32.l;
  }
  return *this;
}

BigUint& BigUint::operator*=(const BigUint& b) {
  uint m = _data.size(), n = b._data.size();
  std::vector<uint32_t> c(m + n);
  for (uint i = 0; i < m; ++i) {
    union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
    _u.u64 = 0;
    for (uint j = 0; j < n; ++j) {
      _u.u64 = (uint64_t)_data[i] * b._data[j]  + c[i + j] + _u.u32.h;
      c[i + j] = _u.u32.l;
    }
    c[i + n] = _u.u32.h;
  }
  while (c.back() == 0 && c.size() > 1) {
    c.pop_back();
  }
  _data = std::move(c);
  return *this;
}

BigUint& BigUint::operator/=(const BigUint& b) {
  assert(b > 0);
  if (*this == b) {
    // *this = 1
    _set_uint32_(1);
  } else if (*this < b) {
    // *this = 0
    _set_uint32_(0);
  } else {
    BigUint q, r;
    _div_and_mod_(b, q, r);
    *this = std::move(q);
  }
  return *this;
}

BigUint& BigUint::operator%=(const BigUint& b) {
  if (*this == b) {
    // *this = 0
    _set_uint32_(0);
  } else if (*this > b) {
    BigUint q, r;
    _div_and_mod_(b, q, r);
    *this = std::move(r);
  }
  return *this;
}

} // namespace simple_rsa
