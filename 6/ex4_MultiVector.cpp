#include <iostream>
#include <vector>
using namespace std;

template<typename... T> class MultiVector;

template<>
class MultiVector<> {
public:
  void push_back() {
  }

  void print() {
    std::cout << std::endl;
  }
};

template<typename T, typename... Ts>
class MultiVector<T, Ts...>: public MultiVector<Ts...> {
  std::vector<T> data;

public:
  MultiVector(): data(std::vector<T>()) {
  }

  using MultiVector<Ts...>::push_back;

  void push_back(T&& d) {
    data.push_back(std::forward<T>(d));
  }

  void print() {
    std::cout << "[ ";
    for(auto&& n: data) std::cout << n << " ";
    std::cout << "] ";
    MultiVector<Ts...>::print();
  }
};

int main(){

  MultiVector<int, string, double> m;
  m.push_back(5);
  m.push_back(string("text"));
  m.push_back(7);
  m.push_back(1.2);
  m.print();   // [ 5 7 ] [ text ] [ 1.2 ]
  auto v = std::move(m);
  for(auto x: { 1, 2, 3}){
	v.push_back(x+10);
	v.push_back(std::to_string(x));
	v.push_back(x/10.0);
  }
  m.print(); // [ ] [ ] [ ]   but can be undefined
  v.print(); // [ 5 7 11 12 13 ] [ text 1 2 3 ] [ 1.2 0.1 0.2 0.3 ]
  MultiVector<int, int, double, int> w;
  w.push_back(1);
  w.push_back(2.0);
  w.print(); // [ 1 ] [ ] [ 2 ] [ ]
}
/*
[ 5 7 ] [ text ] [ 1.2 ]
[ ] [ ] [ ]
[ 5 7 11 12 13 ] [ text 1 2 3 ] [ 1.2 0.1 0.2 0.3 ]
[ 1 ] [ ] [ 2 ] [ ]  
 */
