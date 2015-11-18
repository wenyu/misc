// A simple big integer library for C++.
// By Wenyu Zhang

#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

#define PBB pair<BigInt, BigInt>
#define PBU32 pair<BigInt, uint_fast32_t>
typedef vector<uint_fast32_t> BINARY;

class BigInt {
public:
  BigInt() {
    sign = false;
    bits.clear();
    dirty = true;
  }

  BigInt(int_fast64_t v) {
    bits.resize(2);
    if (v < 0) {
      v = -v;
      sign = true;
    } else {
      sign = false;
    }
    bits[0] = v;
    bits[1] = v >> 32;
    compress();
  }

  const BigInt & operator+=(const BigInt &rhs) {
    if (sign == rhs.sign) {
      bits = add(bits, rhs.bits);
    } else if (cmp(*this, rhs, false)) {
      sign = !sign;
      bits = sub(rhs.bits, bits);
    } else {
      bits = sub(bits, rhs.bits);
    }
    compress();
    return *this;
  }

  const BigInt & operator-=(const BigInt &rhs) {
    if (sign != rhs.sign) {
      bits = add(bits, rhs.bits);
    } else if (cmp(*this, rhs, false)) {
      sign = !sign;
      bits = sub(rhs.bits, bits);
    } else {
      bits = sub(bits, rhs.bits);
    }
    compress();
    return *this;
  }

  const BigInt & operator*=(const BigInt &rhs) {
    BINARY r(0);
    for (int i = 0; i < rhs.bits.size(); ++i) {
      r = add(r, mul(bits, rhs.bits[i], i));
    }
    bits = r;
    sign ^= rhs.sign;
    compress();
    return *this;
  }

  PBB divide(const BigInt &divisor) const {
    PBB result = div(*this, divisor);
    result.first.sign = bits.size() && (sign != divisor.sign);
    result.second.sign = result.second.bits.size() && sign;
    return result;
  }

  const BigInt & operator/=(const BigInt &rhs) {
    *this = divide(rhs).first;
    return *this;
  }

  const BigInt & operator%=(const BigInt &rhs) {
    *this = divide(rhs).second;
    return *this;
  }

  const BigInt & operator>>=(const int rhs) {
    if (rhs < 0) return operator<<=(rhs);

    int drop = rhs >> 5;
    if (drop >= bits.size()) {
      bits.resize(0);
      sign = false;
      return *this;
    }

    int shift_down = rhs & 31;
    int shift_up = 32 - shift_down;
    int i, j;

    for (i = 0, j = drop; j + 1 < bits.size(); ++i, ++j) {
      bits[i] = (bits[j] >> shift_down) | ((uint_fast64_t) bits[j + 1] << shift_up);
    }
    bits[i] = bits[j] >> shift_down;

    bits.resize(bits.size() - drop);
    compress();
    return *this;
  }

  const BigInt & operator<<=(const int rhs) {
    if (rhs < 0) return operator>>=(-rhs);

    int pad = rhs >> 5;
    bits.resize(bits.size() + pad + 1);

    int shift_up = rhs & 31;
    int shift_down = 32 - shift_up;
    int i, j;

    for (i = bits.size() - 1, j = i - pad; j > 0; --i, --j) {
      bits[i] = (bits[j] << shift_up) | ((uint_fast64_t) bits[j - 1] >> shift_down);
    }

    bits[i--] = bits[j] << shift_up;

    while (i >= 0) {
      bits[i--] = 0;
    }

    compress();
    return *this;
  }

  const BigInt & operator&=(const BigInt &rhs) {
    bits.resize(min(bits.size(), rhs.bits.size()));

    for (register int i = 0; i < bits.size(); ++i) {
      bits[i] &= rhs.bits[i];
    }

    compress();
    return *this;
  }

  const BigInt & operator|=(const BigInt &rhs) {
    bits.resize(max(bits.size(), rhs.bits.size()));

    for (register int i = 0; i < bits.size(); ++i) {
      bits[i] |= rhs.bits[i];
    }

    compress();
    return *this;
  }

  const BigInt & operator^=(const BigInt &rhs) {
    bits.resize(max(bits.size(), rhs.bits.size()));

    for (register int i = 0; i < bits.size(); ++i) {
      bits[i] ^= rhs.bits[i];
    }

    compress();
    return *this;
  }

  const BigInt & operator~() {
    for (register int i = 0; i < bits.size(); ++i) {
      bits[i] = ~bits[i];
    }

    compress();
    return *this;
  }

