#include <iostream>
using namespace std;

class BaseVectorOps {
};

template <int N>
class Vector;

template <typename A>
struct VectorMulByScalarOp;

template <typename T>
struct VectorSizeTrait {
  const static int size = T::size;
};

template <int N>
struct VectorSizeTrait<Vector<N>&> {
  const static int size = N;
};

template <typename A, typename B>
struct VectorOp: public BaseVectorOps {
  const A a;
  const B b;
  const static int size = VectorSizeTrait<A>::size;
  const static int a_size = VectorSizeTrait<A>::size;
  const static int b_size = VectorSizeTrait<A>::size;

  VectorAddOp(A a, B b): a(a), b(b) {
    static_assert(a_size == b_size);
  }

  int operator[](int index) const {
    return a[index] + b[index];
  }

  operator Vector<size>() const {
    Vector<size> result;
    for(int i=0; i != size; i++) {
      result[i] = this->operator[](i);
    }
    return result;
  }
};

template <typename A>
struct VectorMulByScalarOp: public BaseVectorOps {
  int s;
  const A a;
  const static int size = VectorSizeTrait<A>::size;

  VectorMulByScalarOp(int s, A a): s(s), a(a) {
  }

  int operator[](int index) const {
    return s*a[index];
  }

  operator Vector<size>() const {
    Vector<size> result;
    for(int i=0; i != size; i++) {
      result[i] = this->operator[](i);
    }
    return result;
  }
};

template <int N>
class Vector: public BaseVectorOps {
  int data[N];
 public:
  const static int size = N;

  Vector(){
	cout << " Default constr" << endl;
  }
  Vector(std::initializer_list<int> list){
	cout << " Init list constr" << endl;
	auto it = list.begin();
	for(int i=0; i<N; i++) {
	  data[i] = *it++;
	}
  }
  Vector(const Vector & m){
	std::copy(m.data, m.data+N, data);
	cout << " Copy constr" << endl;
  }

  int operator[](int index) const {
	return data[index];
  }

  int & operator[](int index){
	return data[index];
  }

  friend ostream & operator << (ostream & out, const Vector & m){
    for(auto x : m.data){
      cout << x << ", ";
    }
    return out;
  }
};

template<typename T>
concept DerivedBaseVectorOps = std::derived_from<T, BaseVectorOps> || std::derived_from<std::remove_reference_t<T>, BaseVectorOps>;

template<DerivedBaseVectorOps A>
auto operator*(int s, A&& a) {
  return VectorMulByScalarOp<A>(s, std::forward<A>(a));
}

template<DerivedBaseVectorOps A>
auto operator*(A&& a, int s) {
  return VectorMulByScalarOp<A>(s, std::forward<A>(a));
}

template<DerivedBaseVectorOps A, DerivedBaseVectorOps B>
auto operator+(A&& a, B&& b) {
  return VectorAddOp<A, B>(std::forward<A>(a), std::forward<B>(b));
}

template<DerivedBaseVectorOps A, DerivedBaseVectorOps B>
auto operator-(A&& a, B&& b) {
  return VectorAddOp<A, VectorMulByScalarOp<B>>(std::forward<A>(a), VectorMulByScalarOp<B>(-1, std::forward<B>(b)));
}

class Base {
};

template<typename T>
concept DerivedBaseAB = std::derived_from<T, Base> || std::derived_from<std::remove_reference_t<T>, Base>;

struct A: public Base {
  int a;
  A(int a): a(a) {}
};

struct B: public Base {
  int a;
  B(int a): a(a) {}
};

template<DerivedBaseAB D, DerivedBaseAB E>
auto operator+(D& a, E& b) {
  A c{a.a+b.a};
  return c;
}

int main(){
  using V = Vector<10>;
  V v{1,2,3,4,5,6,7,8,9,10};
  V x(v);
  V y{4,4,2,5,3,2,3,4,2,1};

  cout << "Lazy operations :\n";
  // It does not create temporary Vectors
  // It computes resulting vector coordinate by coordinate
  // (evaluating whole expression)
  V z = v + x + 3 * y - 2 * x;
  cout << z << endl;

  V z2 = 2 * (v + x + 3 * y - 2 * x);
  cout << z2 << endl;

  V z3 = (v + x + 3 * y - 2 * x) * 3;
  cout << z3 << endl;

  A a{1};
  B b{1};
  A c = a+b;
  std::cout<<c.a<<std::endl;

  // Computes only one coordinate of Vector
  int e = (z+x+y)[2];
  cout << " e = " << e << endl;
  return 0;
}
/**
 Init list constr
 Copy constr
 Init list constr
Lazy operations :
 Default constr
12, 12, 6, 15, 9, 6, 9, 12, 6, 3,
e = 11
 */
