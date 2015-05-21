/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "FunctionParameterExpression.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


FunctionParameterExpression::FunctionParameterExpression(FunctionParameter* target)
    : mTarget(target)
{
}


void*
FunctionParameterExpression::visit(Visitor* v)
{
    return v->visitFunctionParameterExpression(this);
}


Type*
FunctionParameterExpression::type() const
{
    return target()->type();
}


FunctionParameter*
FunctionParameterExpression::target() const
{
    return mTarget.target();
}


}}
