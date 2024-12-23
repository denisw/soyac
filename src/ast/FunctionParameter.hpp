/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FUNCTION_PARAMETER_HPP
#define _FUNCTION_PARAMETER_HPP

#include "DeclaredEntity.hpp"
#include "Link.hpp"
#include "Type.hpp"

namespace soyac {
namespace ast {

/**
 * Represents a function parameter.
 *
 * (See the Soya Language Reference,)
 */
class FunctionParameter : public DeclaredEntity {
public:
    /**
     * Creates a FunctionParameter.
     *
     * @param name  The parameter's name.
     * @param type  The parameter's type.
     */
    FunctionParameter(const Name& name, Type* type);

    /**
     * Visits the FunctionParameter.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the function parameter's type.
     *
     * @return  The parameter's type.
     */
    Type* type() const;

private:
    Link<Type> mType;
};

} // namespace ast
} // namespace soyac

#endif
