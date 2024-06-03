#include <cstddef>
#include <exception>
#include <iterator>
#pragma include once

#include <algorithm>
#include <iomanip>
#include <iostream>
using namespace std;

template <typename T, size_t N, size_t M> class Matrix {
  T data[N * M];

public:
  constexpr size_t numberOfRows() const { return N; }
  constexpr size_t numberOfColumns() const { return M; }

  Matrix(int nrows = N, int ncols = M) { std::fill_n(data, N * M, T{}); }

  Matrix(const std::initializer_list<std::initializer_list<T>> &list) {
    T *p = data;
    for (const auto &row : list) {
      T *p2 = std::copy(row.begin(), row.end(), p);
      std::fill(p2, p += M, T{});
    }
  }

  Matrix(const Matrix &m) { std::copy_n(m.data, N * M, data); }

  Matrix &operator=(const Matrix &m) {
    if (&m != this) {
      std::copy_n(m.data, N * M, data);
    }
    return *this;
  }

  T& operator()(int i, int j) { return data[(i - 0) * M + j]; }

  T get(int i, int j) const { return data[(i - 0) * M + j]; }

  friend Matrix operator+(const Matrix &a, const Matrix &b) {
    Matrix r;
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < M; ++j) {
        r(i, j) = a.get(i, j) + b.get(i, j);
      }
    }
    return r;
  }

  template<typename IteratorPolicy>
  class iterator_base {
    typename IteratorPolicy::matrix_type matrix;
    size_t row;
    size_t column;

  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename IteratorPolicy::value_type;
    using pointer_type = typename IteratorPolicy::pointer_type;
    using reference_type = typename IteratorPolicy::reference_type;
    using Self = iterator_base;

    iterator_base(Matrix<T, N, M>& matrix): matrix(matrix), row(0), column(0) {
    }

    iterator_base(Matrix<T, N, M>& matrix, size_t row, size_t column): matrix(matrix), row(row), column(column) {
    }

    reference_type operator*() {
      return IteratorPolicy::get(matrix, row, column);
    }

    pointer_type operator->() {
      return IteratorPolicy::get(matrix, row, column);
    }

    Self& operator++() {
      if(column < M-1) {
        column++;
      } else if(row < N) {
        column = 0;
        row++;
      }
      return *this;
    }

    Self& operator++(int) {
      auto tmp = *this;
      *this++;
      return tmp;
    }

    friend bool operator==(const Self& a, const Self& b) {
      auto result = &a.matrix == &b.matrix && (a.column == b.column && a.row == b.row);
      return &a.matrix == &b.matrix && (a.column == b.column && a.row == b.row);
    }

    friend bool operator!=(const Self& a, const Self& b) {
      return !(a == b);
    }
  };

  struct iterator_policy {
    using value_type = T;
    using pointer_type = T*;
    using reference_type = T&;
    using matrix_type = Matrix<T, N, M>&;

    static reference_type get(matrix_type m, size_t row, size_t col) {
      return m(row, col);
    }
  };

  struct const_iterator_policy {
    using value_type = T;
    using pointer_type = const T*;
    using reference_type = const T&;
    using matrix_type = const Matrix<T, N, M>&;

    static reference_type get(matrix_type m, size_t row, size_t col) {
      return m.get(row, col);
    }
  };

  using iterator = iterator_base<iterator_policy>;
  using const_iterator = iterator_base<const_iterator_policy>;

  iterator begin() {
    return iterator(*this);
  }

  iterator end() {
    return iterator(*this, N, 0);
  }

  class col_iterator {
    Matrix<T, N, M>& matrix;
    size_t row;
    size_t column;

  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer_type = T*;
    using reference_type = T&;
    using Self = col_iterator;

    col_iterator(Matrix<T, N, M>& matrix, size_t col): matrix(matrix), row(0), column(col) {
    }

    col_iterator(Matrix<T, N, M>& matrix, size_t col, size_t row): matrix(matrix), row(row), column(col) {
    }

    reference_type operator*() {
      return matrix(row, column);
    }

    pointer_type operator->() {
      return matrix(row, column);
    }

    Self& operator++() {
      if(row < N) {
        row++;
      }
      return *this;
    }

    Self& operator++(int) {
      auto tmp = *this;
      if(row < N) {
        row++;
      }
      return tmp;
    }

    friend bool operator==(const Self& a, const Self& b) {
      return a.column == b.column && a.row == b.row && &a.matrix == &b.matrix;
    }

    friend bool operator!=(const Self& a, const Self& b) {
      return !(a == b);
    }
  };

  col_iterator col_begin(size_t col) {
    return col_iterator(*this, col);
  }

  col_iterator col_end(size_t col) {
    return col_iterator(*this, col, N);
  }

  class row_iterator {
    Matrix<T, N, M>& matrix;
    size_t row;
    size_t column;

  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer_type = T*;
    using reference_type = T&;
    using Self = row_iterator;

    row_iterator(Matrix<T, N, M>& matrix, size_t row): matrix(matrix), row(row), column(0) {
    }

    row_iterator(Matrix<T, N, M>& matrix, size_t row, size_t column): matrix(matrix), row(row), column(column) {
    }

    reference_type operator*() {
      return matrix(row, column);
    }

    pointer_type operator->() {
      return &matrix(row, column);
    }

    Self& operator++() {
      if(column < M) {
        column++;
      }
      return *this;
    }

    Self& operator++(int) {
      auto tmp = *this;
      if(column < M) {
        column++;
      }
      return tmp;
    }

    friend bool operator==(const Self& a, const Self& b) {
      return a.column == b.column && a.row == b.row && &a.matrix == &b.matrix;
    }

    friend bool operator!=(const Self& a, const Self& b) {
      return !(a == b);
    }
  };

  row_iterator row_begin(size_t row) {
    return row_iterator(*this, row);
  }

  row_iterator row_end(size_t row) {
    return row_iterator(*this, row, M);
  }
};

template <typename T, size_t N, size_t M>
void printMatrix(const Matrix<T, N, M> &m, int width = 10) {
  for (int i = 0; i < m.numberOfRows(); i++) {
    for (int j = 0; j < m.numberOfColumns(); j++) {
      if (j != 0)
        cout << " ";
      cout << setw(width) << m.get(i, j);
    }
    cout << endl;
  }
}
