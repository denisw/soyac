/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "IntegerLiteral.h"
#include "IntegerType.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

IntegerLiteral::IntegerLiteral(const IntegerValue& value)
    : mValue(value)
{
}

void* IntegerLiteral::visit(Visitor* v) { return v->visitIntegerLiteral(this); }

Type* IntegerLiteral::type() const
{
    if (value() >= TYPE_INT->min() && value() <= TYPE_INT->max()) {
        return TYPE_INT;
    } else {
        return TYPE_LONG;
    }
}

const IntegerValue& IntegerLiteral::value() const { return mValue; }

} // namespace ast
} // namespace soyac
