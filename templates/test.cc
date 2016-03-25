#include "bigint.h"
#include "fenwick.h"

template <typename T>
void assert_fenwick_range(const Fenwick<T> &t, int left, int right, const T &expected) {
  T r = t.get(left, right);
  cout << ( ( r == expected ) ? "PASS" : "FAIL" ) << ": Fenwick[" << left << ".." << right << "] = " << r << ", expected " << expected << endl;
}

template <typename T>
void test_fenwick() {
  Fenwick<T> fenwick(100000);
  for (int i = 0; i < 100000; ++i) {
    fenwick.add(i, i + 1);
  }

  assert_fenwick_range(fenwick, 0, 0, T(1));
  assert_fenwick_range(fenwick, 1, 1, T(2));
  assert_fenwick_range(fenwick, 0, 9, T(55));
  assert_fenwick_range(fenwick, 1, 10, T(65));
  assert_fenwick_range(fenwick, 0, 99999, T(5000050000));
  assert_fenwick_range(fenwick, 1, 99999, T(5000049999));
  assert_fenwick_range(fenwick, 50000, 60000, T(550065001));

  for (int i = 50000; i <= 60000; ++i) {
    fenwick.add(i, -fenwick.get(i, i));
  }

  assert_fenwick_range(fenwick, 50000, 60000, T(0LL));
  assert_fenwick_range(fenwick, 0, 99999, T(4449984999));
}

void assert_sqrt(const BigInt& base, const BigInt &extra) {
  BigInt square = base * base + extra;
  pair<BigInt, BigInt> result = square.sqrt2();
  cout << ((result.first == base && result.second == extra) ? "PASS" : "FAIL");
  cout << ": sqrt2(" << square << ") = <" << result.first << ", " << result.second << ">, ";
  cout << "expected <" << base << ", " << extra << ">." << endl;
}

void test_sqrt() {
  assert_sqrt(BigInt(), BigInt());
  assert_sqrt(BigInt(1), BigInt());
  assert_sqrt(BigInt(2), BigInt());
  assert_sqrt(BigInt(2), BigInt(1));
  assert_sqrt(BigInt(2), BigInt(2));
  assert_sqrt(BigInt(10), BigInt(2));
  assert_sqrt(BigInt("31415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679"), BigInt("27182818280618"));
  BigInt t;
  for (int i = 1; i <= 999; ++i) {
    t = (t * BigInt(1000)) + BigInt(i);
  }
  assert_sqrt(t, BigInt(1234));
}

void assert_equals(const BigInt& a, const BigInt& b) {
  cout << (a == b ? "PASS" : "FAIL") << ": a = " << a << ", b = " << b << endl;
}

int main() {
  cout << "Testing BigInt" << endl;
  assert_equals(BigInt(65536) * BigInt(65536), BigInt(string("4294967296")));
  assert_equals(BigInt(4294967296L), BigInt(string("4294967296")));
  assert_equals(BigInt(4294967295L), BigInt(string("4294967295")));

  cout << "Testing Fenwick<long long>" << endl;
  test_fenwick<long long>();

  cout << "Testing Fenwick<BigInt>" << endl;
  test_fenwick<BigInt>();

  cout << "Testing BigInt::sqrt" << endl;
  test_sqrt();
  return 0;
}
