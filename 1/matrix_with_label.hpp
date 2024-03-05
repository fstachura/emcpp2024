#pragma once
#include "matrix.hpp"
#include <initializer_list>

template<typename T>
class MatrixWithLabel: public Matrix<T> {
    std::string label = "A";

public:
    using Matrix<T>::Matrix;
    using size = typename Matrix<T>::size;

    MatrixWithLabel(std::string label, size N, size M): label(label), Matrix<T>::Matrix(N, M) {
    }

    MatrixWithLabel(std::string label, std::initializer_list<std::initializer_list<T>> init): 
        label(label), Matrix<T>::Matrix(init) {
    }

    MatrixWithLabel(const MatrixWithLabel& m): label(m.label), Matrix<T>::Matrix(m) {
    }

    MatrixWithLabel& operator=(const MatrixWithLabel&) = default;
    MatrixWithLabel& operator=(MatrixWithLabel&&) = default;
    MatrixWithLabel(MatrixWithLabel&&) = default;

    std::string getLabel() {
        return label;
    }

    void setLabel(std::string label) {
        this->label = label;
    }
};
