#include <numeric>
#include <ostream>
#include <type_traits>
#include <iostream>
#include <iomanip>

template <int N, typename IL> 
struct getInt {
  static const int value = IL::get(N);
};

template <typename IL> 
struct Max {
  static const int value = IL::max();
};

template <typename IL> 
struct IsSorted {
  static const bool value = IL::isSorted();
};

template <int T, int... Ts>
struct IntegerList {
  static constexpr int arr[sizeof...(Ts)+1] = {T, Ts...};

  static constexpr int get(int n) {
    return arr[n];
  }

  static constexpr int max() {
    auto result = arr[0];
    for(int i=1; i <= sizeof...(Ts); i++) {
      if(arr[i] > result) {
	result = arr[i];
      }
    }
    return result;
  }

  static constexpr int isSorted() {
    for(int i=0; i != sizeof...(Ts); i++) {
      if(arr[i] > arr[i+1]) {
	return false;
      }
    }
    return true;
  }

  friend std::ostream& operator<<(std::ostream& o, IntegerList<T, Ts...> l) {
    for(auto&& n: l.arr) {
      o << n << " ";
    }
    return o;
  }
};

template<typename IL1, typename IL2>
struct Join {
};

template<int... As, int... Bs>
struct Join<IntegerList<As...>, IntegerList<Bs...>> {
  using type = IntegerList<As..., Bs...>;
};

using namespace std;
int main() {
  using listA = IntegerList<5, -1, 5, 2, 1>;
  using listB = IntegerList<1, 4, 6, 9>;
  cout << "List A : " << listA() << endl;
  cout << "List B : " << listB() << endl;

  cout << "A[1] = " << getInt<1, listA>::value << endl;
  cout << "B[3] = " << listB::get(3) << endl;
  static_assert(getInt<1, listA>::value == -1);
  static_assert(listB::get(3) == 9);
  static_assert(listB::get(1) == 4);

  cout << boolalpha;
  cout << "Is A sorted? " << IsSorted<listA>::value << endl;
  cout << "Is B sorted? " << IsSorted<listB>::value << endl;

  using listC = Join<listA, listB>::type;

  cout << "List C : " << listC() << endl;
  cout << Max<listC>::value << endl;
  cout << listC::max() << endl;
  static_assert(listC::max() == 9);
  static_assert(listA::max() == 5);
}
