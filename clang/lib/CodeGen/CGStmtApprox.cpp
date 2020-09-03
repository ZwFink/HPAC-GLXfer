//===--- CGStmtApprox.cpp - Emit LLVM Code from Statements ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This contains code to emit Approx nodes as LLVM code.
//
//===----------------------------------------------------------------------===//

#include "CodeGenFunction.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/StmtApprox.h"
#include "clang/Basic/Approx.h"
#include "llvm/Support/Debug.h"

using namespace clang;
using namespace CodeGen;
using namespace llvm;



void CodeGenFunction::EmitApproxDirective(const ApproxDirective &AD) {
  CGApproxRuntime &RT = CGM.getApproxRuntime();
  CapturedStmt *CStmt = cast<CapturedStmt>(AD.getAssociatedStmt());

  RT.CGApproxRuntimeEnterRegion(*this, *CStmt);
  for (auto C : AD.clauses()) {
    if (ApproxIfClause *IfClause = dyn_cast_or_null<ApproxIfClause>(C)) {
      RT.CGApproxRuntimeEmitIfInit(*this, *IfClause);
    }
    else if (ApproxPerfoClause *PerfoClause = dyn_cast_or_null<ApproxPerfoClause>(C)){
      RT.CGApproxRuntimeEmitPerfoInit(*this, *CStmt, *PerfoClause);
    }
  }

  RT.CGApproxRuntimeExitRegion(*this);
}
