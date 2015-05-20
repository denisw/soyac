/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _NAMED_ENTITY_HPP
#define _NAMED_ENTITY_HPP

#include "Name.hpp"
#include "Node.hpp"

namespace soyac {
namespace ast
{

/*
 * Forward-declare UnknownType, it is a friend class of NamedEntity. See the
 * comment in the friend declaration.
 */
class UnknownType;

/**
 * Inherited by all Node classes representing named entities.
 *
 * (See the Soya Language Reference,)
 */
class NamedEntity : public Node
{
public:
    /**
     * NamedEntity constructor.
     *
     * @param name  The entity's name.
     */
    NamedEntity(const Name& name);

    /**
     * Returns the named entity's name.
     *
     * @return  The entity's name.
     */
    const Name& name() const;

    /**
     * Returns the entity's fully qualified name. If the entity cannot
     * be accessed with a qualified name (e.g. because it is local to a
     * function), the entity's simple name is returned. Whether the
     * returned name is a simple name can be checked via the Name::isSimpe()
     * method.
     *
     * @return  The fully qualified name, or the entity's simple name if
     *          the entity has no qualified name.
     */
    Name qualifiedName() const;

    /**
     * Returns the named entity that immediately encloses this entity.
     * If the entity has no parent entity (for instance, if it is a
     * module) or is not publicly accessible (for instance if it is local
     * to a function), null is returned.
     *
     * @return  The parent entity, or null.
     */
    NamedEntity* parent() const;

    /**
     * Returns a string that identifies the named entity. In the
     * default implementation, the entity's qualified name is
     * returned.
     *
     * This return value of str() is used to denote the entity in
     * error or warning messages.
     *
     * @return  A string identifying the entity.
     */
    virtual std::string str() const;

protected:
    /**
     * Makes the named entity the parent of the passed other entity.
     *
     * @param child  A new enclosed entity.
     */
    void addChild(NamedEntity* child);

    /**
     * Unsets the parent of a previously enclosed entity.
     *
     * @param child  The enclosed entity to remove.
     */
    void removeChild(NamedEntity* child);

private:
    Name mName;
    NamedEntity* mParent;

    /*
     * UnknownType is made a friend class of NamedEntity to allow UnknownType
     * instances with qualified names. See the comment in the UnkownType
     * constructor's implementation for more details.
     */
    friend class soyac::ast::UnknownType;
};

/**
 * Writes the return value of a NamedEntity's str() method
 * to an input stream.
 *
 * @param s     The stream to write to.
 * @param name  The NamedEntity whose str() return value should
 *              be written.
 */
std::ostream& operator<<(std::ostream& s, const NamedEntity* entity);

}}

#endif
