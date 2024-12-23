/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedThisExpression.hpp"
#include "UnknownType.hpp"
#include "Visitor.hpp"
#include <cassert>

namespace soyac {
namespace ast {

void* UnresolvedThisExpression::visit(Visitor* v)
{
    return v->visitUnresolvedThisExpression(this);
}

Type* UnresolvedThisExpression::type() const { return TYPE_UNKNOWN; }

} // namespace ast
} // namespace soyac
