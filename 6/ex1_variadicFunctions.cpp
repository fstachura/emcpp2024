#include <bits/utility.h>
#include <iostream>
#include <vector>

using namespace std;

template<typename... Ts>
double average(Ts... t) {
  return (static_cast<double>(t) + ...)/sizeof...(Ts);
}

template<typename F, typename... Args>
auto computeSum(F f, Args... a) {
  return (f(a) + ...);
}

template<typename Cont, typename... Args>
void insert(Cont& cont, Args... a) {
  (cont.push_back(a), ...);
}

int f(int x) {
  return x*x;
}

int cube(int x) {
  return x*x*x;
}

int main() {
    cout << average(1, 2) << endl;                    // 1.5
    cout << average(1.0, 10, 1, 34.5f, 12u) << endl;  // 11.7
    cout << computeSum(f, 1, 2, 3, 4) << endl;        // f(1)+f(2)+f(3)+f(4) = 30
    cout << computeSum(cube, 1, 2, 3, 4) << endl;     // 1^3 + 2^3 + 3^3 + 4^3 = 100

    std::vector<int> v;
    insert(v, 1, 2, 3, 4, 5, 6);
    insert(v, 7, 8);

    for(auto x: v)                              //1 2 3 4 5 6 7 8
        cout << x << " ";
    return 0;
}
