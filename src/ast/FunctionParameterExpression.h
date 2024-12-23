/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_FUNCTION_PARAMETER_EXPRESSION_H_
#define SOYA_FUNCTION_PARAMETER_EXPRESSION_H_

#include "Expression.h"
#include "FunctionParameter.h"

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
