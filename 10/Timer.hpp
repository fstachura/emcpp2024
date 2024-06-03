#include <chrono>
#include <iostream>
using namespace std::chrono;

template<typename T>
class Timer {
    T data;
    std::string name;
    bool moved = false;
    decltype(steady_clock::now()) start;

    void swap(Timer& tmp) throw() {
        std::swap(data, tmp.data);
        std::swap(name, tmp.name);
        std::swap(start, tmp.start);
    }

public:
    Timer(std::string name, T data): data(data), name(name), start(steady_clock::now()) {}

    Timer(const Timer& c): data(c.data), name(c.name), start(steady_clock::now()) {
    }

    T& operator*() {
        return data;
    }

    T* operator->() {
        return &data;
    }

    Timer& operator=(const Timer& c) {
        if(this == c) return *this;
        Timer tmp(c);
        swap(tmp);
        return *this;
    }

    Timer(Timer&& c): data(c.data), name(c.name), start(c.start) {
        c.moved = true;
    }

    Timer& operator=(Timer&& c) {
        if(this == &c) return *this;
        swap(c);
        c.moved = true;
        return *this;
    }

    ~Timer() {
        if(!moved) {
            auto lived = duration_cast<seconds>(steady_clock::now()-start).count();
            std::cout<<"object destroyed: "<<name<<", lived "<<lived<<" seconds"<<std::endl;
        }
    }
};
