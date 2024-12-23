/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_CHARACTER_TYPE_H_
#define SOYA_CHARACTER_TYPE_H_

#include "BuiltInType.h"

/**
 * The "char" type. This is a shortcut to CharacterType::get().
 */
#define TYPE_CHAR soyac::ast::CharacterType::get()

namespace soyac {
namespace ast {

/**
 * Represents a boolean type.
 *
 * (See the Soya Language Reference,)
 */
class CharacterType : public BuiltInType {
public:
    /**
     * Returns a singleton CharacterType instance representing the
     * "char" type.
     *
     * @return The "char" type.
     */
    static CharacterType* get();

    /**
     * Visits the CharacterType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Override of Type::isConvertableTo().
     */
    virtual bool isConvertableTo(Type* other) const;

private:
    static CharacterType* sInstance;

    /**
     * CharacterType constructor.
     */
    CharacterType();
};

} // namespace ast
} // namespace soyac

#endif
