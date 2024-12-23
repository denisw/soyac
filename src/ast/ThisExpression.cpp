/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ThisExpression.h"
#include "Visitor.h"
#include <cassert>

namespace soyac {
namespace ast {

ThisExpression::ThisExpression(Type* type)
    : mType(type)
{
    assert(type != nullptr);
}

void* ThisExpression::visit(Visitor* v) { return v->visitThisExpression(this); }

Type* ThisExpression::type() const { return mType.target(); }

} // namespace ast
} // namespace soyac
