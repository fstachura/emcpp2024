#include "matrix.hpp"
#include "matrix_with_label.hpp"

int main() {
    using Matrix = Matrix<int>;
    using MatrixWithLabel = MatrixWithLabel<int>;

    Matrix m1; // default constructor: N=M=0, data=nullptr
    Matrix m2(3,4); // N = 3, M = 4, data = N*M doubles set to 0.0
    Matrix m3({{1,2,3},{32, 23, 22},{3,234,23,44}}); // from initializer_list

    std::cout << m2(1,1) << std::endl; // 0 - the first element in data (we count from 1)
    std::cout << m3(2,2) << std::endl; // 23
    std::cout << m3; // prints matrix
    std::cout << "Copy semantics \n";
    Matrix m4 = m2; // copy constructor that makes deep copy of the matrix m2
    m4 = m3; // copy assignment makes deep copy of the matrix m3

    std::cout << std::endl;

    std::cout << "Move semantics \n";
    Matrix m7 = std::move(m2);
    m4 = -m3;
    std::cout << "Copy elision \n";
    Matrix m6 = -m4;
    Matrix * pm = new Matrix(-m4);
    std::cout << m6(2,1) << std::endl; // 32

    std::cout << std::endl;

    std::cout << "Inheritance \n";
    MatrixWithLabel l0("B", 3, 4);
    MatrixWithLabel l1({{1,2},{4,5}});
    l1.setLabel("A");
    MatrixWithLabel l2 = l1;
    MatrixWithLabel l3 = std::move(l1);
    std::cout << l2.getLabel() << " " << l3.getLabel() << std::endl;
    // cout << l1.getLabel() << endl;

    std::cout << std::endl;

    MatrixWithLabel a({{1,2,3},{3,4,5},{0,0,0}});
    MatrixWithLabel b(a);
    b(1,1) = 5;
    b.setLabel("B");
    std::cout<<a.getLabel()<<std::endl;
    std::cout<<a<<std::endl;
    std::cout<<b.getLabel()<<std::endl;
    std::cout<<b<<std::endl;

    std::cout<<"move"<<std::endl;
    MatrixWithLabel c(std::move(b));
    std::cout<<"end move"<<std::endl;
    c(3,3) = 5;
    std::cout<<c.getLabel()<<std::endl;
    std::cout<<c<<std::endl;
    c.setLabel("C");
    a.setLabel("AA");
    std::cout<<a.getLabel()<<std::endl;
    std::cout<<c.getLabel()<<std::endl;

    // 1. they seem to work correctly
    // 2. move now copies
    // 3. move moves again
}
