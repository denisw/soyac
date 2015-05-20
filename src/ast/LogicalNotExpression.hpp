/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _LOGICAL_NOT_EXPRESSION_HPP
#define _LOGICAL_NOT_EXPRESSION_HPP

#include "Expression.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents a logical-not expression.
 *
 * (See the Soya Language Reference,)
 */
class LogicalNotExpression : public Expression
{
public:
    /**
     * Creates a LogicalNotExpression.
     *
     * @param operand  The expression's operand.
     */
    LogicalNotExpression(Expression* operand);

    /**
     * Visits the LogicalNotExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the expression's operand.
     *
     * @returns  The operand.
     */
    Expression* operand() const;

private:
    Link<Expression> mOperand;
};

}}

#endif
