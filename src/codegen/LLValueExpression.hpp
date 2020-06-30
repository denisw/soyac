/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _LL_VALUE_EXPRESSION_HPP
#define _LL_VALUE_EXPRESSION_HPP

#include <llvm/IR/Value.h>
#include <ast/Expression.hpp>
#include <ast/Type.hpp>

using namespace soyac::ast;

namespace soyac {
namespace codegen
{

/**
 * Wrapper for an already-evaluated LLVM value.
 */
class LLValueExpression : public Expression
{
public:
    /**
     * Creates a LLValueExpression.
     *
     * @param type   The expression's type.
     * @param value  The LLVM Value to wrap.
     */
    LLValueExpression(Type* type, llvm::Value* value);

    /**
     * Visits the LLValueExpression.
     *
     * @see ASTNode::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the LLVM value wrapped by the LLValueExpression.
     *
     * @returns  The wrapped LLVM value.
     */
    llvm::Value* value() const;

private:
    Link<Type> mType;
    llvm::Value* mValue;
};

}}

#endif
