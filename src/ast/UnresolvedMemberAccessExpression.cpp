/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedMemberAccessExpression.h"
#include "UnknownType.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

UnresolvedMemberAccessExpression::UnresolvedMemberAccessExpression(
    Expression* operand, const Name& memberName)
    : mOperand(operand)
    , mMemberName(memberName)
{
}

void* UnresolvedMemberAccessExpression::visit(Visitor* v)
{
    return v->visitUnresolvedMemberAccessExpression(this);
}

Type* UnresolvedMemberAccessExpression::type() const { return TYPE_UNKNOWN; }

Expression* UnresolvedMemberAccessExpression::operand() const
{
    return mOperand.target();
}

const Name& UnresolvedMemberAccessExpression::memberName() const
{
    return mMemberName;
}

} // namespace ast
} // namespace soyac
