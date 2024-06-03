#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "Timer.hpp"

template<typename T>
void test(T t) {
    std::this_thread::sleep_for(1s);
}

int main() {
    Timer<std::vector<int>> t("test", {1,2,3});
    test(t);
    std::this_thread::sleep_for(1s);
    auto t2(std::move(t));
    auto t3 = t2;
    auto t4 = std::move(t2);
    for(auto&& v: *t4) {
        std::cout<<v<<" ";
    }
    std::cout<<std::endl;
    for(auto&& v: *t3) {
        std::cout<<v<<" ";
    }
    std::cout<<std::endl;
}
