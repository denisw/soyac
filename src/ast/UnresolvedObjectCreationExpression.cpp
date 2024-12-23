/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedObjectCreationExpression.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast {

void* UnresolvedObjectCreationExpression::visit(Visitor* v)
{
    return v->visitUnresolvedObjectCreationExpression(this);
}

Type* UnresolvedObjectCreationExpression::type() const
{
    return mType.target();
}

UnresolvedObjectCreationExpression::arguments_iterator
UnresolvedObjectCreationExpression::arguments_begin() const
{
    return mArguments.begin();
}

UnresolvedObjectCreationExpression::arguments_iterator
UnresolvedObjectCreationExpression::arguments_end() const
{
    return mArguments.end();
}

} // namespace ast
} // namespace soyac
