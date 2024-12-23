/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "CompoundAssignmentExpression.h"
#include "BooleanType.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

CompoundAssignmentExpression::CompoundAssignmentExpression(
    Expression* leftHand, Expression* rightHand, DeclaredEntity* callee)
    : mLeftHand(leftHand)
    , mRightHand(rightHand)
    , mCallee(callee)
{
    assert(leftHand != nullptr);
    assert(rightHand != nullptr);
    assert(callee != nullptr);
}

void* CompoundAssignmentExpression::visit(Visitor* v)
{
    return v->visitCompoundAssignmentExpression(this);
}

Type* CompoundAssignmentExpression::type() const { return leftHand()->type(); }

Expression* CompoundAssignmentExpression::leftHand() const
{
    return mLeftHand.target();
}

Expression* CompoundAssignmentExpression::rightHand() const
{
    return mRightHand.target();
}

DeclaredEntity* CompoundAssignmentExpression::callee() const
{
    return mCallee.target();
}

} // namespace ast
} // namespace soyac
