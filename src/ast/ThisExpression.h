/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_THIS_EXPRESSION_H_
#define SOYA_THIS_EXPRESSION_H_

#include "Expression.h"
#include "Type.h"

namespace soyac {
namespace ast {

/**
 * Represents a "this" expression.
 */
class ThisExpression : public Expression {
public:
    /**
     * Creates a ThisExpression.
     *
     * @param type  The "this" object's type.
     */
    ThisExpression(Type* type);

    /**
     * Visits the ThisExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

private:
    Link<Type> mType;
};

} // namespace ast
} // namespace soyac

#endif
