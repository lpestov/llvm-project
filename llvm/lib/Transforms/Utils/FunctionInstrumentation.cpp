//===-- FunctionInstrumentation.cpp - Simple instrumentation pass ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Utils/FunctionInstrumentation.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"

using namespace llvm;

PreservedAnalyses
FunctionInstrumentationPass::run(Function &F, FunctionAnalysisManager &AM) {
  (void)AM;

  if (F.isDeclaration() || F.getName() == "instrument_start" ||
      F.getName() == "instrument_end")
    return PreservedAnalyses::all();

  LLVMContext &Ctx = F.getContext();
  FunctionType *VoidFnTy = FunctionType::get(Type::getVoidTy(Ctx), false);

  FunctionCallee Start =
      F.getParent()->getOrInsertFunction("instrument_start", VoidFnTy);
  FunctionCallee End =
      F.getParent()->getOrInsertFunction("instrument_end", VoidFnTy);

  IRBuilder<> Builder(&*F.getEntryBlock().getFirstInsertionPt());
  Builder.CreateCall(Start);

  for (BasicBlock &BB : F) {
    if (auto *Ret = dyn_cast<ReturnInst>(BB.getTerminator())) {
      IRBuilder<> RetBuilder(Ret);
      RetBuilder.CreateCall(End);
    }
  }

  return PreservedAnalyses::none();
}
