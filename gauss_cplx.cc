#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <vector>
#include <set>
#include <utility>
#include <string>
#include <queue>
#include <map>
#include <bitset>
#include <climits>

#ifdef _DEBUG_MODE_
#define db(X) { cerr << "* DEBUG [L" << __LINE__ << "]: " << #X << " = " << X << endl; }
#define db_arr(X) { cerr << "* DEBUG [L" << __LINE__ << "]: {" << #X << "} = "; for (int __i__=0; __i__<sizeof(X)/sizeof(X[0]); __i__++) cerr << X[__i__] << " "; cerr << endl; }
#define db_arrM(X, M) { cerr << "* DEBUG [L" << __LINE__ << "]: {" << #X << "} = "; for (int __i__=0; __i__<M; __i__++) cerr << X[__i__] << " "; cerr << endl; }
#define db_arrMN(X, M, N) { cerr << "* DEBUG [L" << __LINE__ << "]: {" << #X << "} = "; for (int __i__=M; __i__<N; __i__++) cerr << X[__i__] << " "; cerr << endl; }
#else
#define db(X)
#define db_arr(X)
#define db_arrM(X, M)
#define db_arrMN(X, M, N)
#endif

#define For(i, n) for(i=0;i<(n);i++)
#define ForL(i, m, n) for(i=(m);i<(n);i++)

#define Clear(X) memset( (X), 0, sizeof( (X) ) )
#define Fill(X) memset( (X), -1, sizeof( (X) ) )

#define ArraySize(X) (sizeof(X)/sizeof(X[0]))

template <typename T> void xchg(T &a, T &b) { T c=a; a=b; b=c; }

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long double ld;

void _main();

int main() {
  // COUNTER CODE STARTS HERE

  _main();

  // COUNTER CODE ENDS HERE

  return 0;
}

// ACTUAL CODE STARTS BELOW

class Frac {
public:
  ll num, den;
  Frac(const int a = 0, const int b = 1) { num = a; den = b; simp(); }

  inline const Frac & operator+=(const Frac &rhs) {
    ll m = lcm(den, rhs.den);
    num = m/den*num + m/rhs.den*rhs.num;
    den = m;
    simp();
    return *this;
  }

  inline const Frac operator+(const Frac &rhs) const {
    Frac result(*this);
    return result += rhs;;
  }

  inline const Frac & operator-=(const Frac &rhs) {
    ll m = lcm(den, rhs.den);
    num = m/den*num - m/rhs.den*rhs.num;
    den = m;
    simp();
    return *this;
  }

  inline const Frac operator-(const Frac &rhs) const {
    Frac result(*this);
    return result -= rhs;
  }

  inline const Frac & operator*=(const Frac &rhs) {
    num *= rhs.num;
    den *= rhs.den;
    simp();
    return *this;
  }

  inline const Frac operator*(const Frac &rhs) const {
    Frac result(*this);
    return result *= rhs;
  }

  inline const Frac & operator/=(const Frac &rhs) {
    assert(rhs.num);
    num *= rhs.den;
    den *= rhs.num;
    simp();
    return *this;
  }

  inline const Frac operator/(const Frac &rhs) const {
    Frac result(*this);
    return result /= rhs;
  }

  inline const bool operator<(const Frac &rhs) const {
    return num * rhs.den < den * rhs.num;
  }

  inline const bool operator>(const Frac &rhs) const {
    return num * rhs.den > den * rhs.num;
  }

  inline const bool operator==(const Frac &rhs) const {
    return llabs(num * rhs.den - den * rhs.num) < 1e-6;
  }

  inline const bool operator!=(const Frac &rhs) const {
    return llabs(num * rhs.den - den * rhs.num) >= 1e-6;
  }

  inline const bool operator<=(const Frac &rhs) const {
    return num * rhs.den <= den * rhs.num;
  }

  inline const bool operator>=(const Frac &rhs) const {
    return num * rhs.den >= den * rhs.num;
  }

