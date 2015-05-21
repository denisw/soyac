/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "DeclaredEntity.hpp"

namespace soyac {
namespace ast
{


DeclaredEntity::DeclaredEntity(const Name& name)
    : NamedEntity(name)
{
    assert (name.isSimple());
}


bool
DeclaredEntity::hasModifier(DeclaredEntity::Modifier m)
{
    return mModifiers.find(m) != mModifiers.end();
}


bool
DeclaredEntity::addModifier(DeclaredEntity::Modifier m)
{
    return mModifiers.insert(m).second;
}


}}
