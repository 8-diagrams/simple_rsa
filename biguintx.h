#ifndef _BIGUINT_H__
#define _BIGUINT_H__ 1

#include <cstdint>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>

namespace simple_rsa {

using std::uint32_t;
using std::uint64_t;


namespace {

int compare(const uint32_t *a, const uint32_t *b, int n) {
  for (int i = n - 1; i >= 0; --i) {
    if (a[i] > b[i]) {
      return 1;
    } else if (a[i] < b[i]) {
      return -1;
    }
  }
  return 0;
}

} // namespace


template<int N>
class BigUint {
public:
  BigUint():_data{0} {}
  BigUint(uint32_t n):_data{n} {}
  BigUint(const BigUint& other) {
    std::memcpy(_data, other._data, N + 1);
  }
  ~BigUint() = default;

  BigUint& operator=(const BigUint& other) {
    if (this != &other) {
      std::memcpy(_data, other._data, N + 1);
    }
    return *this;
  }

  bool overflow() const { return _data[N] != 0; }

  // most significant uint32
  uint32_t msu() const { return _data[N -1]; }
  uint32_t& msu() { return _data[N - 1]; }
  // least significant uint32
  uint32_t lsu() const { return _data[0]; }
  uint32_t& lsu() { return _data[0]; }

  bool is_odd() const { return (_data[0] & 0x1) != 0; }
  bool is_even() const { return (_data[0] & 0x1) == 0; }

  // hexadecimal format
  std::string to_string() const {
    std::stringstream ss;
    ss<<"0x";
    for (int i = N - 1; i >= 0; --i) {
      ss<<std::setfill('0')<<std::setw(8)<<std::hex<<_data[i];
    }
    return ss.str();
  }

  void random() {
    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < N; ++i) {
      _data[i] = generator();
    }
    _data[N] = 0;
  }


  bool operator<(const BigUint& b) const { return compare(_data, b._data, N) < 0; }
  bool operator>(const BigUint& b) const { return compare(_data, b._data, N) > 0; }
  bool operator==(const BigUint& b) const { return compare(_data, b._data, N) == 0; }
  bool operator!=(const BigUint& b) const { return compare(_data, b._data, N) != 0; }
  bool operator<=(const BigUint& b) const { return compare(_data, b._data, N) <= 0; }
  bool operator>=(const BigUint& b) const { return compare(_data, b._data, N) >= 0; }

  BigUint& operator+=(uint32_t n) {
    union { struct {uint32_t l, h;} u32; uint64_t u64; } _u;
    _u.u32 = {0, n};
    for (int i = 0; i < N; ++i) {
      _u.u64 = (uint64_t)_data[i] + _u.u32.h;
      _data[i] = _u.u32.l;
      if (_u.u32.h == 0) {
        break;
      }
    }
    _data[N] = _u.u32.h;
    return *this;
  }

  BigUint& operator-=(uint32_t n) {
    union { struct {uint32_t l, h;} u32; uint64_t u64; } _u;
    _u.u32 = {0, -n};
    for (int i = 0; i < N; ++i) {
      _u.u64 = (int64_t)_data[i] + (int32_t)_u.u32.h;
      _data[i] = _u.u32.l;
      if (_u.u32.h == 0) {
        break;
      }
    }
    _data[N] = _u.u32.h;
    return *this;
  }

  BigUint& operator*=(uint32_t n) {
    union { struct {uint32_t l, h;} u32; uint64_t u64; } _u;
    _u.u32.h = 0;
    for (uint i = 0; i < N; ++i) {
      _u.u64 = (uint64_t)_data[i] * n + _u.u32.h;
      _data[i] = _u.u32.l;
    }
    _data[N] = _u.u32.h;
    return *this;
  }

  BigUint& operator/=(uint32_t n) {
    union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
    _u.u64 = 0;
    for(int i = N - 1; i >= 0; --i) {
      _u.u32.l = _data[i];
      _data[i] = _u.u64 / n;
      _u.u32.h = _u.u64 % n;
    }
    return *this;
  }

  uint32_t operator%=(uint32_t n) {
    uint32_t k = UINT32_MAX % n + 1;
    uint32_t r = 0;
    for (int i = N - 1; i >= 0; --i) {
      r = ((uint64_t)r * k + _data[i]) % n;
    }
    return r;
  }

  BigUint& operator+=(const BigUint& b);
  BigUint& operator-=(const BigUint& b);
  BigUint& operator*=(const BigUint& b);
  BigUint& operator/=(const BigUint& b);
  BigUint& operator%=(const BigUint& b);

private:
  uint32_t _data[N + 1];
};

template<int N>
inline BigUint<N> operator+(const BigUint<N>& b, uint32_t n) {
  BigUint<N> c{b};
  return c += n;
}

template<int N>
inline BigUint<N> operator-(const BigUint<N>& b, uint32_t n) {
  BigUint<N> c{b};
  return c -= n;
}

template<int N>
inline BigUint<N> operator*(const BigUint<N>& b, uint32_t n) {
  BigUint<N> c{b};
  return c *= n;
}

template<int N>
inline BigUint<N> operator/(const BigUint<N>& b, uint32_t n) {
  BigUint<N> c{b};
  return c /= n;
}

//template<int N>
//inline uint32_t operator%(const BigUint<N>& b, uint32_t n) {
//  return b.operator%(n);
//}


typedef BigUint<32> uint1024;
typedef BigUint<64> uint2048;

} // namespace simple_rsa

#endif // _BIGUINT_H__
