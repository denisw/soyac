/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ObjectCreationExpression.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast {

void* ObjectCreationExpression::visit(Visitor* v)
{
    return v->visitObjectCreationExpression(this);
}

Type* ObjectCreationExpression::type() const { return mType.target(); }

Constructor* ObjectCreationExpression::constructor() const
{
    return mConstructor.target();
}

ObjectCreationExpression::arguments_iterator
ObjectCreationExpression::arguments_begin() const
{
    return mArguments.begin();
}

ObjectCreationExpression::arguments_iterator
ObjectCreationExpression::arguments_end() const
{
    return mArguments.end();
}

} // namespace ast
} // namespace soyac
