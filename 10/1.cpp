#include <iostream>
#include <vector>
#include <type_traits>
using namespace std;

//first args get deduced (always to (T, void))
template <typename T, typename = void>
struct hasSize
    : std::false_type {};

//then, if decltype is valid, the pattern deduces to (T, void)
//so the args match
//if decltype is invaid (no size method), this errors and only
//the default template can match
template <typename T>
struct hasSize<T, std::void_t<decltype(declval<T>().size())>>
    : std::true_type {};

template <typename T, typename = void>
struct hasValueType
    : std::false_type {};

template <typename T>
struct hasValueType<T, std::void_t<typename T::value_type>>
    : std::true_type {};

namespace v1 {
    template <typename T>
    constexpr bool getSizeCond = (hasSize<T>::value && hasValueType<T>::value);

    template <typename T>
    size_t getSize(const T& x, std::enable_if_t<!getSizeCond<T>, int> = 0) {
        return sizeof(x);
    }

    template <typename T>
    size_t getSize(const T& x, std::enable_if_t<getSizeCond<T>, int> = 0) {
        return x.size()*sizeof(typename T::value_type);
    }
};

namespace v2 {
    template <typename T>
    size_t getSize(const T& x) {
        if constexpr (hasSize<T>::value && hasValueType<T>::value) {
            return x.size()*sizeof(typename T::value_type);
        } else {
            return sizeof(x);
        }
    }
};

struct test {
    int size() {
        return 1000;
    };
};

struct test2 {
    using value_type = long long;
};

int main() {
    cout << hasSize<int>::value << endl; // false
    cout << hasSize<vector<int>>::value << endl; //true
    cout << hasValueType<int>::value << endl; // false
    cout << hasValueType<vector<int>>::value << endl; //true
                                                      
    std::vector<int> v{1,2,3,4,5};
    cout << v1::getSize(5) << endl; // 4
    cout << v1::getSize(v) << endl; // 20
    cout << v1::getSize(test{}) << endl; // 20
    cout << v1::getSize(test2{}) << endl; // 20
    cout << v2::getSize(5) << endl; // 4
    cout << v2::getSize(v) << endl; // 20
    cout << v2::getSize(test{}) << endl; // 20
    cout << v2::getSize(test2{}) << endl; // 20
}
