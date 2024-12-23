/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_VARIABLE_H_
#define SOYA_VARIABLE_H_

#include "DeclaredEntity.h"
#include "Expression.h"
#include "Type.h"

namespace soyac {
namespace ast {

/**
 * Represents a variable.
 *
 * (See the Soya Language Reference,)
 */
class Variable : public DeclaredEntity {
public:
    /**
     * Creates a Variable.
     *
     * @param name         The variable's name.
     * @param type         The variable's type.
     * @param initializer  The variable's initial value. May be null if
     *                     the variable is initialized to its default
     *                     value.
     */
    Variable(const Name& name, Type* type, Expression* initializer = nullptr);

    /**
     * Visits the Variable.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the variable's type. If the variable's type is TYPE_UNKNOWN
     * and has an initializer expression (see initializer()), that
     * expression's type is the variable's type and thus is returned
     * instead.
     *
     * @return  The variable's type.
     */
    Type* type() const;

    /**
     * Returns the expression whose value is initially assigned to the
     * variable. May be null if the variable's declaration does not include
     * an initializer expression.
     *
     * @return  The variable's initial value.
     */
    Expression* initializer() const;

private:
    Link<Type> mType;
    Link<Expression> mInitializer;
};

} // namespace ast
} // namespace soyac

#endif
