#include <iostream>
using namespace std;

template <int N>
class Vector;

template <typename A>
struct VectorMulByScalarOp;

template<typename C, int N>
struct VectorAddOpBase {
  int operator[](int index) const {
    auto cthis = static_cast<const C*>(this);
    return cthis->a[index] + cthis->b[index];
  }

  operator Vector<N>() const {
    Vector<N> result;
    for(int i=0; i != N; i++) {
      result[i] = this->operator[](i);
    }
    return result;
  }
};

template <typename A, typename B>
struct VectorAddOp: public VectorAddOpBase<VectorAddOp<A, B>, A::size> {
  const A a;
  const B b;
  const static int size = A::size;

  VectorAddOp(const A& a, const B& b): a(a), b(b) {
    static_assert(A::size == B::size);
  }
};

template <typename T, int N>
struct VectorAddOp<Vector<N>, T>: public VectorAddOpBase<VectorAddOp<Vector<N>, T>, N>  {
  const Vector<N>& a;
  const T b;
  const static int size = N;

  VectorAddOp(const Vector<N>& a, const T& b): a(a), b(b) {
    static_assert(N == T::size);
  }
};

template <typename T, int N>
struct VectorAddOp<T, Vector<N>>: public VectorAddOpBase<VectorAddOp<T, Vector<N>>, N> {
  const T a;
  const Vector<N>& b;
  const static int size = N;

  VectorAddOp(const T& a, const Vector<N>& b): a(a), b(b) {
    static_assert(N == T::size);
  }
};

template <int N>
struct VectorAddOp<Vector<N>, Vector<N>>: public VectorAddOpBase<VectorAddOp<Vector<N>, Vector<N>>, N> {
  const Vector<N>& a;
  const Vector<N>& b;
  const static int size = N;

  VectorAddOp(const Vector<N>& a, const Vector<N>& b): a(a), b(b) {
  }
};

template<typename A, typename B>
auto operator*(int s, const VectorAddOp<A, B>& v) {
  return VectorMulByScalarOp<VectorAddOp<A, B>>(s, v);
}

template<typename T, typename A, typename B>
auto operator+(const VectorAddOp<A, B>& a, const T& n) {
  return VectorAddOp<VectorAddOp<A, B>, T>(a, n);
}

template<typename T, typename A, typename B>
auto operator-(const VectorAddOp<A, B>& a, const T& n) {
  return VectorAddOp<VectorAddOp<A, B>, VectorMulByScalarOp<T>>(a, VectorMulByScalarOp<T>(-1, n));
}

template <typename C, int N>
struct VectorMulByScalarOpBase {
  int operator[](int index) const {
    auto cthis = static_cast<const C*>(this);
    return (cthis->s)*(cthis->a[index]);
  }

  operator Vector<N>() const {
    Vector<C::A::size> result;
    for(int i=0; i != C::A::size; i++) {
      result[i] = this->operator[](i);
    }
    return result;
  }
};

template <typename A>
struct VectorMulByScalarOp: public VectorMulByScalarOpBase<VectorMulByScalarOp<A>, A::size> {
  int s;
  const A a;
  const static int size = A::size;

  VectorMulByScalarOp(int s, const A& a): s(s), a(a) {
  }
};

template <int N>
struct VectorMulByScalarOp<Vector<N>>: 
  public VectorMulByScalarOpBase<VectorMulByScalarOp<Vector<N>>, N> {

  int s;
  const Vector<N>& a;
  const static int size = N;

  VectorMulByScalarOp(int s, const Vector<N>& a): s(s), a(a) {
  }
};

template<typename A>
auto operator*(const VectorMulByScalarOp<A>& op, int s) {
  return VectorMulByScalarOp<VectorMulByScalarOp<A>>(s, op);
}

template<typename A, typename T>
auto operator+(const VectorMulByScalarOp<A>& op, const T& n) {
  return VectorAddOp<VectorMulByScalarOp<A>, T>(op, n);
}

template<typename A, typename T>
auto operator-(const VectorMulByScalarOp<A>& op, const T& n) {
  return VectorAddOp<VectorMulByScalarOp<A>, VectorMulByScalarOp<T>>(op, VectorMulByScalarOp<T>(-1, n));
}

template <int N>
class Vector{
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

  auto operator*(int s) {
    return VectorMulByScalarOp<Vector<N>>(s, *this);
  }

  friend auto operator*(int s, const Vector<N>& v) {
    return VectorMulByScalarOp<Vector<N>>(s, v);
  }

  template<typename T>
  auto operator+(const T& n) {
    return VectorAddOp<Vector<N>, T>(*this, n);
  }

  template<typename T>
  auto operator-(const T& n) {
    return VectorAddOp<Vector<N>, VectorMulByScalarOp<T>>(*this, VectorMulByScalarOp<T>(-1, n));
  }

  friend ostream & operator << (ostream & out, const Vector & m){
	for(auto x : m.data){
	  cout << x << ", ";
	}
	return out;
  }
};


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
