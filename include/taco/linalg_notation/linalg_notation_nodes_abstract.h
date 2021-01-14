//
// Created by Olivia Hsu on 10/30/20.
//

#ifndef TACO_LINALG_NOTATION_NODES_ABSTRACT_H
#define TACO_LINALG_NOTATION_NODES_ABSTRACT_H

#include <vector>
#include <memory>

#include "taco/type.h"
#include "taco/util/uncopyable.h"
#include "taco/util/intrusive_ptr.h"
#include "taco/linalg_notation/linalg_notation_visitor.h"

namespace taco {

class TensorVar;
class LinalgExprVisitorStrict;
class Precompute;

/// A node of a scalar index expression tree.
struct LinalgExprNode : public util::Manageable<LinalgExprNode>,
                       private util::Uncopyable {
public:
  LinalgExprNode() = default;
  explicit LinalgExprNode(Datatype type);
  LinalgExprNode(Datatype type, int order);
  LinalgExprNode(Datatype type, int order, bool isColVec);

  virtual ~LinalgExprNode() = default;

  virtual void accept(LinalgExprVisitorStrict*) const = 0;

  /// Return the scalar data type of the index expression.
  Datatype getDataType() const;
  int getOrder() const;
  bool isColVector() const;
  void setColVector(bool val);

private:
  Datatype dataType;
  int order;
  bool isColVec;
};

struct LinalgStmtNode : public util::Manageable<LinalgStmtNode>,
                       private util::Uncopyable {
public:
  LinalgStmtNode() = default;
  LinalgStmtNode(Type type);
  virtual ~LinalgStmtNode() = default;
  virtual void accept(LinalgStmtVisitorStrict*) const = 0;

  Type getType() const;

private:
  Type type;
};

}

#endif //TACO_LINALG_NOTATION_NODES_ABSTRACT_H
