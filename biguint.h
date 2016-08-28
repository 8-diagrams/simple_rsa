#ifndef _BIGUINT_H__
#define _BIGUINT_H__ 1

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

namespace simple_rsa {

using std::uint32_t;
using std::uint64_t;
typedef unsigned int uint;

class BigUint {
public:
  BigUint():_data{0} {}
  BigUint(uint32_t n):_data{n} {}
  BigUint(const BigUint& other):_data{other._data} {}
  BigUint(BigUint&& rhs):_data{std::move(rhs._data)} {}
  ~BigUint() = default;

  BigUint& operator=(const BigUint& other) {
    if (this != &other) {
      _data = other._data;
    }
    return *this;
  }
  BigUint& operator=(BigUint&& rhs) {
    if (this != &rhs) {
      _data = std::move(rhs._data);
    }
    return *this;
  }

  // hexadecimal format
  std::string to_string() const;

  // not exactly, multiple of 32
  void shrink_to_fit();

  // not exactly, multiple of 32
  void random_bits(uint bits);

  bool operator<(const BigUint& b) const { return _compare_(b) < 0; }
  bool operator>(const BigUint& b) const { return _compare_(b) > 0; }
  bool operator==(const BigUint& b) const { return _compare_(b) == 0; }
  bool operator<=(const BigUint& b) const { return _compare_(b) <= 0; }
  bool operator>=(const BigUint& b) const { return _compare_(b) >= 0; }

  BigUint& operator+=(uint32_t n);
  BigUint& operator-=(uint32_t n);
  BigUint& operator*=(uint32_t n);
  BigUint& operator/=(uint32_t n);
  BigUint& operator%=(uint32_t n);
  BigUint& operator+=(const BigUint& b);
  BigUint& operator-=(const BigUint& b);
  BigUint& operator*=(const BigUint& b);
  BigUint& operator/=(const BigUint& b);
  BigUint& operator%=(const BigUint& b);

private:
  int _compare_uint32_(const uint32_t *a, const uint32_t *b, int n) const;
  int _compare_(const BigUint& b) const;
  // left shift 32 * n bits
  void _left_shift32_(uint s);

private:
  std::vector<uint32_t> _data;
};

inline BigUint operator+(const BigUint& b1, const BigUint& b2) {
  BigUint c(b1);
  return c += b2;
}

inline BigUint operator-(const BigUint& b1, const BigUint& b2) {
  BigUint c(b1);
  return c -= b2;
}

inline BigUint operator*(const BigUint& b1, const BigUint& b2) {
  BigUint c(b1);
  return c *= b2;
}

inline BigUint operator/(const BigUint& b1, const BigUint& b2) {
  BigUint c(b1);
  return c /= b2;
}

inline BigUint operator%(const BigUint& b1, const BigUint& b2) {
  BigUint c(b1);
  return c %= b2;
}

} // namespace simple_rsa

#endif // _BIGUINT_H__
