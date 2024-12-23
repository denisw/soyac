/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_ENUM_VALUE_EXPRESSION_H_
#define SOYA_ENUM_VALUE_EXPRESSION_H_

#include "EnumType.h"
#include "Expression.h"

namespace soyac {
namespace ast {

/**
 * Represents an expression that denotes an enum value.
 */
class EnumValueExpression : public Expression {
public:
    /**
     * Creates an EnumValueExpression.
     *
     * @param type   The enum type of which a value is denoted.
     * @param value  The denoted value.
     */
    EnumValueExpression(EnumType* type, EnumConstant* value);

    /**
     * Visits the EnumValueExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the enum value denoted by the EnumValueExpression.
     *
     * @returns  The denoted value.
     */
    EnumConstant* target() const;

private:
    Link<EnumType> mType;
    Link<EnumConstant> mTarget;
};

} // namespace ast
} // namespace soyac

#endif