  inline const Frac & operator=(const ll &rhs) {
    num = roundl(rhs); den = 1;
    return *this;
  }

  inline const Frac operator-() const {
    Frac result(*this);
    result.num = -result.num;
    return result;
  }

  inline const Frac abs() const {
    Frac result(*this);
    if (result.num < 0) result.num = -result.num;
    return result;
  }

  inline ll gcd(ll a, ll b) {
    a = llabs(a); b = llabs(b);
    ll c;
    while ( b >= 1e-6 ) {
      c = a%b; a = b; b = c;
    }
    return a;
  }

  inline ll lcm(const ll &a, const ll &b) {
    ll d = gcd(a, b);
    return llabs(a)/d*llabs(b);
  }

  inline void simp() {
    if (den < 0) {
      den = -den; num = -num;
    }

    ll d = gcd(num, den);
    num = num / d;
    den = den / d;
  }

  const string str() const {
    char * ret = c_str();
    string result(ret);
    free(ret);
    return result;
  }

  char * c_str() const {
    char buf[50];
    if (den != 1) {
      sprintf(buf, "%Ld/%Ld", num, den);
    } else {
      sprintf(buf, "%Ld", num);
    }
    return strdup(buf);
  }

  inline const long double val() const {
    return (long double)num / den;
  }
};

class Cplx {
public:
  Frac real, imag;
  Cplx(const Frac &r=0, const Frac &i=0) { real = r; imag = i; }
  Cplx(const Cplx &v) { real = v.real; imag = v.imag; }
  inline const Cplx conj() const { return Cplx(real, -imag); }
  inline const Frac normSqr() const { return real * real + imag * imag; }
  inline const long double norm() const { long double a = real.val(), b = imag.val(); return a*a+b*b; }

  inline const Cplx & operator+=(const Cplx & rhs) {
    real += rhs.real; imag += rhs.imag;
    return *this;
  }

  inline const Cplx & operator-=(const Cplx & rhs) {
    real -= rhs.real; imag -= rhs.imag;
    return *this;
  }

  inline const Cplx & operator*=(const Cplx & rhs) {
    *this = Cplx(real * rhs.real - imag * rhs.imag, real * rhs.imag + imag * rhs.real);
    return *this;
  }

  inline const Cplx & operator/=(const Cplx & rhs) {
    Cplx c = rhs.conj(); Frac t = rhs.normSqr();
    *this *= c;
    real /= t;
    imag /= t;
    return *this;
  }

  inline const Cplx operator+(const Cplx &rhs) const { Cplx r(*this); r += rhs; return r; }
  inline const Cplx operator-(const Cplx &rhs) const { Cplx r(*this); r -= rhs; return r; }
  inline const Cplx operator*(const Cplx &rhs) const { Cplx r(*this); r *= rhs; return r; }
  inline const Cplx operator/(const Cplx &rhs) const { Cplx r(*this); r /= rhs; return r; }

  inline const bool operator==(const Cplx &rhs) const { return real == rhs.real && imag == rhs.imag; }
  inline const bool operator==(const Frac &rhs) const { return real == rhs; }

  const string str() const {
    if (real == 0 && imag == 0) return "0";
    string result = "";

    if (real != 0) {
      result += real.str();
    }

    if (imag != 0) {
      if (imag > 0 && result.size()) result += '+';
      if (imag == -1)
	result += '-';
      else if (imag != 1) {
	if (imag.den == 1) {
	  result += imag.str();
	} else {
	  if (imag.num > 0) {
	    result = result + "(" + imag.str() + ")";
	  } else {
	    result = result + "-(" + imag.abs().str() + ")";
	  }
	}
      }
      result += 'i';
    }

    return result;
  }

  char * c_str() const {
    return strdup(str().c_str());
  }
};

int cols, rows;
Cplx **data;

