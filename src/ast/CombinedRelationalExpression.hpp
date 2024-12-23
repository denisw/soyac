/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _COMBINED_RELATIONAL_EXPRESSION_HPP
#define _COMBINED_RELATIONAL_EXPRESSION_HPP

#include "Expression.hpp"

namespace soyac {
namespace ast {

class DeclaredEntity;

/**
 * Represents a relational expression which combines multiple relational
 * operators, that is, an "<=" or ">=" expression.
 *
 * (See the Soya Language Reference,)
 */
class CombinedRelationalExpression : public Expression {
public:
    /**
     * Creates an CombinedRelationalExpression.
     *
     * @param leftHand          The expression's left-hand expression.
     * @param rightHand         The expression's right-hand expression.
     * @param equalsCallee      The called "equals" member.
     * @param comparisonCallee  The called "lessThan" or "greaterThan"
     * member.
     */
    CombinedRelationalExpression(Expression* leftHand, Expression* rightHand,
        DeclaredEntity* equalsCallee, DeclaredEntity* comparisonCallee);

    /**
     * Visits the CombinedRelationalExpression.
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
     * Returns the "equals" member of the left-hand operand that is called
     * for the "==" part of the comparison. This can be any callable entity,
     * not just a function.
     *
     * @return  The called "equals" member.
     */
    DeclaredEntity* equalsCallee() const;

    /**
     * Returns the "lessThan" or "greaterThan" member of the left-hand
     * operand to be called for the "<" or ">" part of the comparison. This
     * can be any callable entity, not just a function.
     *
     * @return  The called "lessThan" or "greaterThan" member.
     */
    DeclaredEntity* comparisonCallee() const;

private:
    Link<Expression> mLeftHand;
    Link<Expression> mRightHand;
    Link<DeclaredEntity> mEqualsCallee;
    Link<DeclaredEntity> mComparisonCallee;
};

} // namespace ast
} // namespace soyac

#endif
