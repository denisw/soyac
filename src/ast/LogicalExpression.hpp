/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _LOGICAL_EXPRESSION_HPP
#define _LOGICAL_EXPRESSION_HPP

#include "Expression.hpp"

namespace soyac {
namespace ast {

/**
 * Represents a logical AND or OR expression.
 *
 * (See the Soya Language Reference,)
 */
class LogicalExpression : public Expression {
public:
    enum Kind { AND, OR };

    /**
     * Creates a LogicalExpression.
     *
     * @param kind       The expression's operator.
     * @param leftHand   The expression's left-hand expression.
     * @param rightHand  The expression's right-hand expression.
     */
    LogicalExpression(Kind kind, Expression* leftHand, Expression* rightHand);

    /**
     * Visits the LogicalExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the kind of operator used in the expression.
     *
     * @return  The expression's kind.
     */
    Kind kind() const;

    /**
     * Returns the expression's left-hand expression.
     *
     * @returns  The left-hand expression.
     */
    Expression* leftHand() const;

    /**
     * Returns the expression's right-hand expression.
     *
     * @returns  The right-hand expression.
     */
    Expression* rightHand() const;

private:
    Kind mKind;
    Link<Expression> mLeftHand;
    Link<Expression> mRightHand;
};

} // namespace ast
} // namespace soyac

#endif
