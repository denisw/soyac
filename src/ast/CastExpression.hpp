/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _CAST_EXPRESSION_HPP
#define _CAST_EXPRESSION_HPP

#include "Expression.hpp"
#include "Type.hpp"

namespace soyac {
namespace ast {

/**
 * Represents a cast expression.
 *
 * (See the Soya Language Reference,)
 */
class CastExpression : public Expression {
public:
    /**
     * Creates an CastExpression.
     *
     * @param operand  The expression whose value is converted.
     * @param type     The converted value's type.
     */
    CastExpression(Expression* operand, Type* type);

    /**
     * Visits the CastExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the expression whose value is converted with the cast.
     *
     * @returns  The cast expression's operand.
     */
    Expression* operand() const;

private:
    Link<Expression> mOperand;
    Link<Type> mType;
};

} // namespace ast
} // namespace soyac

#endif
