/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FUNCTION_PARAMETER_EXPRESSION_HPP
#define _FUNCTION_PARAMETER_EXPRESSION_HPP

#include "Expression.hpp"
#include "FunctionParameter.hpp"

namespace soyac {
namespace ast {

/**
 * Represents an expression that denotes a function parameter.
 */
class FunctionParameterExpression : public Expression {
public:
    /**
     * Creates a FunctionParameterExpression.
     *
     * @param target  The denoted function parameter.
     */
    FunctionParameterExpression(FunctionParameter* target);

    /**
     * Visits the FunctionParameterExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the function parameter denoted by the expression.
     *
     * @returns  The denoted function parameter.
     */
    FunctionParameter* target() const;

private:
    Link<FunctionParameter> mTarget;
};

} // namespace ast
} // namespace soyac

#endif
