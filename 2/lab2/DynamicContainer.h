#pragma once
#include <iostream>
#include <memory>
#include "Box.h"

using namespace std;

class Container{
    // Exercise 2: Use smart pointer.
    // there is no no-parameter constructor generated becuase there
    // are no constructors with parameters
    // hence, nullptr here should not be a problem
    std::unique_ptr<Box> pbox = nullptr;

public:
    static bool verbose;

    Container(int content): pbox(std::make_unique<Box>(content)) {
        if(verbose) 
            cout << "Creating Container" << endl;
    }

    Container(const Container& container) {
        if(this != &container) {
            pbox = std::make_unique<Box>(*container.pbox);
            if(verbose) 
                cout << "Creating copy of Container\n";
        }
    }

    Container(Container&& container): pbox(std::move(container.pbox)) {
        if(verbose) 
            cout << "Moving Container\n";
    }

    Container& operator=(const Container& container) {
        if(this != &container) {
            if(verbose) cout << "Copying Container\n";
            *pbox = *container.pbox;
        }

        return *this;
    }

    Container& operator=(Container&& container) {
        if(this != &container)
            pbox = std::move(container.pbox);

        return *this;
    }

    ~Container() {
        if(verbose) 
            cout << "Destroying Container \n";
    }

    friend Container operator+(const Container & p1, const Container & p2);
    friend std::ostream & operator << (std::ostream & out, const Container & p) {
        return (out << " [" << p.pbox->getContent() << "] ");
    }
};

bool Container::verbose = false;

inline Container operator+(const Container & p1, const Container & p2) {
    Container suma(p1.pbox->getContent() + p2.pbox->getContent());
    return suma;
}
