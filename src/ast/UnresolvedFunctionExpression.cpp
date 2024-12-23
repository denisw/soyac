/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedFunctionExpression.hpp"
#include "UnknownType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast {

void* UnresolvedFunctionExpression::visit(Visitor* v)
{
    return v->visitUnresolvedFunctionExpression(this);
}

Type* UnresolvedFunctionExpression::type() const { return TYPE_UNKNOWN; }

UnresolvedFunctionExpression::overloads_iterator
UnresolvedFunctionExpression::overloads_begin() const
{
    return mOverloads.begin();
}

UnresolvedFunctionExpression::overloads_iterator
UnresolvedFunctionExpression::overloads_end() const
{
    return mOverloads.end();
}

} // namespace ast
} // namespace soyac
