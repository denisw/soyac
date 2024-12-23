/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _STRUCT_TYPE_HPP
#define _STRUCT_TYPE_HPP

#include "UserDefinedType.hpp"

namespace soyac {
namespace ast {

/**
 * Represents a structure type.
 */
class StructType : public UserDefinedType {
public:
    /**
     * Creates a StructType.
     *
     * @param name  The struct type's name.
     * @param body  The type declaration's body.
     */
    StructType(const Name& name, DeclarationBlock* body);

    /**
     * Visits the StructType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);
};

} // namespace ast
} // namespace soyac

#endif
