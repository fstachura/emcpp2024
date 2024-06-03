#include <iostream>
#include <chrono>
#include <string>
using namespace std::chrono;

class Timer {
    std::string name;
    bool moved = false;
    decltype(steady_clock::now()) start;

    void swap(Timer& tmp) throw() {
        std::swap(name, tmp.name);
        std::swap(start, tmp.start);
    }

public:
    Timer(std::string name): name(name), start(steady_clock::now()) {}

    Timer(const Timer& c): name(c.name), start(steady_clock::now()) {
    }

    Timer& operator=(const Timer& c) {
        if(this == &c) return *this;
        Timer tmp(c);
        swap(tmp);
        return *this;
    }

    Timer(Timer&& c): name(c.name), start(c.start) {
        c.moved = true;
    }

    Timer& operator=(Timer&& c) {
        if(this == &c) return *this;
        swap(c);
        c.moved = true;
        return *this;
    }

    auto durationInNanoseconds() {
        return duration<long, std::nano>(steady_clock::now()-start).count();
    }

    ~Timer() {
        if(!moved) {
            auto lived = duration<float, std::milli>(steady_clock::now()-start);
            std::cout<<name<<" : "<<lived.count()<<" ms."<<std::endl;
        }
    }
};

long fibonacci(unsigned n){
    if (n < 2) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}

int main()
{
    Timer total("Total time");
    for(auto n : {1, 5, 15, 20, 25, 40, 45}){
        {
            Timer t("fibonacci(" + std::to_string(n) + ")");  // RAII
            fibonacci(n);

        }
    //2
    //3
    }
    std::cout << "Elapsed " << total.durationInNanoseconds() << " ns.\n";
}
/** Expected output similar to:

fibonacci(1) : 0.000619 ms.
fibonacci(5) : 0.000891 ms.
fibonacci(15) : 0.01025 ms.
fibonacci(20) : 0.094905 ms.
fibonacci(25) : 1.04443 ms.
fibonacci(40) : 989.97 ms.
fibonacci(45) : 8537.47 ms.
Elapsed 9528821533 ns.
Total time : 9528.84 ms.
*/
