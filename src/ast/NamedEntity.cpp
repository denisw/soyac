/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "NamedEntity.hpp"

namespace soyac {
namespace ast
{


NamedEntity::NamedEntity(const Name& name)
    : mName(name),
      mParent(NULL)
{
    assert (name.isSimple());
}


const Name&
NamedEntity::name() const
{
    return mName;
}


Name
NamedEntity::qualifiedName() const
{
    if (mParent == NULL)
        return name();
    else
        return mParent->qualifiedName() + name();
}


NamedEntity*
NamedEntity::parent() const
{
    return mParent;
}


std::string
NamedEntity::str() const
{
    return qualifiedName().str();
}


std::ostream&
operator<<(std::ostream& s, const NamedEntity* entity)
{
    s << entity->str();
    return s;
}


void
NamedEntity::addChild(NamedEntity* child)
{
    assert (child->mParent == this || child->mParent == NULL);
    child->mParent = this;
}


void
NamedEntity::removeChild(NamedEntity* child)
{
    assert (child->mParent == this || child->mParent == NULL);
    child->mParent = NULL;
}


}}
