#ifndef _BIGINT_H__
#define _BIGINT_H__ 1

#include <cassert>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <random>

namespace simple_rsa {

template<int N>
class bigint {
public:
  bigint():_data{0} {}
  bigint(uint32_t n):_data{n} {}
  bigint(const bigint&) = default;
  bigint(bigint&&) = default;
  ~bigint() = default;

  bigint& operator=(bigint&& other) = default;
  bigint& operator=(const bigint& rhs) = default;

  std::string to_string() const {
    std::stringstream ss;
    ss<<"0x";
    for (int i = N - 1; i >= 0; --i) {
      ss<<std::setfill('0')<<std::setw(8)<<std::hex<<_data[i];
    }
    return ss.str();
  }

  void random_init_all() {
    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < N; ++i) {
      _data[i] = generator();
    }
  }

  void random_init_half() {
    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < N / 2; ++i) {
      _data[i] = generator();
    }
  }

  bool operator<(uint32_t n) const { return comapre(n) < 0; }
  bool operator>(uint32_t n) const { return comapre(n) > 0; }
  bool operator==(uint32_t n) const { return comapre(n) == 0; }
  bool operator<=(uint32_t n) const { return comapre(n) <= 0; }
  bool operator>=(uint32_t n) const { return comapre(n) >= 0; }

  bool operator<(const bigint& b) const { return comapre(b) < 0; }
  bool operator>(const bigint& b) const { return comapre(b) > 0; }
  bool operator==(const bigint& b) const { return comapre(b) == 0; }
  bool operator<=(const bigint& b) const { return comapre(b) <= 0; }
  bool operator>=(const bigint& b) const { return comapre(b) >= 0; }

  bigint& operator+=(uint32_t n) {
    union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
    _u.u32 = {0, n};
    for (int i = 0; i < N; ++i) {
      _u.u64 = (uint64_t)_data[i] + _u.u32.h;
      _data[i] = _u.u32.l;
      if (_u.u32.h == 0) {
        break;
      }
    }
    return *this;
  }

  bigint& operator+=(const bigint& b) {
    union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
    _u.u64 = 0;
    for (int i = 0; i < N; ++i) {
      _u.u64 = (uint64_t)_data[i] + b._data[i] + _u.u32.h;
      _data[i] = _u.u32.l;
    }
    return *this;
  }

  bigint& operator-=(uint32_t n) {
  assert(*this > n);
  union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
    _u.u32 = {0, -n};
    for (int i = 0; i < N; ++i) {
      _u.u64 = (int64_t)_data[i] + (int32_t)_u.u32.h;
      _data[i] = _u.u32.l;
    }
    return *this;
  }

  bigint& operator-=(const bigint& b) {
    assert(*this > b);
    union { struct {uint32_t l, h;} u32; uint64_t u64;} _u;
    _u.u64 = 0;
    for (int i = 0; i < N; ++i) {
      _u.u64 = (int64_t)_data[i] -(int64_t)b._data[i] + (int32_t)_u.u32.h;
      _data[i] = _u.u32.l;
    }
    return *this;
  }

  bigint operator+(uint32_t n) const {
    bigint x{*this};
    x += n;
    return x;
  }
  bigint operator+(const bigint& b) const {
    bigint x{*this};
    x += b;
    return x;
  }
  bigint operator-(uint32_t n) const {
    bigint x{*this};
    x -= n;
    return x;
  }
  bigint operator-(const bigint& b) const {
    bigint x{*this};
    x -= b;
    return x;
  }

private:

  int comapre(const bigint& b) const {
    int cmp = 0;
    for (int i = N-1; i >= 0; --i) {
      if(_data[i] < b._data[i]) {
        cmp = -1;
        break;
      }
      if(_data[i] > b._data[i]) {
        cmp = 1;
        break;
      }
    }
    return cmp;
  }

private:
  uint32_t _data[N];
};

} // namespace simple_rsa

#endif // _BIGINT_H__
