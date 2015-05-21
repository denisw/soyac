/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "LogicalNotExpression.hpp"
#include "BooleanType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


LogicalNotExpression::LogicalNotExpression(Expression* operand)
    : mOperand(operand)
{
    assert (operand != NULL);
}


void*
LogicalNotExpression::visit(Visitor* v)
{
    return v->visitLogicalNotExpression(this);
}


Type*
LogicalNotExpression::type() const
{
    return TYPE_BOOL;
}


Expression*
LogicalNotExpression::operand() const
{
    return mOperand.target();
}


}}
