/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "EnumValueExpression.h"
#include "Visitor.h"
#include <cassert>

namespace soyac {
namespace ast {

EnumValueExpression::EnumValueExpression(EnumType* type, EnumConstant* target)
    : mType(type)
    , mTarget(target)
{
    assert(type != nullptr);
    assert(target != nullptr);
}

void* EnumValueExpression::visit(Visitor* v)
{
    return v->visitEnumValueExpression(this);
}

Type* EnumValueExpression::type() const { return mType.target(); }

EnumConstant* EnumValueExpression::target() const { return mTarget.target(); }

} // namespace ast
} // namespace soyac
