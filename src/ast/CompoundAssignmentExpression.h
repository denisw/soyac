/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_COMPOUND_ASSIGNMENT_EXPRESSION_H_
#define SOYA_COMPOUND_ASSIGNMENT_EXPRESSION_H_

#include "Expression.h"

namespace soyac {
namespace ast {

class DeclaredEntity;

/**
 * Represents a relational expression which combines multiple relational
 * operators, that is, an "<=" or ">=" expression.
 *
 * (See the Soya Language Reference,)
 */
class CompoundAssignmentExpression : public Expression {
public:
    /**
     * Creates an CompoundAssignmentExpression.
     *
     * @param leftHand          The expression's left-hand expression.
     * @param rightHand         The expression's right-hand expression.
     * @param operatorCallee    The called operator method.
     */
    CompoundAssignmentExpression(
        Expression* leftHand, Expression* rightHand, DeclaredEntity* callee);

    /**
     * Visits the CompoundAssignmentExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

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

    /**
     * Returns the operator method of the left-hand operand that is called
     * for to evaluate the value to assign.
     *
     * @return  The called operator method.
     */
    DeclaredEntity* callee() const;

private:
    Link<Expression> mLeftHand;
    Link<Expression> mRightHand;
    Link<DeclaredEntity> mCallee;
};

} // namespace ast
} // namespace soyac

#endif
