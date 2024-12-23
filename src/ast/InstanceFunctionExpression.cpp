/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "InstanceFunctionExpression.h"
#include "FunctionType.h"
#include "UnknownType.h"
#include "Visitor.h"
#include <cassert>

namespace soyac {
namespace ast {

InstanceFunctionExpression::InstanceFunctionExpression(
    Expression* instance, Function* target)
    : mInstance(instance)
    , mTarget(target)
{
    assert(instance != nullptr);
    assert(target != nullptr);
}

void* InstanceFunctionExpression::visit(Visitor* v)
{
    return v->visitInstanceFunctionExpression(this);
}

Type* InstanceFunctionExpression::type() const { return target()->type(); }

Expression* InstanceFunctionExpression::instance() const
{
    return mInstance.target();
}

Function* InstanceFunctionExpression::target() const
{
    return mTarget.target();
}

} // namespace ast
} // namespace soyac
