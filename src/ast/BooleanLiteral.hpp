/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _BOOLEAN_LITERAL_HPP
#define _BOOLEAN_LITERAL_HPP

#include "Expression.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents a boolean literal.
 *
 * (See the Soya Language Reference,)
 */
class BooleanLiteral : public Expression
{
public:
    /**
     * Creates an BooleanLiteral.
     *
     * @param value  The represented boolean value.
     */
    BooleanLiteral(bool value);

    /**
     * Visits the BooleanLiteral.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the value represented by the boolean literal.
     *
     * @returns  The literal's value.
     */
     bool value() const;

private:
    bool mValue;
};

}}

#endif
