#ifndef TACO_DENSE_MODE_TYPE_H
#define TACO_DENSE_MODE_TYPE_H

#include "taco/lower/mode.h"

namespace taco {

class DenseModeType : public ModeTypeImpl {
public:
  DenseModeType();
  DenseModeType(const bool isOrdered, const bool isUnique);

  virtual ~DenseModeType() {}

  virtual ModeType copy(
      const std::vector<ModeType::Property>& properties) const;

  virtual std::tuple<ir::Stmt,ir::Expr,ir::Expr> getCoordIter(
      const std::vector<ir::Expr>& i, Mode mode) const;
  virtual std::tuple<ir::Stmt,ir::Expr,ir::Expr> getCoordAccess(
      ir::Expr pPrev, const std::vector<ir::Expr>& i, Mode mode) const;
  
  virtual std::tuple<ir::Stmt,ir::Expr,ir::Expr> getLocate(
      ir::Expr pPrev, const std::vector<ir::Expr>& i, Mode mode) const;

  virtual ir::Stmt getInsertCoord(ir::Expr p,
      const std::vector<ir::Expr>& i, Mode mode) const;
  virtual ir::Expr getSize(Mode mode) const;
  virtual ir::Stmt getInsertInitCoords(ir::Expr pBegin,
      ir::Expr pEnd, Mode mode) const;
  virtual ir::Stmt getInsertInitLevel(ir::Expr szPrev,
      ir::Expr sz, Mode mode) const;
  virtual ir::Stmt getInsertFinalizeLevel(ir::Expr szPrev,
      ir::Expr sz, Mode mode) const;
  
  
  virtual ir::Expr getArray(size_t idx, const Mode mode) const;
  virtual std::vector<ir::Expr> getArrays(ir::Expr tensor, size_t level) const;

protected:
  ir::Expr getSizeArray(ModePack pack) const;
};

}

#endif
