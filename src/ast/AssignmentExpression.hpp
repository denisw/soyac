/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _ASSIGNMENT_EXPRESSION_HPP
#define _ASSIGNMENT_EXPRESSION_HPP

#include "Expression.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents an assignment expression.
 *
 * (See the Soya Language Reference,)
 */
class AssignmentExpression : public Expression
{
public:
    /**
     * Creates an AssignmentExpression.
     *
     * @param leftHand   The assignment's left-hand expression.
     * @param rightHand  The assignment's right-hand expression.
     */
    AssignmentExpression(Expression* leftHand, Expression* rightHand);

    /**
     * Visits the AssignmentExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the assignment's left-hand expression.
     *
     * @returns  The left-hand expression.
     */
    Expression* leftHand() const;

    /**
     * Returns the assignment's right-hand expression.
     *
     * @returns  The right-hand expression.
     */
    Expression* rightHand() const;

private:
    Link<Expression> mLeftHand;
    Link<Expression> mRightHand;
};

}}

#endif
