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

  void random_bits(int bits);
  int bits() const;

  bool is_odd() const { return (_data[0] & 0x1) != 0; }
  bool is_even() const { return (_data[0] & 0x1) == 0; }

  // most significant uint32
  uint32_t msu() const { return _data.back(); }
  uint32_t& msu() { return _data.back(); }
  // least significant uint32
  uint32_t lsu() const { return _data.front(); }
  uint32_t& lsu() { return _data.front(); }

  bool operator<(uint32_t n) const { return _data.size() == 1 && _data[0] < n; }
  bool operator>(uint32_t n) const { return _data.size() > 1 || _data[0] > n; }
  bool operator==(uint32_t n) const { return _data.size() == 1 && _data[0] == n; }
  bool operator!=(uint32_t n) const { return _data.size() == 1 || _data[0] != n; }
  bool operator<=(uint32_t n) const { return _data.size() == 1 && _data[0] <= n; }
  bool operator>=(uint32_t n) const { return _data.size() > 1 || _data[0] >= n; }

  bool operator<(const BigUint& b) const { return _compare_(b) < 0; }
  bool operator>(const BigUint& b) const { return _compare_(b) > 0; }
  bool operator==(const BigUint& b) const { return _compare_(b) == 0; }
  bool operator!=(const BigUint& b) const { return _compare_(b) != 0; }
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

  // return *this << n
  BigUint& left_shift(uint32_t n) { this->_left_shift_(n); return *this;}
  BigUint left_shift(uint32_t n) const { BigUint b{*this}; b._left_shift_(n); return b;}

  // return *this >> n
  BigUint& right_shift(uint32_t n) { this->_right_shift32_(n); return *this;}
  BigUint right_shift(uint32_t n) const { BigUint b{*this}; b._right_shift_(n); return b;}

  // modular multiplicative inverse, n^(-1) mod(*this)
  BigUint mod_mul_inv(uint32_t n) const;

  // modular exponentiation, b^e mod(*this)
  BigUint mod_pow(const BigUint& b, const BigUint& e) const;

private:
  void _set_uint32_(uint32_t n);
  int _compare_uint32_(const uint32_t *a, const uint32_t *b, int n) const;
  int _compare_(const BigUint& b) const;

  // *this << n
  void _left_shift_(uint32_t n);

  // left shift 32 * n bits
  void _left_shift32_(uint s);

  // *this >> n
  void _right_shift_(uint s);

  // right shift 32 * n bits
  void _right_shift32_(uint s);

  // calculate q = *this / n; r = *this % n;
  void _div_and_mod_(uint32_t n, BigUint& q, uint32_t& r) const;

  // montgomery multiplication, a*b*(R^-1) mod(*this)
  // r = 2^32, R = r^(*this.size()), m == r - (*this[0])^-1,
  // calculate m outside to avoid calculating every time in loop
  BigUint _montgomery_(const BigUint& a, const BigUint& b, uint32_t m) const;

private:
  std::vector<uint32_t> _data;
};


inline BigUint operator+(const BigUint& b, uint32_t n) {
  BigUint c(b);
  return c += n;
}

inline BigUint operator-(const BigUint& b, uint32_t n) {
  BigUint c(b);
  return c -= n;
}

inline BigUint operator*(const BigUint& b, uint32_t n) {
  BigUint c(b);
  return c *= n;
}

inline BigUint operator/(const BigUint& b, uint32_t n) {
  BigUint c(b);
  return c /= n;
}

inline uint32_t operator%(const BigUint& b, uint32_t n) {
  BigUint c(b);
  c %= n;
  return c.lsu();
}


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


extern const uint32_t PRIME_NUMBERS[];
extern const int PRIME_NUMBERS_SIZE;

// make b and all PRIME_NUMBERS are relatively prime
void primer_numbers_test(BigUint& b);

bool miller_rabin_test(const BigUint& b);

} // namespace simple_rsa

#endif // _BIGUINT_H__
