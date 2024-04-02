#pragma once
#include "staticVector.h"
#include <stdexcept>

class VectorException: public std::runtime_error {
public:
    VectorException(): runtime_error("Sizes do not match.") {}
};

template <typename T>
class Vector<T, 0> {
    std::unique_ptr<T[]> data;
    size_t array_size;

public:
    typedef T value_type;
    typedef std::size_t  size_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T& const_reference;

    Vector(size_t size): array_size(size), data(new T[size]) {
    }

    Vector(const Vector& v) {
        data = std::make_unique<T[]>(v.array_size);
        array_size = v.array_size;
        for(int i=0; i != v.array_size; i++) {
            data[i] = v.data[i];
        }
    }

    Vector &operator=(const Vector& m) {
        data = m.data;
        array_size = m.array_size;
    }

    Vector(const std::initializer_list<T> &list) { 
        data = std::make_unique<T[]>(list.size());
        array_size = list.size();
        auto el = list.begin();
        for(int i=0; i != array_size; i++) {
            if(el == list.end()) return;
            data[i] = *el;
            el++;
        }
    }

    Vector(Vector&& v): data(std::move(v.data)) {
        v.data = nullptr;
        v.array_size = 0;
    }

    Vector operator=(Vector&& v) {
        data = v.data;
        v.data = nullptr;
        v.array_size = 0;
    }

    friend Vector operator+(const Vector& u, const Vector& v) {
        if(u.array_size != v.array_size) {
            throw VectorException();
        }
        Vector result(u.array_size);
        for(int i=0; i != u.array_size; i++) {
            result[i] = u[i] + v[i];
        }
        return result;
    }

    template<typename S, size_t M>
    friend Vector<S, M> operator+(const Vector<S, M>& u, const Vector<T, 0>& v) {
        if(v.array_size != M) {
            throw VectorException();
        }
        Vector<S, M> result = u;
        for(int i=0; i != M; i++) {
            result[i] += v[i];
        }
        return result;
    }

    template<typename S, size_t M>
    friend Vector<S, M> operator+(const Vector<T, 0>& v, const Vector<S, M>& u) {
        return u + v;
    }

    template<size_t N>
    explicit operator Vector<T, N>() {
        Vector<T, N> result;
        for(int i=0; i != N; i++) {
            result.data[i] = data[i];
        }
        return result;
    }

    template<typename S, size_t M>
    explicit operator Vector<S, M>() {
        Vector<S, M> result;
        for(int i=0; i < std::min(array_size, M); i++) {
            result[i] = static_cast<S>(data[i]);
        }
        for(int i=array_size; i < M; i++) {
            result[i] = {};
        }
    }

    template<typename S>
    explicit operator Vector<S, 0>() {
        Vector<S, 0> result(array_size);
        for(int i=0; i != array_size; i++) {
            result[i] = static_cast<S>(data[i]);
        }
    }

    void resize(size_t new_size) {
        T* tmp = new T[new_size];
        for(int i=0; i < std::min(array_size, new_size); i++) {
            tmp[i] = data[i];
        }
        for(int i=array_size; i < new_size; i++) {
            tmp[i] = {};
        }
        array_size = new_size;
    }

    constexpr size_type size() const {
        return array_size;
    }

    const_reference get(size_type index) const {
        return data.get()[index];
    }

    void set(size_type index, const_reference value) {
        data.get()[index] = value;
    }

    reference operator[](size_type index){
        return data[index];
    }

    const_reference operator[](size_type index) const{
        return data[index];
    }

    friend std::ostream &operator<<(std::ostream &out, const Vector & v) {
        for(int i=0; i != v.array_size; i++) {
            out << v[i] << " ";
        }
        return out;
    }
};
