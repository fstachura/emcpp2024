#include <array>
#include <iostream>
#include <cmath>

template<unsigned long long N, unsigned long long K>
struct Binomial {
  const static unsigned long long value = Binomial<N-1, K>::value + Binomial<N-1, K-1>::value;
};

template<>
struct Binomial<0, 0> {
  const static unsigned long long value = 1;
};

template<unsigned long long N>
struct Binomial<N, 0> {
  const static unsigned long long value = 1;
};

template<unsigned long long K>
struct Binomial<0, K> {
  const static unsigned long long value = 0;
};

/** 
 * Exercise 2a: 
 * Implement function power that computes x^n 
 * during compilation using recurrent formula
   x^2n = (x^n)^2
   x^2n+1 = (x^2n)*x
  */
consteval auto power(auto x, int n) {
  if(n == 0) {
    return 1;
  } else if(n == 1) {
    return x;
  } else if(n % 2 == 0) {
    auto res = power(x, n/2);
    return res*res;
  } else {
    auto res = power(x, n-1);
    return res*x;
  }
}

constinit double fiveToPowerFour = power(5, 4);

/**
 *  Exercise 2b:
 * Implement function generate_triangle that during compilation 
 * generates the Pascal trriangle of the given size N.
 * Pascal triangle
 * https://en.wikipedia.org/wiki/Pascal%27s_triangle
 *  
 */
template <size_t N>
class PascalTriangle {
  std::array<int, N*(N+1)/2> data{};
public:
  constexpr PascalTriangle() {
    data[0] = 1;
    for(int i=1; i < N; i++) {
      int prev_beg = (i-1)*i/2;
      int beg = i*(i+1)/2;
      int end = (i+1)*(i+2)/2 - 1;
      data[beg] = 1;
      for(int j=1; j < i; j++) {
        data[beg+j] = data[prev_beg+j-1] + data[prev_beg+j];
      }
      data[end] = 1;
    }
  }

  constexpr int operator()(size_t n, size_t m) const {
    return data[n*(n+1)/2 + m];
  }
};

template <auto N>
constexpr PascalTriangle<N> generatePascalTriangle() {
  return PascalTriangle<N>();
}

constexpr size_t n = 10;
constexpr auto triangle = generatePascalTriangle<n>() ;

int main(){
  static_assert(triangle(0,0) == 1);
  static_assert(triangle(1,0) == 1);
  static_assert(triangle(1,1) == 1);
  static_assert(triangle(2,0) == 1);
  static_assert(triangle(2,1) == 2);
  static_assert(triangle(2,2) == 1);
  static_assert(triangle(3,0) == 1);
  static_assert(triangle(3,1) == 3);
  static_assert(triangle(3,2) == 3);
  static_assert(triangle(3,3) == 1);
  static_assert(triangle(5,3) == 10);
  static_assert(triangle(9,4) == 126);
}
