#include <iostream>

template<unsigned long long A, unsigned long long B>
struct Power {
  const static unsigned long long value = A*Power<A, B-1>::value;
};

template<unsigned long long A>
struct Power<A, 0> {
  const static unsigned long long value = 1;
};

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

int main() {

  std::cout << Power<5,3>::value << std::endl;   // 125
  static_assert( Power<5,3>::value == 125);
  static_assert( Power<2,8>::value == 256);

  std::cout << Binomial<4,2>::value << std::endl; //6
  std::cout << Binomial<100,0>::value << std::endl; //1
  std::cout << Binomial<100,1>::value << std::endl; //100
  std::cout << Binomial<100,7>::value << std::endl; //16007560800

  static_assert( Binomial<4,2>::value == 6 );
  static_assert( Binomial<100,0>::value == 1 );
  static_assert( Binomial<100,1>::value == 100 );
  static_assert( Binomial<100,7>::value == 16007560800 );

  return 0;
}
