/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "FunctionExpression.hpp"
#include "FunctionType.hpp"
#include "Visitor.hpp"
#include <cassert>

namespace soyac {
namespace ast {

FunctionExpression::FunctionExpression(Function* target)
    : mTarget(target)
{
    assert(target != NULL);
}

void* FunctionExpression::visit(Visitor* v)
{
    return v->visitFunctionExpression(this);
}

Type* FunctionExpression::type() const { return target()->type(); }

Function* FunctionExpression::target() const { return mTarget.target(); }

} // namespace ast
} // namespace soyac
