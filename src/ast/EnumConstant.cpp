/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "EnumConstant.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast {

EnumConstant::EnumConstant(const Name& name, const IntegerValue& value)
    : DeclaredEntity(name)
    , mValue(value)
{
}

void* EnumConstant::visit(Visitor* v) { return v->visitEnumConstant(this); }

const IntegerValue& EnumConstant::value() const { return mValue; }

} // namespace ast
} // namespace soyac
