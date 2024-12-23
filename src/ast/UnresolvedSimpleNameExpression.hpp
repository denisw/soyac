/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_SIMPLE_NAME_EXPRESSION_HPP
#define _UNRESOLVED_SIMPLE_NAME_EXPRESSION_HPP

#include "Expression.hpp"
#include "NamedEntity.hpp"

namespace soyac {
namespace ast {

/**
 * Placeholder for a simple name expression denoting a to-be-determined
 * declared entity.
 *
 * UnresolvedSimpleNameExpression instances are replaced with semantically
 * meaningful equivalents by the ContextualAnalyzer.
 *
 * (See the Soya Language Reference,)
 */
class UnresolvedSimpleNameExpression : public Expression {
public:
    /**
     * Creates a UnresolvedSimpleNameExpression.
     *
     * @param name  The denoted entity's name.
     *              Must be a simple name.
     */
    UnresolvedSimpleNameExpression(const Name& name);

    /**
     * Visits the UnresolvedSimpleNameExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the name of the entity referred to by the name expression.
     *
     * @returns  The denoted entity's name.
     */
    const Name& name() const;

private:
    Name mName;
};

} // namespace ast
} // namespace soyac

#endif
