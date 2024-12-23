/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "FloatingPointLiteral.hpp"
#include "FloatingPointType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast {

FloatingPointLiteral::FloatingPointLiteral(const FloatingPointValue& value)
    : mValue(value)
{
}

void* FloatingPointLiteral::visit(Visitor* v)
{
    return v->visitFloatingPointLiteral(this);
}

Type* FloatingPointLiteral::type() const
{
    if (value().isSinglePrecision()) {
        return TYPE_FLOAT;
    } else {
        return TYPE_DOUBLE;
    }
}

const FloatingPointValue& FloatingPointLiteral::value() const { return mValue; }

} // namespace ast
} // namespace soyac
