/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _PROPERTY_HPP
#define _PROPERTY_HPP

#include "DeclaredEntity.hpp"
#include "PropertyGetAccessor.hpp"
#include "PropertySetAccessor.hpp"
#include "Type.hpp"

namespace soyac {
namespace ast {

/**
 * Represents a property.
 */
class Property : public DeclaredEntity {
public:
    /**
     * Creates a Property.
     *
     * @param name         The property's name.
     * @param getAccessor  The property's get accessor, or null.
     * @param setAccessor  The property's set accessor, or null.
     */
    Property(const Name& name, Type* type, PropertyGetAccessor* getAccessor,
        PropertySetAccessor* setAccessor);

    /**
     * Visits the Property.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the property's type.
     *
     * @return  The property's type.
     */
    Type* type() const;

    /**
     * Returns the property's get accessor if the property has one, or
     * null otherwise.
     *
     * @return  The property's get accessor, or null.
     */
    PropertyGetAccessor* getAccessor() const;

    /**
     * Returns the property's set accessor if the property has one, or
     * null otherwise.
     *
     * @return  The property's set accessor, or null.
     */
    PropertySetAccessor* setAccessor() const;

private:
    Link<Type> mType;
    Link<PropertyGetAccessor> mGetAccessor;
    Link<PropertySetAccessor> mSetAccessor;

    /**
     * Called whenever the property's get accessor is exchanged.
     *
     * @see Link::targetReplaced()
     */
    void onGetAccessorChanged(
        PropertyGetAccessor* oldAcc, PropertyGetAccessor* newAcc);

    /**
     * Called whenever the property's set accessor is exchanged.
     *
     * @see Link::targetReplaced()
     */
    void onSetAccessorChanged(
        PropertySetAccessor* oldAcc, PropertySetAccessor* newAcc);
};

} // namespace ast
} // namespace soyac

#endif
