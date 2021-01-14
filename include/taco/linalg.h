#ifndef TACO_LINALG_H
#define TACO_LINALG_H

#include "taco/type.h"
#include "taco/tensor.h"
#include "taco/format.h"

#include "taco/linalg_notation/linalg_notation.h"
#include "taco/linalg_notation/linalg_notation_nodes.h"
#include "taco/linalg_notation/linalg_notation_printer.h"


namespace taco {

class LinalgBase : public LinalgExpr {
protected:
  std::string name;
  Type tensorType;

  LinalgAssignment assignment;
  IndexStmt indexAssignment;

  int idxcount = 0;
  std::vector<std::string> indexVarNameList = {"i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

  IndexExpr rewrite(LinalgExpr linalg, std::vector<IndexVar> indices);

  IndexVar getUniqueIndex();

  std::vector<IndexVar> getUniqueIndices(size_t order);

public:
  LinalgBase(std::string name, Type tensorType, Datatype dtype, std::vector<int> dims, Format format, bool isColVec = false);

  /// [LINALG NOTATION]
  LinalgAssignment operator=(const LinalgExpr &expr);

  const LinalgAssignment getAssignment() const;

  const IndexStmt getIndexAssignment() const;

  IndexStmt rewrite();

  typedef LinalgVarNode Node;
};

std::ostream &operator<<(std::ostream &os, const LinalgBase &linalg);

IndexExpr rewrite(LinalgExpr linalg, std::vector<IndexVar>);

IndexStmt rewrite(LinalgStmt linalg);

// ------------------------------------------------------------
// Matrix class
// ------------------------------------------------------------

template<typename CType>
class Matrix : public LinalgBase {
public:
  explicit Matrix(std::string name);

  Matrix(std::string name, size_t dim1, size_t dim2);

  Matrix(std::string name, std::vector<size_t> dimensions);

  Matrix(std::string name, size_t dim1, size_t dim2, Format format);

  Matrix(std::string name, std::vector<size_t> dimensions, Format format);

  Matrix(std::string name, size_t dim1, size_t dim2, ModeFormat format1, ModeFormat format2);

  LinalgAssignment operator=(const LinalgExpr &expr) {
    return LinalgBase::operator=(expr);
  }

  // Read method
  CType at(int coord_x, int coord_y);

  // Write method
  void insert(int coord_x, int coord_y, CType value);

  // ScalarAccess supports reading/assigning to single element
  ScalarAccess<CType> operator()(int i, int j);

  // Access methods
  const Access operator()(const IndexVar i, const IndexVar j) const;
  Access operator()(const IndexVar i, const IndexVar j);

  // Allow to be cast to a TensorBase for the sake of ASSERT_TENSOR_EQ
  operator TensorBase() const { return *tensorBase; }
};

// ------------------------------------------------------------
// Matrix template method implementations
// ------------------------------------------------------------

template<typename CType>
Matrix<CType>::Matrix(std::string name) : 
  LinalgBase(name, Type(type<CType>(), {42, 42}), type<CType>(), {42, 42}, Format({dense, dense})) {}

template<typename CType>
Matrix<CType>::Matrix(std::string name, std::vector<size_t> dimensions) : 
  LinalgBase(name, Type(type<CType>(), Shape(std::vector<Dimension>(dimensions.begin(), dimensions.end()))), type<CType>(), std::vector<int>(dimensions.begin(), dimensions.end()), Format({dense,dense})) {}

template<typename CType>
Matrix<CType>::Matrix(std::string name, size_t dim1, size_t dim2) : 
  LinalgBase(name, Type(type<CType>(), {dim1, dim2}), type<CType>(), {(int) dim1, (int) dim2}, Format({dense,dense})) {}

template<typename CType>
Matrix<CType>::Matrix(std::string name, size_t dim1, size_t dim2, Format format) :
  LinalgBase(name, Type(type<CType>(), {dim1, dim2}), type<CType>(), {(int) dim1, (int) dim2}, format) {}

template<typename CType>
Matrix<CType>::Matrix(std::string name, std::vector<size_t> dimensions, Format format) :
  LinalgBase(name, Type(type<CType>(), Shape(std::vector<Dimension>(dimensions.begin(), dimensions.end()))), type<CType>(), std::vector<int>(dimensions.begin(), dimensions.end()), format) {}

template<typename CType>
Matrix<CType>::Matrix(std::string name, size_t dim1, size_t dim2, ModeFormat format1, ModeFormat format2) :
  LinalgBase(name, Type(type<CType>(), {dim1, dim2}), type<CType>(), {(int)dim1, (int)dim2}, Format({format1, format2}), false) {}

// Definition of Read methods
template <typename CType>
CType Matrix<CType>::at(int coord_x, int coord_y) {
  return tensorBase->at<CType>({coord_x, coord_y});
}

// Definition of Write methods
template <typename CType>
void Matrix<CType>::insert(int coord_x, int coord_y, CType value) {
  tensorBase->insert({coord_x, coord_y}, value);
}

template <typename CType>
ScalarAccess<CType> Matrix<CType>::operator()(int i, int j) {
  return ScalarAccess<CType>(tensorBase, {i, j});
}

// Definition of Access methods
template <typename CType>
const Access Matrix<CType>::operator()(const IndexVar i, const IndexVar j) const {
  return (*tensorBase)({i,j});
}

template <typename CType>
Access Matrix<CType>::operator()(const IndexVar i, const IndexVar j) {
  return (*tensorBase)({i,j});
}


// ------------------------------------------------------------
// Vector class
// ------------------------------------------------------------

template<typename CType>
class Vector : public LinalgBase {
  std::string name;
  Datatype ctype;
public:
  explicit Vector(std::string name);

