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

int main() {
  cout << "Testing Fenwick<long long>" << endl;
  test_fenwick<long long>();

  cout << "Testing Fenwick<BigInt>" << endl;
  test_fenwick<BigInt>();

  return 0;
}
