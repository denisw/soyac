/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNRESOLVED_ELEMENT_ACCESS_EXPRESSION_H_
#define SOYA_UNRESOLVED_ELEMENT_ACCESS_EXPRESSION_H_

#include "Expression.h"

namespace soyac {
namespace ast {

/**
 * Represents an element access expression that acts as a shortcut for a
 * call to a to-be-determined getElement() or setElement() method.
 *
 * UnresolvedElementAccessExpression instances are replaced with
 * corresponding CallExpression instances by the BasicAnalyzer.
 *
 * (See the Soya Language Reference,)
 */
class UnresolvedElementAccessExpression : public Expression {
public:
    /**
     * Creates an UnresolvedElementAccessExpression.
     *
     * @param operand  The expression's operand.
     * @param index    The element index expression.
     */
    UnresolvedElementAccessExpression(Expression* operand, Expression* index);

    /**
     * Visits the UnresolvedElementAccessExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the operand of the element access expression.
     *
     * @returns  The expression's operand.
     */
    Expression* operand() const;

    /**
     * Returns the expression yielding the index of the element to
     * be accessed by the expression.
     *
     * @returns  The expression' element index expression.
     */
    Expression* index() const;

private:
    Link<Expression> mOperand;
    Link<Expression> mIndex;
};

} // namespace ast
} // namespace soyac

#endif
