#include <iostream>
#include <numeric>
#include <vector>

template<typename T>
concept HasPrint = requires(T a) {
  a.print();
};

template<typename T>
concept HasPrintOp = requires(T a, std::ostream os) {
  os << a;
};

template<typename T>
concept Printable = HasPrint<T> && HasPrintOp<T>;

template<typename T>
concept Container = (HasPrintOp<typename T::value_type> || HasPrint<typename T::value_type>) && requires(T a, T b) {
  typename T::value_type;
  a[0]+b[0];
  a.begin();
  a.end();
};

void print(const HasPrint auto& a) {
  a.print();
  std::cout << std::endl;
}

void print(const HasPrintOp auto& a) {
  std::cout << a << std::endl;
}

void print(const Printable auto& a) {
  std::cout << a << std::endl;
}

void print(const Container auto& a) {
  int i = 0;
  for(auto&& n: a) {
    std::cout<<i<<": ";
    print(n);
    i++;
  }
}

using namespace std;
template <typename  T>
class A{
protected:
    T x;
public:
    A(T x = T()) : x(x) {}
    A operator=(const A& b) {
      x = b.x;
      return *this;
    }
    void print() const {
      cout << "[" << x << "]";
    }
    friend A operator+(const A & a, const A & b){
      return a.x + b.x;
    }
};

template <typename T>
class B: public A<T>{
public:
    using A<T>::A;
    using A<T>::operator=;
    friend std::ostream & operator<<(std::ostream & out, const B & b){
      return (out << "#" << b.x << "#");
    }
};

template<Container T>
auto sum(T& a) {
  return std::accumulate(a.begin(), a.end(), typename T::value_type {});
}

using namespace std;
int main() {
  vector v{1,2,4,5};
  print(v);
  A<int> a{5};
  print(a);
  B<double> b{3.14};
  print(b);
  print(2.7);
  vector<A<int>> va{ 4, 5, 7, 9};
  vector<B<int>> vb{ 4, 5, 7, 9};
  print(va);
  print(vb);
  print( sum(v) );
  print( sum(vb) );
  return 0;
}
/**
* Expected output
0 : 1
1 : 2
2 : 4
3 : 5
-------
[5]
#3.14#
2.7
0 : [4]
1 : [5]
2 : [7]
3 : [9]
-------
0 : #4#
1 : #5#
2 : #7#
3 : #9#
-------
12
#25#
*/
