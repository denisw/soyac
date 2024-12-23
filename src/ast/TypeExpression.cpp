/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "TypeExpression.hpp"
#include "UnknownType.hpp"
#include "Visitor.hpp"
#include <cassert>

namespace soyac {
namespace ast {

TypeExpression::TypeExpression(Type* target)
    : mTarget(target)
{
    assert(target != NULL);
}

void* TypeExpression::visit(Visitor* v) { return v->visitTypeExpression(this); }

Type* TypeExpression::type() const { return TYPE_UNKNOWN; }

Type* TypeExpression::target() const { return mTarget.target(); }

} // namespace ast
} // namespace soyac
