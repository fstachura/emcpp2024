#include <cstddef>
#include <numeric>
#include <optional>
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

template <int I, int... Is>
struct IntegerList {
  const static int cur = I;
  const static inline IntegerList<Is...> rest;
  const static bool last = false;

  friend std::ostream& operator<<(std::ostream& o, IntegerList<I, Is...> l) {
    o << l.cur << " ";
    return (o << l.rest);
  }

  template<typename T = IntegerList<I, Is...>>
  constexpr static int get(int n, T curList = {}) {
    if(curList.last || n == 0 || std::is_same_v<std::nullptr_t, decltype(curList.rest)>) {
      return curList.cur;
    } else {
      return get(n-1, curList.rest);
    }
  }

  template<int N>
  constexpr static int get(int n, IntegerList<N> curList) {
    return curList.cur;
  }

  template<typename T = IntegerList<I, Is...>>
  constexpr static int max(T curList = {}, int lastMax = cur) {
    if(curList.last) {
      return lastMax < curList.cur ? curList.cur : lastMax;
    } else {
      return max(curList.rest, lastMax < curList.cur ? curList.cur : lastMax);
    }
  }

  template<int N>
  constexpr static int max(IntegerList<N> curList, int lastMax) {
    return lastMax < curList.cur ? curList.cur : lastMax;
  }

  template<typename T = IntegerList<I, Is...>>
  constexpr static int isSorted(T curList = {}) {
    if(curList.last) {
      return true;
    } else if(curList.cur > curList.rest.cur) {
      return false; 
    } else {
      return isSorted(curList.rest);
    }
  }

  template<int N>
  constexpr static int isSorted(IntegerList<N> curList = {}) {
    return true;
  }
};

template<int I>
struct IntegerList<I> {
  const static int cur = I;
  const static bool last = true;
  const static std::nullptr_t rest;

  friend std::ostream& operator<<(std::ostream& o, IntegerList<I> l) {
    o << l.cur;
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
