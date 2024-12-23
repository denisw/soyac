/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedInstancePropertyExpression.h"
#include "UnknownType.h"
#include "Visitor.h"
#include <cassert>

namespace soyac {
namespace ast {

UnresolvedInstancePropertyExpression::UnresolvedInstancePropertyExpression(
    Expression* instance, Property* target)
    : mInstance(instance)
    , mTarget(target)
{
    assert(instance != nullptr);
    assert(target != nullptr);
}

void* UnresolvedInstancePropertyExpression::visit(Visitor* v)
{
    return v->visitUnresolvedInstancePropertyExpression(this);
}

Type* UnresolvedInstancePropertyExpression::type() const
{
    return TYPE_UNKNOWN;
}

Expression* UnresolvedInstancePropertyExpression::instance() const
{
    return mInstance.target();
}

Property* UnresolvedInstancePropertyExpression::target() const
{
    return mTarget.target();
}

} // namespace ast
} // namespace soyac
