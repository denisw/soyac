/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "AssignmentExpression.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast {

AssignmentExpression::AssignmentExpression(
    Expression* leftHand, Expression* rightHand)
    : mLeftHand(leftHand)
    , mRightHand(rightHand)
{
    assert(leftHand != NULL);
    assert(rightHand != NULL);
}

void* AssignmentExpression::visit(Visitor* v)
{
    return v->visitAssignmentExpression(this);
}

Type* AssignmentExpression::type() const { return leftHand()->type(); }

Expression* AssignmentExpression::leftHand() const
{
    return mLeftHand.target();
}

Expression* AssignmentExpression::rightHand() const
{
    return mRightHand.target();
}

} // namespace ast
} // namespace soyac
