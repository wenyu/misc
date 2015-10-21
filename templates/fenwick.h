#ifndef __FENWICK_H__
#define __FENWICK_H__

#include <algorithm>

// Zero-indexed Fenwick Tree
template <typename T = long long>
class Fenwick {
public:
  Fenwick(int n, T zero = T(0LL)) {
    N = n;
    S = new T[N];
    Z = zero;
    std::fill_n(S, N, Z);
  }


  ~Fenwick() {
    delete [] S;
  }

  void add(int index, T delta) {
    if (!index) {
      S[0] += delta;
      return;
    }
    while (index < N) {
      S[index] += delta;
      index += lowest_bit(index);
    }
  }

  T get(int left, int right) const {
    return sum(right) - sum(left - 1);
  }

private:
  int N;
  T* S;
  T Z;
  static inline int lowest_bit(register int n) { return n & (-n); }
  T sum(int p) const {
    if (p < 0) return Z;
    T result = S[p];
    int mask;
    while ( (mask = lowest_bit(p)) ) {
      result += S[p -= mask];
    }
    return result;
  }
};

#endif // __FENWICK_H__
