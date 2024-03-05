#pragma once
#include <initializer_list>
#include <ranges>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <ostream>

template<typename T>
class Matrix {
protected:
    using size = size_t;

private:
    size N, M;
    T* data;
        
public:
    Matrix(): M(0), N(0), data(nullptr) {
        std::cout<<"emtpy constructor"<<std::endl;
    }

    Matrix(size N, size M): N(N), M(M), data(new T[M*N]) {
        std::cout<<"constructor of "<<N<<"x"<<M<<" matrix"<<std::endl;
        std::fill(data, data+M*N, 0.0);
    }

    Matrix(std::initializer_list<std::initializer_list<T>> init) {
        N = init.size();
        M = std::accumulate(init.begin(), init.end(), init.begin()->size(), [](auto a, auto l){
            return std::max(a, l.size());
        });
        std::cout<<"constructor of "<<N<<"x"<<M<<" matrix from initalizer_list"<<std::endl;
        data = new T[M*N];
        auto tmp = data;
        std::for_each(init.begin(), init.end(), [&tmp, this](const auto& row){
            std::copy(row.begin(), row.end(), tmp);
            std::fill(tmp+row.size(), tmp+M, 0);
            tmp += M;
        });
    }

    Matrix(const Matrix& m): N(m.N), M(m.M), data(new T[m.M*m.N]) {
        std::cout<<"copy constructor"<<std::endl;
        for(int i=0; i != m.N; i++) {
            std::copy(m.data+i*m.M, m.data+(i+1)*m.M, data+i*m.M);
        }
    }

    Matrix& operator=(const Matrix& m) {
        std::cout<<"copy assignment operator"<<std::endl;
        Matrix tmp(m);
        swap(tmp);
        return *this;
    }

    Matrix(Matrix&& m): N(m.N), M(m.M), data(m.data) {
        std::cout<<"move constructor"<<std::endl;
        m.M = 0;
        m.N = 0;
        m.data = nullptr;
    }

    Matrix& operator=(Matrix&& m) {
        std::cout<<"move assignment operator"<<std::endl;
        swap(m);
        return *this;
    }

    Matrix operator-() const {
        Matrix tmp(N, M);
        for(size i=0; i != N; i++) {
            for(size j=0; j != M; j++) {
                *(tmp.data+i*M+j) = -*(data+i*M+j);
            }
        }
        return tmp;
    }

    void swap(Matrix& tmp) throw() {
        std::swap(data, tmp.data);
        std::swap(M, tmp.M);
        std::swap(N, tmp.N);
    }

    T& operator()(size y, size x) {
        return data[(y-1)*M+(x-1)];
    }
    
    size m() {
        return M;
    }
    
    size n() {
        return N;
    }

    ~Matrix() {
        //std::cout<<"destructor"<<std::endl;
        delete[] data;
    }

    template<typename TT>
    friend std::ostream& operator<<(std::ostream& str, const Matrix<TT>& m);
};

template<typename T>
std::ostream& operator<<(std::ostream& str, const Matrix<T>& m) {
    for(int i=0; i != m.N; i++) {
        std::cout<<"{";
        std::for_each(m.data+i*m.M, m.data+(i+1)*m.M-1, [](const auto& n) {
            std::cout<<n<<", ";
        });
        std::cout<<*(m.data+(i+1)*m.M-1)<<"}"<<std::endl;
    }
    return str;
}
