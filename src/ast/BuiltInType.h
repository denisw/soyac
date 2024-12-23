/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_BUILT_IN_TYPE_H_
#define SOYA_BUILT_IN_TYPE_H_

#include "NodeList.h"
#include "Type.h"

namespace soyac {
namespace ast {

/**
 * Inherited by node classes representing types which are built into
 * the Soya language.
 */
class BuiltInType : public Type {
public:
    /**
     * Iterates over the built-in type's members.
     */
    typedef NodeList<DeclaredEntity>::const_iterator members_iterator;

    /**
     * BuiltInType constructor.
     *
     * @param name  The type's name.
     */
    BuiltInType(const Name& name);

    /**
     * Returns a constant iterator pointing to the first member of the
     * built-in type.
     *
     * @return  The start iterator for the type's members.
     */
    members_iterator members_begin() const;

    /**
     * Returns a constant iterator pointing past the last member of the
     * built-in type.
     *
     * @return  The end iterator for the type's members.
     */
    members_iterator members_end() const;

protected:
    /**
     * Adds a member to the built-in type.
     *
     * @param member  The member to add.
     */
    void addMember(DeclaredEntity* member);

private:
    NodeList<DeclaredEntity> mMembers;
};

} // namespace ast
} // namespace soyac

#endif
