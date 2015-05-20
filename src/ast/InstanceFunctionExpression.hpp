/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _INSTANCE_FUNCTION_EXPRESSION_HPP
#define _INSTANCE_FUNCTION_EXPRESSION_HPP

#include "Expression.hpp"
#include "Function.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents an expression that denotes an instance function.
 *
 * Note that, because PropertyGetAccessor, PropertySetAccessor and
 * Constructor all inherit from Function, references to these types
 * of nodes can also be represented as InstanceFunctionExpression
 * instances. In combination with CallExpression, this is used
 * to represent constructor calls and property access.
 *
 * @see FunctionExpression
 */
class InstanceFunctionExpression : public Expression
{
public:
    /**
     * Creates an InstanceFunctionExpression.
     *
     * @param instance  The instance whose function is denoted.
     * @param target    The denoted function.
     */
    InstanceFunctionExpression(Expression* instance, Function* target);

    /**
     * Visits the InstanceFunctionExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the expression yielding the instance whose function is
     * denoted by the InstanceFunctionExpression.
     *
     * @return  The denoted instance.
     */
    Expression* instance() const;

    /**
     * Returns the function denoted by the InstanceFunctionExpression.
     *
     * @returns  The denoted function.
     */
    Function* target() const;

private:
    Link<Expression> mInstance;
    Link<Function> mTarget;
};

}}

#endif
