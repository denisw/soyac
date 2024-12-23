/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_PROPERTY_GET_ACCESSOR_H_
#define SOYA_PROPERTY_GET_ACCESSOR_H_

#include "Function.h"

namespace soyac {
namespace ast {

/**
 * Represents a property get accessor.
 */
class PropertyGetAccessor : public Function {
public:
    /**
     * Initializes a PropertyGetAccessor.
     *
     * @param body  The accessor's body, or null.
     */
    PropertyGetAccessor(Block* body);

    /**
     * Visits the PropertyGetAccessor.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);
};

} // namespace ast
} // namespace soyac

#endif
