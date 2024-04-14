#ifndef LAB8_VECTOR_H
#define LAB8_VECTOR_H

#include <memory>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>

template<typename T>
class VectorTraits {
public:
  using value_type = T;
  using reference_type = const T&;
  using scalar_type = T;

  static value_type multiply(scalar_type s, T val) {
    return s*val;
  }

  static value_type default_value() {
    return value_type{};
  }
};

template<>
class VectorTraits<int> {
public:
  using value_type = int;
  using reference_type = int;
  using scalar_type = int;

  static value_type multiply(scalar_type s, int val) {
    return s*val;
  }

  static value_type default_value() {
    return 0;
  }
};

template<>
class VectorTraits<double> {
public:
  using value_type = double;
  using reference_type = double;
  using scalar_type = double;

  static value_type multiply(scalar_type s, double val) {
    return s*val;
  }

  static value_type default_value() {
    return 0;
  }
};

template<>
class VectorTraits<std::string> {
public:
  using value_type = std::string;
  using reference_type = const std::string&;
  using scalar_type = int;

  static value_type multiply(scalar_type s, value_type val) {
    value_type result = default_value();
    for(scalar_type i = 0; i != s; i++) {
      result += val;
    }
    return result;
  }

  static value_type default_value() {
    return "";
  }
};

template<typename T>
class SafePolicy {
public: 
  void init(T* data, std::size_t N) {
    for(std::size_t i = 0; i != N; i++)  {
      *data = T{};
      data++;
    }
  }

  void check(std::size_t i, std::size_t N) const {
    if(i >= N) {
      throw std::out_of_range("vector access out of range");
    }
  }
};

template<typename T>
class FastPolicy {
public: 
  void init(T* data, std::size_t N) {
  }

  void check(std::size_t i, std::size_t N) const {
  }
};

template<typename T>
class InitFastPolicy: public SafePolicy<T> {
public: 
  void check(std::size_t i, std::size_t N) const {
  }
};

template <typename T, size_t N, template<typename> typename P = SafePolicy>
class Vector {
  T data[N];
  P<T> policy{};

 public:
  using value_type = typename VectorTraits<T>::value_type;
  using reference_type = typename VectorTraits<T>::reference_type;
  using scalar_type = typename VectorTraits<T>::scalar_type;

  typedef std::size_t  size_type;
  typedef T* pointer;
  typedef const T& const_reference;

  Vector() {
    policy.init(data, N);
  }

  Vector(const Vector & v) = default;
  Vector &operator=(const Vector & m) = default;

  Vector(const std::initializer_list<T> &list){
    if(list.size() != 0) {
      policy.check(list.size()-1, N);
    }
    std::copy(list.begin(), list.end(), data);
  }

  size_type size() const {
    return N;
  }

  reference_type get(size_type index) const {
    policy.check(index, N);
    return data[index];
  }

  void set(size_type index, reference_type value) {
    policy.check(index, N);
    data[index] = value;
  }

  friend Vector operator* (const scalar_type& x, const Vector & v ){
    Vector result;
    for(int i=0; i < v.size(); ++i){
      result.set(i, VectorTraits<T>::multiply(x, v.get(i)));
    }
    return result;
  }

  friend std::ostream &operator<<(std::ostream &out, const Vector & v) {
	for(int i=0; i < v.size(); ++i){
	  out << v.get(i) << " ";
	}
	return out;
  }

  friend std::istream &operator>>(std::istream &in, Vector & v) {
	Vector::value_type value;
	for(int i=0; i < v.size(); ++i){
	  in >> value;
	  if(in)
		v.set(i, value);
	}
	return in;
  }
};

#endif // LAB8_VECTOR_H
