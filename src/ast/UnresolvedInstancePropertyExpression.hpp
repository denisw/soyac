/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_INSTANCE_PROPERTY_EXPRESSION_HPP
#define _UNRESOLVED_INSTANCE_PROPERTY_EXPRESSION_HPP

#include "Expression.hpp"
#include "Property.hpp"

namespace soyac {
namespace ast {

/**
 * Represents an expression denoting a property, which needs to
 * be resolved to either an InstancPropertyGetExpression or
 * InstancPropertySetExpression.
 *
 * UnresolvedInstancePropertyExpression instances are replaced with
 * semantically unambiguous equivalents by the BasicAnalyzer.
 */
class UnresolvedInstancePropertyExpression : public Expression {
public:
    /**
     * Creates a UnresolvedInstancePropertyExpression.
     *
     * @param instance  The denoted instance.
     * @param target    The denoted property.
     */
    UnresolvedInstancePropertyExpression(
        Expression* instance, Property* target);

    /**
     * Visits the UnresolvedInstancePropertyExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the expression yielding the instance whose property is
     * denoted by the UnresolvedInstancePropertyExpression.
     *
     * @returns  The denoted instance.
     */
    Expression* instance() const;

    /**
     * Returns the property denoted by the
     * UnresolvedInstancePropertyExpression.
     *
     * @return the denoted property.
     */
    Property* target() const;

private:
    Link<Expression> mInstance;
    Link<Property> mTarget;
};

} // namespace ast
} // namespace soyac

#endif
