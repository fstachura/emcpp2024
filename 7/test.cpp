#include <iostream>

//template<typename T>
//concept ImplConcept = requires(T a) {
//    a.add(a);
//    a.sub(a);
//};

//template<ImplConcept T> 
template<typename T> 
struct test {
    T testA(T a, T b) {
        return a.add(b);
    }

    T testB(T a, T b) {
        return a.sub(b);
    }
};

struct impl {
    int a;

    impl add(impl b) {
        return impl{a+b.a};
    }

    //impl sub(impl b) {
    //    return impl{a-b.a};
    //}
};

int main() {
    impl a{1};
    impl b{1};

    test<impl> t;
    std::cout << t.testA(a, b).a << std::endl;
    //std::cout << t.testB(a, b).a << std::endl;
}
