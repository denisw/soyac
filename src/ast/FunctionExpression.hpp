/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FUNCTION_EXPRESSION_HPP
#define _FUNCTION_EXPRESSION_HPP

#include "Expression.hpp"
#include "Function.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents an expression that denotes a non-instance function.
 *
 * @see InstanceFunctionExpression
 */
class FunctionExpression : public Expression
{
public:
    /**
     * Creates a FunctionExpression.
     *
     * @param target  The denoted function.
     */
    FunctionExpression(Function* target);

    /**
     * Visits the FunctionExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the function denoted by the expression.
     *
     * @returns  The denoted function.
     */
    Function* target() const;

private:
    Link<Function> mTarget;
};

}}

#endif
