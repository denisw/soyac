/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _INTEGER_LITERAL_HPP
#define _INTEGER_LITERAL_HPP

#include "Expression.hpp"
#include "IntegerValue.hpp"
#include "Visitor.hpp"
#include <stdint.h>

namespace soyac {
namespace ast {

/**
 * Represents an integer literal.
 *
 * (See the Soya Language Reference,)
 */
class IntegerLiteral : public Expression {
public:
    /**
     * Creates an IntegerLiteral.
     *
     * @param value  The represented integer value.
     */
    IntegerLiteral(const IntegerValue& value);

    /**
     * Visits the IntegerLiteral.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the value represented by the integer literal.
     *
     * @returns  The literal's value.
     */
    const IntegerValue& value() const;

private:
    IntegerValue mValue;
};

} // namespace ast
} // namespace soyac

#endif