  Vector(std::string name, int dim, bool isColVec = true);

  Vector(std::string name, size_t dim, Format format, bool isColVec = true);

  Vector(std::string name, size_t dim, ModeFormat format, bool isColVec = true);

  LinalgAssignment operator=(const LinalgExpr &expr) {
    return LinalgBase::operator=(expr);
  }

  // Support some Write methods
  void insert(int coord, CType value);

  ScalarAccess<CType> operator()(int i);

  // Support some Read methods too
  CType at(int coord);

  // Access methods for use in IndexExprs
  const Access operator()(const IndexVar i) const;
  Access operator()(const IndexVar i);

  // Allow to be cast to a TensorBase for the sake of ASSERT_TENSOR_EQ
  operator TensorBase() const { return *tensorBase; }
};

// ------------------------------------------------------------
// Vector template method implementations
// ------------------------------------------------------------

template<typename CType>
Vector<CType>::Vector(std::string name) : 
  LinalgBase(name, Type(type<CType>(), {42}), type<CType>(), {42}, Format({dense}), true) {}

template<typename CType>
Vector<CType>::Vector(std::string name, int dim, bool isColVec) : 
  LinalgBase(name, Type(type<CType>(), {(size_t)dim}), type<CType>(), {(int)dim}, Format({dense}), isColVec) {}

template<typename CType>
Vector<CType>::Vector(std::string name, size_t dim, Format format, bool isColVec) : LinalgBase(name,
                                                                                            Type(type<CType>(), {dim}),
                                                                                            type<CType>(),
                                                                                            {(int) dim},
                                                                                            format, isColVec) {}

template<typename CType>
Vector<CType>::Vector(std::string name, size_t dim, ModeFormat format, bool isColVec) :
  LinalgBase(name, Type(type<CType>(), {dim}), type<CType>(), {(int)dim}, Format(format), isColVec) {}

// Vector write methods
template<typename CType>
void Vector<CType>::insert(int coord, CType value) {
  tensorBase->insert({coord}, value);
}

template <typename CType>
ScalarAccess<CType> Vector<CType>::operator()(int i) {
  return ScalarAccess<CType>(tensorBase, {i});
}

template <typename CType>
CType Vector<CType>::at(int coord) {
  return tensorBase->at<CType>({coord});
}

// Definition of Access methods
template <typename CType>
const Access Vector<CType>::operator()(const IndexVar i) const {
  return (*tensorBase)({i});
}

template <typename CType>
Access Vector<CType>::operator()(const IndexVar i) {
  return (*tensorBase)({i});
}

// ------------------------------------------------------------
// Scalar class
// ------------------------------------------------------------

template<typename CType>
class Scalar : public LinalgBase {
  std::string name;
  Datatype ctype;
public:
  explicit Scalar(std::string name);

  LinalgAssignment operator=(const LinalgExpr &expr) {
    return LinalgBase::operator=(expr);
  }

  void operator=(const IndexExpr& expr) {
    (*tensorBase) = expr;
  }

  CType operator=(CType x) {
    tensorBase->insert({}, x);
    return x;
  }

  operator CType() const { return tensorBase->at<CType>({}); }

  operator TensorBase() const { return *tensorBase; }
};

template<typename CType>
Scalar<CType>::Scalar(std::string name) :
  LinalgBase(name, Type(type<CType>(), {}) , type<CType>(), {}, Format(), false) {}

}   // namespace taco
#endif
