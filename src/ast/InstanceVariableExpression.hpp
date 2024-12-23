/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _INSTANCE_VARIABLE_EXPRESSION_HPP
#define _INSTANCE_VARIABLE_EXPRESSION_HPP

#include "Expression.hpp"
#include "Variable.hpp"

namespace soyac {
namespace ast {

/**
 * Represents an expression that denotes an instance variable.
 *
 * @see VariableExpression
 */
class InstanceVariableExpression : public Expression {
public:
    /**
     * Creates an InstanceVariableExpression.
     *
     * @param instance  The instance whose variable is denoted.
     * @param target    The denoted function.
     */
    InstanceVariableExpression(Expression* instance, Variable* target);

    /**
     * Visits the InstanceVariableExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the expression yielding the instance whose variable is
     * denoted by the InstanceVariableExpression.
     *
     * @return  The denoted instance.
     */
    Expression* instance() const;

    /**
     * Returns the variable denoted by the InstanceVariableExpression.
     *
     * @returns  The denoted variable.
     */
    Variable* target() const;

private:
    Link<Expression> mInstance;
    Link<Variable> mTarget;
};

} // namespace ast
} // namespace soyac

#endif
