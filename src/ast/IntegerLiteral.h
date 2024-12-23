/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_INTEGER_LITERAL_H_
#define SOYA_INTEGER_LITERAL_H_

#include "Expression.h"
#include "IntegerValue.h"
#include "Visitor.h"
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
