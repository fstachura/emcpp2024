#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cctype>
#include <ctime>
#include <thread>
#include <utility>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

template <int N>
struct Array {
    int a[N];
    int i = 0;
    int value = 1;
    long long sum = 0;
    std::mutex m;

    int f(int x) {
        int y = x%11;
        return (y*y+1);
    }

    void generateArray() {
        std::lock_guard<std::mutex> m_lock(m);
        while(i<N) {
            a[i++] = rand();
        }

        i = 0;
        while(i<N) {
            a[i] = f(a[i]);
            i++;
        }
    }

    long long computeSum() {
        std::lock_guard<std::mutex> m_lock(m);
        sum = 0;
        for(int x: a) {
            sum += x;
        }
        return sum;
    }
};

int main(){
    const int n = 8;
    srand(2019);
    using A = Array<1000>;
    A array;

    std::vector<std::thread> ts;
    for(int i=0; i != n; i++) {
        ts.push_back(std::move(std::thread(&A::generateArray, &array)));
    }
    for(auto&& t: ts) {
        t.join();
    }
    for(int i=0; i<40; i++) {
        cout << array.a[0+i] << "  ";
    }
    long long sum = array.computeSum();
    cout << "\n sum = " << sum << endl;
}
