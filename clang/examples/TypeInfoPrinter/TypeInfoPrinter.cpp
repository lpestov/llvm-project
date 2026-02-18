//===- TypeInfoPrinter.cpp ------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>
#include <string>

using namespace clang;
using namespace std;

namespace {

// получаем строку для спецификатора доступа
string getAccessStr(AccessSpecifier AS) {
    if (AS == AS_public) return "public";
    if (AS == AS_protected) return "protected";
    if (AS == AS_private) return "private";
    return "none";
}

class TypeInfoVisitor : public RecursiveASTVisitor<TypeInfoVisitor> {
    ASTContext &Context;
public:
    TypeInfoVisitor(ASTContext &ctx) : Context(ctx) {}

    bool VisitCXXRecordDecl(CXXRecordDecl *Record) {
        // пропускаем всё лишнее
        if (!Record->isThisDeclarationADefinition()) return true;
        if (Record->isImplicit() || Record->isLambda()) return true;

        const SourceManager &SM = Context.getSourceManager();
        if (!SM.isWrittenInMainFile(Record->getLocation())) return true;

        // выводим имя класса
        llvm::outs() << Record->getNameAsString();

        // собираем базовые классы
        vector<string> bases;
        for (auto &Base : Record->bases()) {
            auto *BaseDecl = Base.getType()->getAsCXXRecordDecl();
            if (BaseDecl)
                bases.push_back(BaseDecl->getNameAsString());
            else
                bases.push_back(Base.getType().getAsString());
        }

        if (!bases.empty()) {
            llvm::outs() << " -> ";
            for (int i = 0; i < bases.size(); i++) {
                if (i != 0) llvm::outs() << ", ";
                llvm::outs() << bases[i];
            }
        }
        llvm::outs() << "\n";

        // поля
        llvm::outs() << "|_Fields\n";
        for (auto *Field : Record->fields()) {
            llvm::outs() << "| |_ " << Field->getNameAsString()
                         << " (" << Field->getType().getAsString()
                         << "|" << getAccessStr(Field->getAccess()) << ")\n";
        }

        // методы
        llvm::outs() << "|\n";
        llvm::outs() << "|_Methods\n";
        for (auto *Method : Record->methods()) {
            if (Method->isImplicit()) continue;
            // пропускаем конструкторы, деструкторы и операторы приведения
            if (isa<CXXConstructorDecl>(Method)) continue;
            if (isa<CXXDestructorDecl>(Method)) continue;
            if (isa<CXXConversionDecl>(Method)) continue;

            llvm::outs() << "| |_ " << Method->getNameAsString()
                         << " (" << Method->getReturnType().getAsString() << "(";

            for (unsigned i = 0; i < Method->getNumParams(); i++) {
                if (i != 0) llvm::outs() << ", ";
                llvm::outs() << Method->getParamDecl(i)->getType().getAsString();
            }
            llvm::outs() << ")|" << getAccessStr(Method->getAccess());

            // определяем виртуальность
            if (Method->isPureVirtual()) {
                llvm::outs() << "|virtual|pure";
            } else if (Method->size_overridden_methods() > 0) {
                llvm::outs() << "|override";
            } else if (Method->isVirtual()) {
                llvm::outs() << "|virtual";
            }

            llvm::outs() << ")\n";
        }

        llvm::outs() << "\n";
        return true;
    }
};

class TypeInfoConsumer : public ASTConsumer {
public:
    void HandleTranslationUnit(ASTContext &Context) override {
        TypeInfoVisitor visitor(Context);
        visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }
};

class TypeInfoAction : public PluginASTAction {
protected:
    unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                              llvm::StringRef) override {
        return make_unique<TypeInfoConsumer>();
    }

    bool ParseArgs(const CompilerInstance &,
                   const vector<string> &) override {
        return true;
    }

    PluginASTAction::ActionType getActionType() override {
        return AddBeforeMainAction;
    }
};

} // namespace

static FrontendPluginRegistry::Add<TypeInfoAction>
    X("type-info-printer", "print fields, methods and base classes");