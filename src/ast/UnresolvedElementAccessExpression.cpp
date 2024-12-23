/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedElementAccessExpression.hpp"
#include "UnknownType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast {

UnresolvedElementAccessExpression::UnresolvedElementAccessExpression(
    Expression* operand, Expression* index)
    : mOperand(operand)
    , mIndex(index)
{
    assert(operand != NULL);
    assert(index != NULL);
}

void* UnresolvedElementAccessExpression::visit(Visitor* v)
{
    return v->visitUnresolvedElementAccessExpression(this);
}

Type* UnresolvedElementAccessExpression::type() const { return TYPE_UNKNOWN; }

Expression* UnresolvedElementAccessExpression::operand() const
{
    return mOperand.target();
}

Expression* UnresolvedElementAccessExpression::index() const
{
    return mIndex.target();
}

} // namespace ast
} // namespace soyac
