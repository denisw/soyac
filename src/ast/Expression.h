/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_EXPRESSION_H_
#define SOYA_EXPRESSION_H_

#include "Link.h"
#include "Node.h"

namespace soyac {
namespace ast {

class Type;

/**
 * Represents an expression.
 *
 * (See the Soya Language Reference,)
 */
class Expression : public Node {
public:
    /**
     * Expression constructor.
     */
    Expression();

    /**
     * Returns the expression's type. As the way an expression's type is
     * determined depends on the type of expression, this method is declared
     * to be purely virtual and must be implemented for each child class
     * independently.
     *
     * @return  The expresion's type.
     */
    virtual Type* type() const = 0;
};

} // namespace ast
} // namespace soyac

#endif
