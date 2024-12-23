/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_FLOATING_POINT_LITERAL_H_
#define SOYA_FLOATING_POINT_LITERAL_H_

#include "Expression.h"
#include "FloatingPointValue.h"

namespace soyac {
namespace ast {

/**
 * Represents a floating-point literal.
 *
 * (See the Soya Language Reference,)
 */
class FloatingPointLiteral : public Expression {
public:
    /**
     * Creates an FloatingPointLiteral.
     *
     * @param value  The represented floating-point value.
     */
    FloatingPointLiteral(const FloatingPointValue& value);

    /**
     * Visits the FloatingPointLiteral.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the value represented by the floating-point literal.
     *
     * @returns  The literal's value.
     */
    const FloatingPointValue& value() const;

private:
    FloatingPointValue mValue;
};

} // namespace ast
} // namespace soyac

#endif