  BigInt(const char *s) { from_c_str(s); }
  BigInt(string s) { from_c_str(s.c_str()); }
  const bool operator==(const BigInt &rhs) const { return equ(*this, rhs); }
  const bool operator!=(const BigInt &rhs) const { return !equ(*this, rhs); }
  const bool operator<(const BigInt &rhs) const { return cmp(*this, rhs); }
  const bool operator<=(const BigInt &rhs) const { return !cmp(rhs, *this); }
  const bool operator>(const BigInt &rhs) const { return cmp(rhs, *this); }
  const bool operator>=(const BigInt &rhs) const { return !cmp(*this, rhs); }
  const BigInt operator+(const BigInt &rhs) const { BigInt result(*this); return result += rhs; }
  const BigInt operator-(const BigInt &rhs) const { BigInt result(*this); return result -= rhs; }
  const BigInt operator*(const BigInt &rhs) const { BigInt result(*this); return result *= rhs; }
  const BigInt operator/(const BigInt &rhs) const { return divide(rhs).first; }
  const BigInt operator%(const BigInt &rhs) const { return divide(rhs).second; }
  const BigInt operator&(const BigInt &rhs) const { BigInt result(*this); return result &= rhs; }
  const BigInt operator|(const BigInt &rhs) const { BigInt result(*this); return result |= rhs; }
  const BigInt operator^(const BigInt &rhs) const { BigInt result(*this); return result ^= rhs; }
  const BigInt operator>>(const int rhs) const { BigInt result(*this); return result >>= rhs; }
  const BigInt operator<<(const int rhs) const { BigInt result(*this); return result <<= rhs; }
  const BigInt operator-() const { BigInt result(*this); result.sign = !sign; return result; }
  const BigInt & operator++() { *this += 1; return *this; }
  const BigInt operator++(int) { BigInt result(*this); ++*this; return result; }
  const BigInt & operator--() { return *this -= 1; return *this; }
  const BigInt operator--(int) { BigInt result(*this); --*this; return result; }
  operator bool() const { return bits.size(); }
  operator int() const { return to_int(); }
  operator long long() const { return to_long_long(); }
  operator string() { return to_string(); }
  string str() { return to_string(); }

  friend ostream & operator<<(ostream &os, BigInt n) {
    os << n.to_string();
    return os;
  }

  friend istream & operator>>(istream &is, BigInt &n) {
    string s;
    is >> s;
    n = s;
    return is;
  }

  int log2() const {
    if (!bits.size()) return 0;
    return ((bits.size() - 1) << 5) + highest_bit(bits.back());
  }

  PBB sqrt2() const {
    if (sign) throw -2;
    int bits_required = log2();
    if (bits_required & 1) ++bits_required;
    BigInt x(*this);
    BigInt e = BigInt(1) << bits_required;
    BigInt r;
    while (e.bits.size()) {
      BigInt t = r | e;
      r >>= 1;
      if (t <= x) {
        x -= t;
        r |= e;
      }
      e >>= 2;
    }
    return make_pair(r, x);
  }

  const BigInt sqrt() const {
    return sqrt2().first;
  }

private:
  bool sign, dirty;
  BINARY bits;
  string decimal;

  void compress() {
    while (bits.size() && !bits.back()) bits.pop_back();
    if (!bits.size()) sign = false;
    dirty = true;
  }

  int to_int() const {
    int r = 0;
    if (bits.size()) r = bits[0];
    if (sign) r = -r;
    return r;
  }

  long long to_long_long() const {
    long long r = 0;
    if (bits.size() >= 1) r = bits[0];
    if (bits.size() >= 2) r |= ((uint_fast64_t) bits[1]) << 32;
    if (sign) r = -r;
    return r;
  }

  string to_string() {
    if (!dirty) return decimal;
    dirty = false;
    if (!bits.size()) {
      decimal = "0";
      return decimal;
    }
    stringstream stream;
    if (sign) stream << "-";
    BINARY groups;
    groups.clear();
    PBU32 qr = make_pair(*this, 0u);
    while (qr.first) {
      qr = div32(qr.first, 1000000000);
      groups.push_back(qr.second);
    }

    while (!groups.empty()) {
      stream << groups.back();
      stream.width(9);
      stream.fill('0');
      groups.pop_back();
    }

    decimal = stream.str();
    return decimal;
  }

