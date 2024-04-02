#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <cassert>
#include <numeric>
#include <cmath>
#include <type_traits>
using namespace std;

void print(const std::vector<int> & v, const std::string & label = "") {
  cout << label;
  for(auto x  : v)
    cout << " " << x;
  cout << endl;
}

int main() {
  std::vector<int> v = {256, 64, 16*3, 16*9, 16, 8*7, -9, -3, 1, 6, -2, 5, 9, 0};
  print(v, "original:");

  /// 1. Sort v with respect to the distance to the center (closest first)
  int center = 50;
  std::sort(v.begin(), v.end(), [center](auto a, auto b){
    return std::abs(center - a) < std::abs(center - b); 
  });            // 1
  print(v, "center 50:");

  /// 2. Sort in increasing order but
  ///  - negative numbers  are after positive ones, with zero in the middle,
  ///  - if numbers have the same sign then odd numbers are after even ones
  /// e.g.  4  < 8 < 1 < 3 < 0 <  -8 < -2 < -7 < -3

  auto sgn = [](auto val) {
    return (decltype(val)(0) < val) - (val < decltype(val)(0));
  };

  // true if first argument less than
  auto positiveEvenFirst = [sgn](auto a, auto b){
    if(sgn(a) != sgn(b)) {
      return sgn(b) < sgn(a);
    } else if(a%2 != b%2) {
      return std::abs(b)%2 > std::abs(a)%2;
    } else {
      return std::abs(b) > std::abs(a);
    }
  };                  // 2
  std::sort(v.begin(), v.end(), positiveEvenFirst);
  print(v, "positiveEven:");

  assert(positiveEvenFirst(2,-2));
  assert(positiveEvenFirst(2,-1));
  assert(positiveEvenFirst(1,-2));
  assert(!positiveEvenFirst(-2,1));
  assert(positiveEvenFirst(2, 1));
  assert(positiveEvenFirst(-2,-1));
  assert(!positiveEvenFirst(2,2));
  assert(!positiveEvenFirst(1,2));
  assert(positiveEvenFirst(0, -1));
  assert(!positiveEvenFirst(0, 1));
  assert(positiveEvenFirst(1, 0));
  assert(!positiveEvenFirst(-1, 0));


  /// 3. generator = function without parameters that return random integer number
  /// from interval [a, b] (a and b both included).
  /// Changing values of a or b should change the interval used by generator.
  srand(2022);
  int a = 0, b = 40;
  auto generator = [a, b]{
    return a + (std::rand() % b);
  };  /// 3

  std::generate(v.begin(), v.end(), generator);
  print(v, "generator [0,40]:");
  a = 100, b = 200;
  std::generate(v.begin(), v.end(), generator);
  print(v, "generator [100, 200]:");


  /// 4. Function that generates arithmetic sequence with given start value and step.
  /// Changing start does not change the beginning of the sequence
  /// but a change of step influences the function output.
  int start = 5, step = 2;
  auto arithmeticGenerator = [start, &step]() mutable {
    auto tmp = start;
    start += step;
    return tmp;
  };                   // [ 4 ]
  std::generate(v.begin(), v.end(), arithmeticGenerator);
  print(v, "arithm [5,2] :");

  v.resize(20);
  start=0; step =5;
  std::generate(v.begin(), v.end(), arithmeticGenerator);
  print(v,"arithm [5,5] :");

  /// 5. Function that for given standard container (vector, list, deque)
  ///  computes l1 norm i.e. the sum of the absolute values of elements in the container.
  ///  Try to use std::accumulate algorithm *with another lambda expression to implement it.
  auto l1_norm = [](auto c){
    return std::accumulate(c.begin(), c.end(), typename decltype(c)::value_type(), 
	[](auto sum, auto el){ return sum+std::abs(el); });
  };                           // [ 5 ]

  cout << "l1 norm (v) : " << l1_norm(v) << endl;
  assert(l1_norm(v) == 1050);

  vector<double> m = { 1.4, 2.4, -2.4, -4.2, -43.3 };
  cout << "l1 norm (m) : " << l1_norm(m) << endl;

  list<double> l(m.begin(), m.end());
  cout << "l1 norm (l) : " << l1_norm(l) << endl;

  /// 6. Function that for given array a and integer n returns
  /// a function with one parameter x that computes
  /// a value of a polynomial of degree n with coefficients a at the point x.
  auto polynomial = [](double* a, int n) {
    return [a, n](auto x) {
      return std::accumulate(a, a+n+1, 0.0, [n, x, c=0](auto sum, auto l) mutable {
	return sum+l*std::pow(x, c++);
      });
    };
  };   // ( 6 )

  double c[] = {1, 2, 3, 4, 5};
  auto w1  = polynomial(c, 4);
  cout << w1(1.0) << " " << w1(0.0) << " " << w1(2.0) << endl;

  auto w2  = polynomial(c, 2);
  cout << w2(1.0) << " " << w2(0.0) << " " << w2(2.0) << endl;

  return 0;
}

/** Expected ouput
original: 256 64 48 144 16 56 -9 -3 1 6 -2 5 9 0
center 50: 48 56 64 16 9 6 5 1 0 -2 -3 -9 144 256
negativeOdd: 6 16 48 56 64 144 256 1 5 9 0 -2 -9 -3
generator [0,40]: 2 10 15 19 37 11 27 30 37 1 0 23 38 32
generator [100, 200]: 120 115 173 107 127 186 178 171 142 176 186 124 200 132
arithm [5,2] : 5 7 9 11 13 15 17 19 21 23 25 27 29 31
arithm [5,5] : 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100
l1 norm (v) : 1050
l1 norm (m) : 53.7
l1 norm (l) : 53.7
15 1 129
6 1 17
 */
