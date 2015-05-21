/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "LogicalExpression.hpp"
#include "BooleanType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


LogicalExpression::LogicalExpression(LogicalExpression::Kind kind,
                                     Expression* leftHand,
                                     Expression* rightHand)
    : mKind(kind),
      mLeftHand(leftHand),
      mRightHand(rightHand)
{
    assert (leftHand != NULL);
    assert (rightHand != NULL);
}


void*
LogicalExpression::visit(Visitor* v)
{
    return v->visitLogicalExpression(this);
}


Type*
LogicalExpression::type() const
{
    return TYPE_BOOL;
}


LogicalExpression::Kind
LogicalExpression::kind() const
{
    return mKind;
}


Expression*
LogicalExpression::leftHand() const
{
    return mLeftHand.target();
}


Expression*
LogicalExpression::rightHand() const
{
    return mRightHand.target();
}


}}
