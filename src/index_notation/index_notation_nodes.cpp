#include "taco/index_notation/index_notation_nodes.h"
#include "taco/index_notation/intrinsic.h"

#include <set>
#include <functional>

#include "taco/util/collections.h"

using namespace std;

namespace taco {

template <typename T>
static std::vector<Datatype> getDataTypes(const std::vector<T> args) {
  std::function<Datatype(T)> getType = [](T arg) { return arg.getDataType(); };
  return util::map(args, getType);
}

// class CallIntrinsicNode
CallIntrinsicNode::CallIntrinsicNode(const std::shared_ptr<Intrinsic>& func, 
                                     const std::vector<IndexExpr>& args, 
                                     const std::vector<Literal>& attrs) 
    : IndexExprNode(
        func->inferReturnType(getDataTypes(args), getDataTypes(attrs))
      ), func(func), args(args), attrs(attrs) {
}


// class ReductionNode
ReductionNode::ReductionNode(IndexExpr op, IndexVar var, IndexExpr a)
    : IndexExprNode(a.getDataType()), op(op), var(var), a(a) {
  taco_iassert(isa<BinaryExprNode>(op.ptr));
}

}
