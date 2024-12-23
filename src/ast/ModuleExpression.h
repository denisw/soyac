/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_MODULE_EXPRESSION_H_
#define SOYA_MODULE_EXPRESSION_H_

#include "Expression.h"
#include "Module.h"

namespace soyac {
namespace ast {

/**
 * Represents an expression that denotes a module.
 */
class ModuleExpression : public Expression {
public:
    /**
     * Creates a ModuleExpression.
     *
     * @param target  The denoted module.
     */
    ModuleExpression(Module* target);

    /**
     * Visits the ModuleExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the module denoted by the expression.
     *
     * @returns  The denoted module.
     */
    Module* target() const;

private:
    Link<Module> mTarget;
};

} // namespace ast
} // namespace soyac

#endif
