/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "CastExpression.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast {

CastExpression::CastExpression(Expression* operand, Type* type)
    : mOperand(operand)
    , mType(type)
{
}

void* CastExpression::visit(Visitor* v) { return v->visitCastExpression(this); }

Type* CastExpression::type() const { return mType.target(); }

Expression* CastExpression::operand() const { return mOperand.target(); }

} // namespace ast
} // namespace soyac
