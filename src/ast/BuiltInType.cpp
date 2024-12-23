/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "BuiltInType.hpp"

namespace soyac {
namespace ast {

BuiltInType::BuiltInType(const Name& name)
    : Type(name)
{
}

BuiltInType::members_iterator BuiltInType::members_begin() const
{
    return mMembers.begin();
}

BuiltInType::members_iterator BuiltInType::members_end() const
{
    return mMembers.end();
}

void BuiltInType::addMember(DeclaredEntity* member)
{
    mMembers.push_back(member);
    addChild(member);
}

} // namespace ast
} // namespace soyac
