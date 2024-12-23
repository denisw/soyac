/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_CHARACTER_LITERAL_H_
#define SOYA_CHARACTER_LITERAL_H_

#include "Expression.h"
#include <stdint.h>

namespace soyac {
namespace ast {

/**
 * Represents a floating-point literal.
 *
 * (See the Soya Language Reference,)
 */
class CharacterLiteral : public Expression {
public:
    /**
     * Creates an CharacterLiteral.
     *
     * @param value  The represented character value.
     */
    CharacterLiteral(uint32_t value);

    /**
     * Visits the CharacterLiteral.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the value represented by the character literal.
     *
     * @returns  The literal's value.
     */
    uint32_t value() const;

private:
    uint32_t mValue;
};

} // namespace ast
} // namespace soyac

#endif
