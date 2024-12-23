/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedBinaryExpression.h"
#include "UnknownType.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

UnresolvedBinaryExpression::UnresolvedBinaryExpression(
    UnresolvedBinaryExpression::Kind kind, Expression* leftHand,
    Expression* rightHand)
    : mKind(kind)
    , mLeftHand(leftHand)
    , mRightHand(rightHand)
{
    assert(leftHand != nullptr);
    assert(rightHand != nullptr);
}

void* UnresolvedBinaryExpression::visit(Visitor* v)
{
    return v->visitUnresolvedBinaryExpression(this);
}

Type* UnresolvedBinaryExpression::type() const { return TYPE_UNKNOWN; }

UnresolvedBinaryExpression::Kind UnresolvedBinaryExpression::kind() const
{
    return mKind;
}

Expression* UnresolvedBinaryExpression::leftHand() const
{
    return mLeftHand.target();
}

Expression* UnresolvedBinaryExpression::rightHand() const
{
    return mRightHand.target();
}

} // namespace ast
} // namespace soyac