  void from_c_str(const char *s) {
    bits.clear();
    if (*s == '-') {
      sign = true;
      ++s;
    }
    BigInt quintillion(1000000000000000000LL);
    int len;
    for (len = 0; '0' <= s[len] && s[len] <= '9'; ++len) ;
    int chunk_count = len / 18, extra = len % 18, i;
    long long chunk = 0;

    while (extra--) {
      chunk = chunk * 10 + (*(s++) - '0');
    }
    *this = chunk;

    while (chunk_count--) {
      chunk = 0;
      for (i = 18; i; --i) {
        chunk = chunk * 10 + (*(s++) - '0');
      }
      *this *= quintillion;
      *this += chunk;
    }
  }

  static const BINARY add(const BINARY &a, const BINARY &b) {
    return a.size() < b.size() ? _add(b, a) : _add(a, b);
  }

  inline static const BINARY _add(const BINARY &a, const BINARY &b) {
    register int i = 0;
    uint_fast64_t c = 0;
    BINARY r(a);

    for (; i < b.size(); ++i, c >>= 32) {
      c += (uint_fast64_t) a[i] + b[i];
      r[i] = c;
    }

    for (; c && i < a.size(); ++i, c >>= 32) {
      c += (uint_fast64_t) a[i];
      r[i] = c;
    }

    if (c) r.push_back(c);

    return r;
  }

  static const BINARY sub(const BINARY &a, const BINARY &b) {
    register int i = 0;
    uint_fast64_t c = 1;
    BINARY r(a);

    for (; i < b.size(); ++i, c >>= 32) {
      c += (uint_fast64_t) a[i] + (0xFFFFFFFF ^ b[i]);
      r[i] = c;
    }

    for (; c != 1 && i < r.size(); ++i, c >>= 32) {
      c += (uint_fast64_t) a[i] + 0xFFFFFFFF;
      r[i] = c;
    }

    return r;
  }

  static const BINARY mul(const BINARY &a, uint_fast32_t m, int shift = 0) {
    if (!m) return BINARY();
    register int i = 0;
    uint_fast64_t c = 0;
    BINARY r(shift, 0);
    r.reserve(shift + a.size() + 1);

    for (i = 0; i < a.size(); ++i, c >>= 32) {
      c += (uint_fast64_t) a[i] * m;
      r.push_back(c);
    }

    for (; c; c >>= 32) {
      r.push_back(c);
    }

    return r;
  }

  static bool equ(const BigInt &a, const BigInt &b) {
    if ((a.sign != b.sign) || (a.bits.size() != b.bits.size())) return false;
    for (register int i = 0; i < a.bits.size(); ++i) {
      if (a.bits[i] != b.bits[i]) return false;
    }
    return true;
  }

  static bool cmp(const BigInt &a, const BigInt &b, bool check_sign = true) {
    if (check_sign && a.sign != b.sign) return a.sign;
    if (a.bits.size() != b.bits.size()) return (a.bits.size() < b.bits.size()) ^ (a.sign && check_sign);
    for (register int i = a.bits.size() - 1; i >= 0; --i) {
      if (a.bits[i] != b.bits[i]) return (a.bits[i] < b.bits[i]) ^ (a.sign && check_sign);
    }
    return false;
  }

  static PBB div(BigInt r, BigInt d) {
    if (!d.bits.size()) throw -1;
    if (d.bits.size() == 1) {
      PBU32 t = div32(r, d.bits[0]);
      return make_pair(t.first, BigInt((int_fast64_t) t.second));
    }
    r.sign = false; d.sign = false;
    if (r < d) return make_pair(BigInt(), r);
    int bits_required = r.log2() - d.log2();
    BigInt b(1), q;
    b <<= bits_required;
    d <<= bits_required;

    while (bits_required >= 0 && r.bits.size() > 0) {
      if (d <= r) {
        q |= b;
        r -= d;
      }
      int to_shift = max(1, d.log2() - r.log2());
      b >>= to_shift;
      d >>= to_shift;
      bits_required -= to_shift;
    }

    return make_pair(q, r);
  }

  static PBU32 div32(BigInt q, uint_fast32_t d) {
    uint_fast64_t r = 0;
    for (int i = q.bits.size() - 1; i >= 0; --i) {
      r = (r << 32) + q.bits[i];
      q.bits[i] = r / d;
      r %= d;
    }
    q.compress();
    return make_pair(q, r);
  }

  static int highest_bit(uint_fast32_t n) {
    int r = 0, b = 1 << 4, t;
    uint_fast32_t c;

    while (b) {
      t = b | r;
      c = 1 << t;
      if (c <= n) r = t;
      b >>= 1;
    }

    return r;
  }
};

#undef PBU32
#undef PBB

#endif // __BIGINT_H__
