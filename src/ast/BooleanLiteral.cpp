/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "BooleanLiteral.h"
#include "BooleanType.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

BooleanLiteral::BooleanLiteral(bool value)
    : mValue(value)
{
}

void* BooleanLiteral::visit(Visitor* v) { return v->visitBooleanLiteral(this); }

Type* BooleanLiteral::type() const { return TYPE_BOOL; }

bool BooleanLiteral::value() const { return mValue; }

} // namespace ast
} // namespace soyac
