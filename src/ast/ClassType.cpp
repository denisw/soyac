/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ClassType.hpp"
#include "UnknownType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast {

ClassType::ClassType(const Name& name, DeclarationBlock* body)
    : UserDefinedType(name, body)
    , mBaseClass(new UnknownType(Name("__core__") + Name("Object")))
{
}

ClassType::ClassType(const Name& name, Type* baseClass, DeclarationBlock* body)
    : UserDefinedType(name, body)
    , mBaseClass(baseClass)
{
    assert(baseClass != NULL);
}

void* ClassType::visit(Visitor* v) { return v->visitClassType(this); }

bool ClassType::isImplicitlyConvertableTo(Type* other) const
{
    if (Type::isImplicitlyConvertableTo(other)) {
        return true;
    }

    ClassType* cls = const_cast<ClassType*>(this);

    while (cls != NULL) {
        if (cls == other) {
            return true;
        } else {
            cls = dynamic_cast<ClassType*>(cls->baseClass());
        }
    }

    return false;
}

Type* ClassType::baseClass() const { return mBaseClass.target(); }

} // namespace ast
} // namespace soyac
