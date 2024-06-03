#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
using namespace std;

template <typename Container>
double median(Container set, std::forward_iterator_tag) {
    set.sort();
    if(set.size() == 0) {
        return 0;
    } else if(set.size() % 2 == 1) {
        auto it = set.begin();
        for(int i=0; i != (set.size())/2; i++) {
            it++; 
        }
        return *it;
    } else {
        auto it = set.begin();
        for(int i=0; i != (set.size())/2; i++) {
            it++; 
        }
        auto a = *it;
        auto b = *(++it);
        return ((double)(a+b))/2;
    }
}

template <typename Container>
double median(Container set, std::random_access_iterator_tag) {
    std::sort(set.begin(), set.end());
    if(set.size() == 0) {
        return 0;
    } else if(set.size() % 2 == 1) {
        return *(set.begin()+(set.size()/2));
    } else {
        auto a = *(set.begin()+(set.size()/2)-1);
        auto b = *(set.begin()+(set.size()/2));
        return (double(a+b))/2;
    }
}

template <typename Container>
double median(Container set) {
    return median(set, typename std::iterator_traits<typename Container::iterator>::iterator_category());
}

int main() {
    std::list<int> a0{};
    cout << median(a0) << endl; // 0
    std::list<int> a1{1};
    cout << median(a1) << endl; // 1
    std::list<int> a{3, 2, 5, 1, 4};
    cout << median(a) << endl; // 3
    std::list<int> a2{3, 2, 5, 1, 4, 2};
    cout << median(a2) << endl; // 3.5
    std::vector<int> v{3, 1, 4, 2};
    cout << median(v) << endl; // 2.5
    std::vector<int> v2{3, 1, 4, 2, 6};
    cout << median(v2) << endl; // 3
    std::vector<int> v0{};
    cout << median(v0) << endl; // 0
    std::vector<int> v1{1};
    cout << median(v1) << endl; // 1
}
