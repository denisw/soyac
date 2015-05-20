/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _TYPE_EXPRESSION_HPP
#define _TYPE_EXPRESSION_HPP

#include "Expression.hpp"
#include "Type.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents an expression that denotes a type.
 */
class TypeExpression : public Expression
{
public:
    /**
     * Creates a TypeExpression.
     *
     * @param target  The denoted type.
     */
    TypeExpression(Type* target);

    /**
     * Visits the TypeExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the type denoted by the expression.
     *
     * @returns  The denoted type.
     */
    Type* target() const;

private:
    Link<Type> mTarget;
};

}}

#endif
