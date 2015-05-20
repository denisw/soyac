/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _PROPERTY_SET_ACCESSOR_HPP
#define _PROPERTY_SET_ACCESSOR_HPP

#include "Function.hpp"

namespace soyac {
namespace ast
{

class FunctionParameter;

/**
 * Represents a property set accessor.
 */
class PropertySetAccessor : public Function
{
public:
    /**
     * Initializes a PropertySetAccessor.
     *
     * @param body  The accessor's body, or null.
     */
    PropertySetAccessor(Block* body);

    /**
     * Visits the PropertySetAccessor.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the property set accessor's implicit "value" parameter.
     *
     * If the PropertySetAccessor has already been made part of a Property,
     * the returned FunctionParameter's type() method will correctly return
     * the enclosing property's type; otherwise, the parameter's type is set
     * to an UnknownType which is replaced (see Node::replaceWith()) if the
     * accessor is added to a property.
     *
     * @return  The "value" parameter.
     */
    FunctionParameter* valueParameter();
};

}}

#endif
