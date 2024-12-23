/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "NamedEntity.h"
#include <cassert>

namespace soyac {
namespace ast {

NamedEntity::NamedEntity(const Name& name)
    : mName(name)
    , mParent(nullptr)
{
    assert(name.isSimple());
}

const Name& NamedEntity::name() const { return mName; }

Name NamedEntity::qualifiedName() const
{
    if (mParent == nullptr) {
        return name();
    } else {
        return mParent->qualifiedName() + name();
    }
}

NamedEntity* NamedEntity::parent() const { return mParent; }

std::string NamedEntity::str() const { return qualifiedName().str(); }

std::ostream& operator<<(std::ostream& s, const NamedEntity* entity)
{
    s << entity->str();
    return s;
}

void NamedEntity::addChild(NamedEntity* child)
{
    assert(child->mParent == this || child->mParent == nullptr);
    child->mParent = this;
}

void NamedEntity::removeChild(NamedEntity* child)
{
    assert(child->mParent == this || child->mParent == nullptr);
    child->mParent = nullptr;
}

} // namespace ast
} // namespace soyac
