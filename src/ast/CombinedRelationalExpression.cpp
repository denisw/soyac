/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "CombinedRelationalExpression.h"
#include "BooleanType.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

CombinedRelationalExpression::CombinedRelationalExpression(Expression* leftHand,
    Expression* rightHand, DeclaredEntity* equalsCallee,
    DeclaredEntity* comparisonCallee)
    : mLeftHand(leftHand)
    , mRightHand(rightHand)
    , mEqualsCallee(equalsCallee)
    , mComparisonCallee(comparisonCallee)
{
    assert(leftHand != nullptr);
    assert(rightHand != nullptr);
    assert(equalsCallee != nullptr);
    assert(equalsCallee->name() == Name("equals"));
    assert(comparisonCallee != nullptr);
    assert(comparisonCallee->name() == Name("lessThan")
        || comparisonCallee->name() == Name("greaterThan"));
}

void* CombinedRelationalExpression::visit(Visitor* v)
{
    return v->visitCombinedRelationalExpression(this);
}

Type* CombinedRelationalExpression::type() const { return TYPE_BOOL; }

Expression* CombinedRelationalExpression::leftHand() const
{
    return mLeftHand.target();
}

Expression* CombinedRelationalExpression::rightHand() const
{
    return mRightHand.target();
}

DeclaredEntity* CombinedRelationalExpression::equalsCallee() const
{
    return mEqualsCallee.target();
}

DeclaredEntity* CombinedRelationalExpression::comparisonCallee() const
{
    return mComparisonCallee.target();
}

} // namespace ast
} // namespace soyac
