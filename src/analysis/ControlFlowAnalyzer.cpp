/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ControlFlowAnalyzer.h"
#include <ast/ast.h>
#include <boost/format.hpp>
#include <cassert>

namespace soyac {
namespace analysis {

ControlFlowAnalyzer::ControlFlowAnalyzer() { }

PassResult* ControlFlowAnalyzer::analyze(Module* m)
{
    mRBuilder = new PassResultBuilder;

    visitModule(m);

    PassResult* r = mRBuilder->result();
    delete mRBuilder;

    return r;
}

///// Helper Methods
//////////////////////////////////////////////////////////////

void ControlFlowAnalyzer::reportUnused()
{
    for (std::list<DeclaredEntity*>::iterator it = mUnused.begin();
        it != mUnused.end(); it++) {
        /*
         * Variables
         */
        if (dynamic_cast<Variable*>(*it) != nullptr) {
            mRBuilder->addWarning(
                *it, boost::format("Unused variable '%1%'.") % *it);
        }

        /*
         * Function Parameters
         */
        else if (dynamic_cast<FunctionParameter*>(*it) != nullptr) {
            mRBuilder->addWarning(
                *it, boost::format("Unused function parameter '%1%'.") % *it);
        }

        /*
         * (Should never be reached.)
         */
        else {
            assert(false);
        }
    }

    mUnused.clear();
}

///// Statements
//////////////////////////////////////////////////////////////////

void* ControlFlowAnalyzer::visitBlock(Block* b)
{
    bool alwaysReturns = false;

    for (Block::statements_iterator it = b->statements_begin();
        it != b->statements_end(); it++) {
        bool ret = (bool)(*it)->visit(this);

        if (ret) {
            alwaysReturns = true;

            it++;
            if (it != b->statements_end()) {
                mRBuilder->addError(*it, "Unreachable statement.");
            }

            break;
        }
    }

    return (void*)alwaysReturns;
}

void* ControlFlowAnalyzer::visitDeclarationStatement(DeclarationStatement* stmt)
{
    if (dynamic_cast<Variable*>(stmt->declaredEntity()) != nullptr
        && stmt->declaredEntity()->qualifiedName().isSimple()) {
        mUnused.push_back(stmt->declaredEntity());
    }

    stmt->declaredEntity()->visit(this);
    return (void*)false;
}

void* ControlFlowAnalyzer::visitIfStatement(IfStatement* stmt)
{
    stmt->condition()->visit(this);

    bool alwaysReturns = (bool)stmt->body()->visit(this);

    if (stmt->elseBody() != nullptr) {
        alwaysReturns = ((bool)stmt->elseBody()->visit(this)) && alwaysReturns;
    }

    return (void*)alwaysReturns;
}

void* ControlFlowAnalyzer::visitReturnStatement(ReturnStatement* stmt)
{
    if (stmt->returnValue() != nullptr) {
        stmt->returnValue()->visit(this);
    }

    return (void*)true;
}

///// Declared Entities
///////////////////////////////////////////////////////////

void* ControlFlowAnalyzer::visitConstructor(Constructor* cons)
{
    if (cons->body() != nullptr) {
        for (Function::parameters_iterator it = cons->parameters_begin();
            it != cons->parameters_end(); it++) {
            mUnused.push_back(*it);
        }

        if (cons->initializer() != nullptr) {
            cons->initializer()->visit(this);
        }

        cons->body()->visit(this);
        reportUnused();
    }

    return nullptr;
}

void* ControlFlowAnalyzer::visitFunction(Function* func)
{
    if (func->body() != nullptr) {
        for (Function::parameters_iterator it = func->parameters_begin();
            it != func->parameters_end(); it++) {
            mUnused.push_back(*it);
        }

        bool alwaysReturning = (bool)func->body()->visit(this);

        if (!alwaysReturning && func->returnType() != TYPE_VOID) {
            mRBuilder->addError(func,
                boost::format(
                    "Function '%1%' might not return a value in all cases.")
                    % func);
        }

        reportUnused();
    }

    return nullptr;
}

void* ControlFlowAnalyzer::visitPropertyGetAccessor(PropertyGetAccessor* acc)
{
    if (acc->body() != nullptr) {
        for (Function::parameters_iterator it = acc->parameters_begin();
            it != acc->parameters_end(); it++) {
            mUnused.push_back(*it);
        }

        bool alwaysReturning = (bool)acc->body()->visit(this);

        if (!alwaysReturning) {
            mRBuilder->addError(acc,
                boost::format("Get accessor of property '%1%' might not "
                              "return a value "
                              "in all cases.")
                    % acc->parent());
        }

        reportUnused();
    }

    return nullptr;
}

void* ControlFlowAnalyzer::visitPropertySetAccessor(PropertySetAccessor* acc)
{
    if (acc->body() != nullptr) {
        acc->body()->visit(this);
        reportUnused();
    }

    return nullptr;
}

///// Expressions
/////////////////////////////////////////////////////////////////

void* ControlFlowAnalyzer::visitFunctionParameterExpression(
    FunctionParameterExpression* expr)
{
    mUnused.remove(expr->target());
    return nullptr;
}

void* ControlFlowAnalyzer::visitVariableExpression(VariableExpression* expr)
{
    mUnused.remove(expr->target());
    return nullptr;
}

} // namespace analysis
} // namespace soyac
