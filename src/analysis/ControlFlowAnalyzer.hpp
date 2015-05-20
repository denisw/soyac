/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _CONTROL_FLOW_ANALYZER_HPP
#define _CONTROL_FLOW_ANALYZER_HPP

#include <list>
#include <common/PassResult.hpp>
#include <common/PassResultBuilder.hpp>
#include "NavigatingVisitor.hpp"

using namespace soyac::ast;

namespace soyac {
namespace analysis
{

/**
 * The control flow analysis pass.
 */
class ControlFlowAnalyzer : public NavigatingVisitor
{
public:
    /**
     * Creates a BasicAnalyzer.
     */
    ControlFlowAnalyzer();

    /**
     * Analyzes the passed module and returns a PassResult instance
     * summarizing the problems found during analysis, if any.
     *
     * @return   The analysis' result (owned by the caller), or null.
     */
    PassResult* analyze(Module* m);

protected:
    /**
     * Visits a Block.
     *
     * @param b  The Block to visit.
     * @return   @c true if the statement always causes the enclosing
     *           function (if existent) to return;
     *           @c false otherwise.
     */
    virtual void* visitBlock(Block* b);

    /**
     * Visits a Constructor.
     *
     * @param cons  The Constructor to visit.
     * @return      Nothing.
     */
    virtual void* visitConstructor(Constructor* cons);

    /**
     * Visits a DeclarationStatement.
     *
     * @param stmt  The DeclarationStatement to visit.
     * @return      @c false (indicates that the statement never causes
     *              the enclosing function to return).
     */
    virtual void* visitDeclarationStatement(DeclarationStatement* stmt);

    /**
     * Visits a Function.
     *
     * @param func  The Function to visit.
     * @return      Nothing.
     */
    virtual void* visitFunction(Function* func);

    /**
     * Visits a FunctionParameterExpression.
     *
     * @param expr  The FunctionParameterExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitFunctionParameterExpression(FunctionParameterExpression* expr);

    /**
     * Visits an IfStatement.
     *
     * @param stmt  The IfStatement to visit.
     * @return      @c true if the statement always causes the enclosing
     *              function (if existent) to return;
     *              @c false otherwise.
     */
    virtual void* visitIfStatement(IfStatement* stmt);

    /**
     * Visits a PropertyGetAccessor.
     *
     * @param acc  The PropertyGetAccessor to visit.
     * @return     Nothing.
     */
    virtual void* visitPropertyGetAccessor(PropertyGetAccessor* acc);

    /**
     * Visits a PropertySetAccessor.
     *
     * @param acc  The PropertySetAccessor to visit.
     * @return     Nothing.
     */
    virtual void* visitPropertySetAccessor(PropertySetAccessor* acc);

    /**
     * Visits a ReturnStatement.
     *
     * @param stmt  The ReturnStatement to visit.
     * @return      @c true (indicates that the statement always causes
     *              the enclosing function to return).
     */
    virtual void* visitReturnStatement(ReturnStatement* stmt);

    /**
     * Visits a VariableExpression.
     *
     * @param expr  The VariableExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitVariableExpression(VariableExpression* expr);

private:
    PassResultBuilder* mRBuilder;
    std::list<DeclaredEntity*> mUnused;

    /**
     * Reports the unused declared entities found by the ControlFlowAnalyzer,
     * and clears the list where the unused entities are collected.
     */
    void reportUnused();
};


}}

#endif
