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

void BigUint::_left_shift_(uint32_t n) {
  if (*this == 0 || n == 0) {
    return;
  }
  const uint x = n / 32, y = n % 32;
  if (y == 0) {
    _left_shift32_(x);
  } else {
    const uint32_t low = (1u << (32 - y)) - 1;
    const uint32_t high = ~low;
    const int old_size = _data.size();
    _data.resize(old_size + x + 1);
    for (int i = old_size - 1; i >= 0; --i) {
      _data[i + x + 1] = (_data[i + 1] &  low) << y;
      _data[i + x + 1] |= (_data[i] & high) >> (32 - y);
    }
    _data[x] = (_data[0] &  low) << y;
    for (int i = x - 1; i >= 0; --i) {
      _data[i] = 0;
    }
    if (_data.back() == 0 && _data.size() > 1) {
      _data.pop_back();
    }
    assert (_data.back() != 0 || _data.size() == 1);
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

void BigUint::_right_shift32_(uint s) {
  if (*this == 0 || s == 0) {
    return;
  }
  uint old_size = _data.size();
  int new_size = old_size - s;
  for (int i = 0; i < new_size; ++i) {
    _data[i] = _data[i + s];
  }
  if (new_size <= 0) {
    _set_uint32_(0);
  } else {
    _data.resize(new_size);
  }
}

void BigUint::_div_and_mod_(uint32_t n, BigUint& q, uint32_t& r) const {
  assert(n > 0);
  q = *this;
  union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
  _u.u64 = 0;
  for(int i = q._data.size() - 1; i >= 0; --i) {
    _u.u32.l = q._data[i];
    q._data[i] = _u.u64 / n;
    _u.u32.h = _u.u64 % n;
  }
  r = _u.u32.h;
  if (q._data.back() == 0 && q._data.size() > 1) {
    q._data.pop_back();
  }
  assert (q._data.back() != 0 || q._data.size() == 1);
}


BigUint BigUint::_montgomery_(const BigUint& a, const BigUint& b, uint32_t m) const {
  BigUint t{0};
  for (uint i = 0; i < _data.size(); ++i) {
    uint32_t tb = 0;
    if (i < b._data.size()) {
      tb = b._data[i];
    }
    uint32_t q = (a._data[0] * tb + t._data[0]) * m;
    t += *this * q + a * tb;
    t._right_shift32_(1);
    t %= *this;
  }
  return t;
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
  if (_data.back() == 0 && _data.size() > 1) {
    _data.pop_back();
  }
  assert (_data.back() != 0 || _data.size() == 1);
  return *this;
}

BigUint& BigUint::operator*=(uint32_t n) {
  union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
  _u.u32.h = 0;
  for (uint i = 0; i < _data.size(); ++i) {
    _u.u64 = (uint64_t)_data[i] * n + _u.u32.h;
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
  if (_data.back() == 0 && _data.size() > 1) {
    _data.pop_back();
  }
  assert (_data.back() != 0 || _data.size() == 1);
  return *this;
}

BigUint& BigUint::operator%=(uint32_t n) {
  uint32_t k = UINT32_MAX % n + 1;
  uint32_t r = 0;
  for (int i = _data.size() - 1; i >= 0; --i) {
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
  while (_data.back() == 0 && _data.size() > 1) {
    _data.pop_back();
  }
  assert (_data.back() != 0 || _data.size() == 1);
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
  assert (c.back() != 0 || c.size() == 1);
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
    const uint m = _data.size(), n = b._data.size();
    std::vector<uint32_t> c(m - n + 1);
    _data.push_back(0);
    for (int i = m - n; i >= 0; --i) {
      union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
      _u.u32.h = _data[i + n];
      _u.u32.l = _data[i + n - 1];
      int64_t q = _u.u64 / b._data.back();
      ++q;
      BigUint bb;
      do {
        --q;
        bb = b * (uint32_t)q;
        if (bb._data.size() != n + 1) {
          bb._data.resize(n + 1);
        }
      } while (q >= 0 && _compare_uint32_(bb._data.data(), _data.data() + i, bb._data.size()) > 0);
      while (bb._data.back() == 0 && bb._data.size() > 1) {
        bb._data.pop_back();
      }
      c[i] = q;
      bb._left_shift32_(i);
      *this -= bb;
    }
    while (c.back() == 0 && c.size() > 1) {
      c.pop_back();
    }
    assert (c.back() != 0 || c.size() == 1);
    _data = std::move(c);
  }
  return *this;
}

BigUint& BigUint::operator%=(const BigUint& b) {
  if (*this == b) {
    // *this = 0
    _set_uint32_(0);
  } else if (*this > b) { // TODO: need be optimized
    const uint m = _data.size(), n = b._data.size();
    _data.push_back(0);
    for (int i = m - n; i >= 0; --i) {
      union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
      _u.u32.h = _data[i + n];
      _u.u32.l = _data[i + n - 1];
      int64_t q = _u.u64 / b._data.back();
      ++q;
      BigUint bb;
      do {
        --q;
        bb = b * (uint32_t)q;
        if (bb._data.size() != n + 1) {
          bb._data.resize(n + 1);
        }
      } while (q >= 0 && _compare_uint32_(bb._data.data(), _data.data() + i, bb._data.size()) > 0);
      while (bb._data.back() == 0 && bb._data.size() > 1) {
        bb._data.pop_back();
      }
      bb._left_shift32_(i);
      *this -= bb;
    }
    while (_data.back() == 0 && _data.size() > 1) {
      _data.pop_back();
    }
    assert (_data.back() != 0 || _data.size() == 1);
  }
  return *this;
}


BigUint BigUint::mod_mul_inv(uint32_t n) const {
  uint32_t r0 = n, r1;
  BigUint t0{1}, t1;

  _div_and_mod_(n, t1, r1);

  bool neg_0 = false;
  bool neg_1 = true;

  while (r1 != 0) {
    uint32_t q1 = r0 / r1;
    uint32_t r2 = r0 % r1;
    r0 = r1;
    r1 = r2;
    BigUint t2 = t1 * q1;
    bool neg_2 = false;
    if (neg_0 != neg_1) {
      t2 += t0;
      neg_2 = neg_0;
    } else {
      if (t2 >= t0) {
        t2 -= t0;
        neg_2 = neg_1;
      } else {
        t2 = t0 - t1;
        neg_2 = neg_0;
      }
    }
    t0 = std::move(t1);
    t1 = std::move(t2);
    neg_0 = neg_1;
    neg_1 = neg_2;
  }
  if (r0 != 1) {
    t0._set_uint32_(0);
  } else if (neg_0) {
    t0 = *this - t0;
  }
  return t0;
}

BigUint BigUint::mod_pow(const BigUint& b, const BigUint& e) const {
  BigUint t{1};
  t._left_shift32_(1);
  BigUint x = t.mod_mul_inv(_data[0]);
  assert(x > 0);
  t -= std::move(x);
  uint32_t m = t._data[0];

  // bR mod(*this)
  uint len = _data.size();
  BigUint bR{b};
  bR._left_shift32_(len);
  bR %= *this;

  // t = R
  t = 1;
  t._left_shift32_(len);
  t %= *this;

  for (int i = e._data.size() - 1; i >= 0; --i) {
    for (int j = 31; j >= 0; --j) {
      t = _montgomery_(t, t, m);
      if ((e._data[i] & (1 << j)) != 0) {
        t = _montgomery_(t, bR, m);
      }
    }
  }
  t = _montgomery_(t, 1, m);
  return t;
}

} // namespace simple_rsa
