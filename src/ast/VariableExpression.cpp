/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "VariableExpression.hpp"
#include "Visitor.hpp"
#include <cassert>

namespace soyac {
namespace ast {

VariableExpression::VariableExpression(Variable* target)
    : mTarget(target)
{
    assert(target != NULL);
}

void* VariableExpression::visit(Visitor* v)
{
    return v->visitVariableExpression(this);
}

Type* VariableExpression::type() const { return target()->type(); }

Variable* VariableExpression::target() const { return mTarget.target(); }

} // namespace ast
} // namespace soyac
