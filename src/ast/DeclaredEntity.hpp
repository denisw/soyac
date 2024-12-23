/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _DECLARED_ENTITY_HPP
#define _DECLARED_ENTITY_HPP

#include "NamedEntity.hpp"
#include <set>

namespace soyac {
namespace ast {

/**
 * Inherited by node classes representing declared entities.
 *
 * (See the Soya Language Reference,)
 */
class DeclaredEntity : public NamedEntity {
public:
    /**
     * Represents a declaration modifier.
     */
    enum Modifier { EXTERN, PRIVATE, PROTECTED, PUBLIC, STATIC };

    /**
     * DeclaredEntity constructor.
     *
     * @param name  The declared entity's name.
     */
    DeclaredEntity(const Name& name);

    /**
     * Returns @c true if the entity is declared with the passed modifier.
     *
     * @param m  The modifier to check for.
     * @return   @c true if the declaration has the modifier;
     *           @c false otherwise.
     */
    bool hasModifier(Modifier m);

    /**
     * Adds a modifier to the declared entity. If the passed modifier has
     * already been added to the entity, @c false is returned; otherwise,
     * the return value is @c true.
     *
     * @param m  The modifier to add.
     * @return   @c true if the modifier has successfully been added;
     *           @c false if it has already been added to the declared
     *           entity.
     */
    bool addModifier(Modifier m);

private:
    std::set<Modifier> mModifiers;
};

} // namespace ast
} // namespace soyac

#endif
