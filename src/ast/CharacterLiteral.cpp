/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "CharacterLiteral.hpp"
#include "CharacterType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast {

CharacterLiteral::CharacterLiteral(uint32_t value)
    : mValue(value)
{
}

void* CharacterLiteral::visit(Visitor* v)
{
    return v->visitCharacterLiteral(this);
}

Type* CharacterLiteral::type() const { return TYPE_CHAR; }

uint32_t CharacterLiteral::value() const { return mValue; }

} // namespace ast
} // namespace soyac
