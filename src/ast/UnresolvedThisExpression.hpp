/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_THIS_EXPRESSION_HPP
#define _UNRESOLVED_THIS_EXPRESSION_HPP

#include "Expression.hpp"
#include "Type.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents a "this" expression denoting an instance of a
 * to-be-determined type.
 */
class UnresolvedThisExpression : public Expression
{
public:
    /**
     * Visits the UnresolvedThisExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;
};

}}

#endif
