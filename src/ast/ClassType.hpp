/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _CLASS_TYPE_HPP
#define _CLASS_TYPE_HPP

#include "UserDefinedType.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents a class type.
 */
class ClassType : public UserDefinedType
{
public:
    /**
     * Creates a ClassType.
     *
     * @param name  The class type's name.
     * @param body  The type declaration's body.
     */
    ClassType(const Name& name, DeclarationBlock* body);

    /**
     * Creates a ClassType with a specific base class.
     *
     * @param name       The class type's name.
     * @param baseClass  The class type's base class.
     * @param body       The type declaration's body.
     */
    ClassType(const Name& name,
              Type* baseClass,
              DeclarationBlock* body);

    /**
     * Visits the ClassType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Override of Type::isImplicitlyConvertableTo().
     */
    virtual bool isImplicitlyConvertableTo(Type* other) const;

    /**
     * Returns the class type's base class.
     *
     * @return  The base class.
     */
    Type* baseClass() const;

private:
    Link<Type> mBaseClass;
};

}}

#endif