void printData() {
  int i, j;
  char *c, fmt[16];

  vector<int> width;
  width.clear(); width.resize(cols);

  For (i, cols) {
    For (j, rows) {
      width[i] = max(width[i], (int)data[j][i].str().size());
    }
  }

  For (i, rows) {
    For (j, cols) {
      if (j) printf("  ");
      sprintf(fmt, "%%%ds", width[j]);
      c = data[i][j].c_str();
      printf(fmt, c);
      free(c);
    }
    putchar('\n');
  }

  width.clear();
  putchar('\n');
}

void rowSwap(int ra, int rb) {
  if (ra == rb) return;
  register int i;
  printf(">>>>>>>>   r%d <--> r%d   <<<<<<<<\n", ra+1, rb+1);
  For (i, cols) xchg(data[ra][i], data[rb][i]);
  printData();
  return;
}

void rowDivide(int r, const Cplx scale) {
  if (scale == Cplx(1)) return;
  if (scale == Cplx(-1)) {
    printf("////////   -r%d   ////////\n", r+1);
  } else {
    printf("////////   r%d/(%s)   ////////\n", r+1, scale.c_str());
  }

  register int i;
  For (i, cols) data[r][i] /= scale;
  printData();
  return;
}

void rowSubtract(int ra, int rb, const Cplx scale) {
  if (scale == Cplx(0,0) ) return;
  if (scale == 1) {
    printf("********   r%d - r%d   ********\n", ra+1, rb+1);
  } else if (scale == -1) {
    printf("********   r%d + r%d   ********\n", ra+1, rb+1);
  } else {
    printf("********   r%d - (%s) x r%d   ********\n", ra+1, scale.c_str(), rb+1);
  }

  register int i;
  For (i, cols) data[ra][i] -= scale*data[rb][i];
  //printData();

  return;
}

void rrefStep() {
  int r, c, pr, pc, i, j;

  printf("******   ORIGINAL CONFIGURATION   ******\n");
  printData();

  pc = 0;

  For (pr, rows) {
    Cplx max(0); r = -1;
    long double maxNorm = 0;

    ForL (i, pr, rows) {
      if (data[i][pc].normSqr() == 0) continue;
      if (r == -1 || maxNorm > data[i][pc].norm()) {
	max = data[i][pc];
	maxNorm = max.norm();
	r = i;
      }
    }

    if (r == -1) {
      --pr; ++pc;
      if (pc == cols) break;
      continue;
    }

    rowSwap(pr, r);
    rowDivide(pr, max);

    For (r, rows) {
      if (r != pr) {
	rowSubtract(r, pr, data[r][pc]);
      }
    }
    printData();

    ++pc;
    if (pc == cols) break;
  }
}

void _main() {
  char buf[64], tok[64];
  cin >> rows >> cols;

  int i, j, r, s, v;
  ll a, b;

  data = new Cplx*[rows];
  For (i, rows) data[i] = new Cplx[cols];

  For (i, rows) {
    For (j, cols) {
      data[i][j] = Cplx(0, 0);

      cin >> buf;
      a = 0; b = 0;
      s = 0;

      while (buf[s]) {
	ForL (r, s+1, strlen(buf)) {
	  if (buf[r] == '+' || buf[r] == '-' || buf[r] == 'i' || !buf[r]) {
	    break;
	  }
	}

	if (buf[r] == 'i') ++r;

	Clear(tok);
	strncpy(tok, buf+s, (r-s));
	v = sscanf(tok, "%Ld/%Ld", &a, &b);

	if (tok[strlen(tok)-1] == 'i' && v == 0) {
	  a = b = 1;
	  if (tok[0] == '-') a = -1;
	} else {
	  switch (v) {
	  case 2: break;
	  case 1: b = 1; break;
	  default: printf("Invalid input data, buf=\"%s\" tok=\"%s\".\n", buf, tok); exit(1);
	  }
	}

	if (tok[r-s-1] == 'i') {
	  data[i][j].imag = Frac(a, b);
	} else {
	  data[i][j].real = Frac(a, b);
	}

	s = r;
      }
    }
  }

  rrefStep();

  For (i, rows) delete [] data[i];
  delete [] data;
}
