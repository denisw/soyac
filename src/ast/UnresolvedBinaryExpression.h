/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNRESOLVED_BINARY_EXPRESSION_H_
#define SOYA_UNRESOLVED_BINARY_EXPRESSION_H_

#include "Expression.h"

namespace soyac {
namespace ast {

/**
 * Represents a binary operator expression that needs to be resolved
 * to its corresponding operator method call expression.
 *
 * UnresolvedBinaryExpression instances are replaced with corresponding
 * CallExpression instances by the BasicAnalyzer.
 *
 * (See the Soya Language Reference,)
 */
class UnresolvedBinaryExpression : public Expression {
public:
    enum Kind {
        PLUS,
        MINUS,
        MUL,
        DIV,
        MOD,
        LSHIFT,
        RSHIFT,
        PLUS_ASSIGN,
        MINUS_ASSIGN,
        MUL_ASSIGN,
        DIV_ASSIGN,
        MOD_ASSIGN,
        LSHIFT_ASSIGN,
        RSHIFT_ASSIGN,
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE
    };

    /**
     * Creates an UnresolvedBinaryExpression.
     *
     * @param kind       The expression's operator.
     * @param leftHand   The expression's left-hand expression.
     * @param rightHand  The expression's right-hand expression.
     */
    UnresolvedBinaryExpression(
        Kind kind, Expression* leftHand, Expression* rightHand);

    /**
     * Visits the UnresolvedBinaryExpression.
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
