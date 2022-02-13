#include <taco/index_notation/transformations.h>
#include <codegen/codegen_c.h>
#include <codegen/codegen_cuda.h>
#include "test.h"
#include "test_tensors.h"
#include "taco/tensor.h"
#include "taco/index_notation/index_notation.h"
#include "codegen/codegen.h"
#include "taco/lower/lower.h"

using namespace taco;



TEST(bound, bound_and_rebound) {
  
  Tensor<double> A("A", {16}, Format{Dense});
  Tensor<double> B("B", {16}, Format{Dense});
  Tensor<double> C("C", {16}, Format{Dense});

  for (int i = 0; i < 16; i++) {
      A.insert({i}, (double) i);
      B.insert({i}, (double) i);
  }

  A.pack();
  B.pack();

  IndexVar i("i");
  IndexVar i0("i0"), i1("i1");
  IndexExpr precomputedExpr = B(i) * C(i);
  A(i) = precomputedExpr;

  IndexStmt stmt = A.getAssignment().concretize();
  TensorVar precomputed("precomputed", Type(Float64, {Dimension(i1)}), taco::dense);
  stmt = stmt.bound(i, 18, BoundType::MaxExact)
             .bound(i, 16, BoundType::MaxExact)
             .split(i, i0, i1, 5)
             .precompute(precomputedExpr, i1, i1, precomputed);
   
  A.compile(stmt.concretize());
  A.assemble();
  A.compute();

  Tensor<double> expected("expected", {16}, Format{Dense});
  expected(i) = B(i) * C(i);
  expected.compile();
  expected.assemble();
  expected.compute();
  ASSERT_TENSOR_EQ(expected, A);
}


// TEST(bound, bound_and_fuse) {
  
//   Tensor<double> A("A", {16}, Format{Dense});
//   Tensor<double> B("B", {16}, Format{Dense});
//   Tensor<double> C("C", {16}, Format{Dense});

//   for (int i = 0; i < 16; i++) {
//       A.insert({i}, (double) i);
//       B.insert({i}, (double) i);
//   }

//   A.pack();
//   B.pack();

//   IndexVar i("i");
//   IndexVar i0("i0"), i1("i1");
//   IndexExpr precomputedExpr = B(i) * C(i);
//   A(i) = precomputedExpr;

//   IndexStmt stmt = A.getAssignment().concretize();
//   TensorVar precomputed("precomputed", Type(Float64, {Dimension(i1)}), taco::dense);
//   stmt = stmt.bound(i, 16, BoundType::MaxExact)
//              .split(i, i0, i1, 5)
//              .fuse()
//              .precompute(precomputedExpr, i1, i1, precomputed);
   
//   A.compile(stmt.concretize());
//   A.assemble();
//   A.compute();

//   Tensor<double> expected("expected", {16}, Format{Dense});
//   expected(i) = B(i) * C(i);
//   expected.compile();
//   expected.assemble();
//   expected.compute();
//   ASSERT_TENSOR_EQ(expected, A);
// }

TEST(bound, bound_and_split) {
  
  Tensor<double> A("A", {17}, Format{Dense});
  Tensor<double> B("B", {17}, Format{Dense});
  Tensor<double> C("C", {17}, Format{Dense});

  for (int i = 0; i < 17; i++) {
      A.insert({i}, (double) i);
      B.insert({i}, (double) i);
  }

  A.pack();
  B.pack();

  IndexVar i("i");
  IndexVar i0("i0"), i1("i1");
  IndexExpr precomputedExpr = B(i) * C(i);
  A(i) = precomputedExpr;

  IndexStmt stmt = A.getAssignment().concretize();
  TensorVar precomputed("precomputed", Type(Float64, {Dimension(i1)}), taco::dense);
  stmt = stmt.bound(i, 17, BoundType::MaxExact)
             .split(i, i0, i1, 4)
             .precompute(precomputedExpr, i1, i1, precomputed);
   
  A.compile(stmt.concretize());
  A.assemble();
  A.compute();

  Tensor<double> expected("expected", {17}, Format{Dense});
  expected(i) = B(i) * C(i);
  expected.compile();
  expected.assemble();
  expected.compute();

  ASSERT_TENSOR_EQ(expected, A);

//  ir::IRPrinter irp = ir::IRPrinter(cout);
//    
//  cout << stmt << endl;
//
//  std::shared_ptr<ir::CodeGen> codegen = ir::CodeGen::init_default(cout, ir::CodeGen::ImplementationGen);
//  ir::Stmt compute = lower(stmt, "compute",  false, true);
//  
//  irp.print(compute);
//  cout << endl;
//  codegen->compile(compute, false);
}

