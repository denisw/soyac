/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_VOID_TYPE_H_
#define SOYA_VOID_TYPE_H_

#include "BuiltInType.h"

/**
 * The "void" type. This is a shortcut to VoidType::get().
 */
#define TYPE_VOID soyac::ast::VoidType::get()

namespace soyac {
namespace ast {

/**
 * Represents the "void" type.
 *
 * (See the Soya Language Reference,)
 */
class VoidType : public BuiltInType {
public:
    /**
     * Returns a singleton VoidType instance representing the "void" type.
     * Usually, the TYPE_VOID macro is used instead of a direct call to
     * get().
     *
     * @return The "bool" type.
     */
    static VoidType* get();

    /**
     * Visits the VoidType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

private:
    static VoidType* sInstance;

    /**
     * VoidType constructor.
     */
    VoidType();
};

} // namespace ast
} // namespace soyac

#endif
