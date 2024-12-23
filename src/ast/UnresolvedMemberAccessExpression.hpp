/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_MEMBER_ACCESS_EXPRESSION_HPP
#define _UNRESOLVED_MEMBER_ACCESS_EXPRESSION_HPP

#include "Expression.hpp"
#include "Function.hpp"
#include <cassert>

namespace soyac {
namespace ast {

/**
 * Represents an expression denoting a to-be-determined member of
 * a specific instance.
 *
 * UnresolvedMemberAccessExpression instances are replaced with semantically
 * meaningful equivalents by the BasicAnalyzer.
 */
class UnresolvedMemberAccessExpression : public Expression {
public:
    /**
     * Creates a UnresolvedMemberAccessExpression.
     *
     * @param operand     The operand.
     * @param memberName  The accessed member's name.
     */
    UnresolvedMemberAccessExpression(
        Expression* operand, const Name& memberName);

    /**
     * Visits the UnresolvedMemberAccessExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the expression yielding the entity whose method is accessed.
     *
     * @returns  The expression's operand.
     */
    Expression* operand() const;

    /**
     * Returns the name of the member access by the expression.
     *
     * @return  The accessed member's name.
     */
    const Name& memberName() const;

private:
    Link<Expression> mOperand;
    Name mMemberName;
};

} // namespace ast
} // namespace soyac

#endif
