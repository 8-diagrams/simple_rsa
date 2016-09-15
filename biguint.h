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

  // modular multiplicative inverse, n^(-1) mod(*this)
  BigUint mod_mul_inv(uint32_t n) const;

private:
  void _set_uint32_(uint32_t n);
  int _compare_uint32_(const uint32_t *a, const uint32_t *b, int n) const;
  int _compare_(const BigUint& b) const;
  // left shift 32 * n bits
  void _left_shift32_(uint s);
  // calculate q = *this / n; r = *this % n;
  void _div_and_mod_(uint32_t n, BigUint& q, uint32_t& r) const;
  // calculate q = *this / b; r = *this % b;
  void _div_and_mod_(const BigUint& b, BigUint& q, BigUint& r) const;

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
