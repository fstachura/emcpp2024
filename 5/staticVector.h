#ifndef STATIC_VECTOR_H
#define STATIC_VECTOR_H

#include <memory>
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

template <typename T, size_t N_>
class Vector {
    static constexpr size_t N = N_;
    T data[N];

public:
    typedef T value_type;
    typedef std::size_t  size_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T& const_reference;

    Vector() {
        for(int i=0; i != N; i++) data[i] = {};
    }

    Vector(const Vector & v) = default;
    Vector &operator=(const Vector & m) = default;

    Vector(const std::initializer_list<T> &list) { 
        auto el = list.begin();
        for(int i=0; i != N; i++) {
            if(el == list.end()) return;
            data[i] = *el;
            el++;
        }
    }

    Vector(Vector&& v) {
        for(int i=0; i != N; i++) {
            data[i] = std::move(v[i]);
        }
    }

    Vector operator=(Vector&& v) {
        for(int i=0; i != N; i++) {
            data[i] = std::move(v[i]);
        }
    }

    friend Vector<T, 0>::operator Vector<T, N>();

    explicit operator Vector<T, 0>() {
        Vector<T, 0> result(N);
        for(int i=0; i != N; i++) {
            result[i] = data[i];
        }
        return result;
    }

    template<typename S, size_t M>
    explicit operator Vector<S, M>() {
        Vector<S, M> result;
        for(int i=0; i < std::min(N, M); i++) {
            result[i] = static_cast<S>(data[i]);
        }
        for(int i=N; i < M; i++) {
            result[i] = {};
        }
    }

    template<typename S>
    explicit operator Vector<S, 0>() {
        Vector<S, 0> result(N);
        for(int i=0; i != N; i++) {
            result[i] = static_cast<S>(data[i]);
        }
    }

    friend Vector operator+(const Vector& u, const Vector& v) {
        //static_assert(u.N == v.N, "vectors have incompatible sizes");
        //static_assert(std::is_same<decltype(u), decltype(v)>::value, "vectors have incompatible types");
        Vector result;
        for(int i=0; i != u.N; i++) {
            result[i] = u[i] + v[i];
        }
        return result;
    }

    constexpr size_type size() const {
        return N;
    }

    const_reference get(size_type index) const {
        return data[index];
    }

    void set(size_type index, const_reference value) {
        data[index] = value;
    }

    reference operator[](size_type index){
        return data[index];
    }
    const_reference operator[](size_type index) const{
        return data[index];
    }

    friend std::ostream &operator<<(std::ostream &out, const Vector & v) {
        for( auto elem: v.data ){
            out << elem << " ";
        }
        return out;
    }

};

#endif //STATIC_VECTOR_H
