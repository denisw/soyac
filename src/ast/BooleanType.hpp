/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _BOOLEAN_TYPE_HPP
#define _BOOLEAN_TYPE_HPP

#include "BuiltInType.hpp"

/**
 * The "bool" type. This is a shortcut to BooleanType::get().
 */
#define TYPE_BOOL soyac::ast::BooleanType::get()

namespace soyac {
namespace ast {

/**
 * Represents the "bool" type.
 *
 * (See the Soya Language Reference,)
 */
class BooleanType : public BuiltInType {
public:
    /**
     * Returns a singleton BooleanType instance representing the "bool"
     * type.
     *
     * @return The "bool" type.
     */
    static BooleanType* get();

    /**
     * Visits the BooleanType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

private:
    static BooleanType* sInstance;

    /**
     * BooleanType constructor.
     */
    BooleanType();
};

} // namespace ast
} // namespace soyac

#endif
