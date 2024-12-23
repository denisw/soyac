/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "EnumValueExpression.hpp"
#include "Visitor.hpp"
#include <cassert>

namespace soyac {
namespace ast {

EnumValueExpression::EnumValueExpression(EnumType* type, EnumConstant* target)
    : mType(type)
    , mTarget(target)
{
    assert(type != NULL);
    assert(target != NULL);
}

void* EnumValueExpression::visit(Visitor* v)
{
    return v->visitEnumValueExpression(this);
}

Type* EnumValueExpression::type() const { return mType.target(); }

EnumConstant* EnumValueExpression::target() const { return mTarget.target(); }

} // namespace ast
} // namespace soyac
