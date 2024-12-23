/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_VARIABLE_EXPRESSION_H_
#define SOYA_VARIABLE_EXPRESSION_H_

#include "Expression.h"
#include "Variable.h"

namespace soyac {
namespace ast {

/**
 * Represents an expression that denotes a non-instance variable.
 * (To represent an expression denoting an instance variable of
 * a particular instance, see MemberAccessExpression.)
 */
class VariableExpression : public Expression {
public:
    /**
     * Creates a VariableExpression.
     *
     * @param target  The denoted variable.
     */
    VariableExpression(Variable* target);

    /**
     * Visits the VariableExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the variable denoted by the expression.
     *
     * @returns  The denoted variable.
     */
    Variable* target() const;

private:
    Link<Variable> mTarget;
};

} // namespace ast
} // namespace soyac

#endif
