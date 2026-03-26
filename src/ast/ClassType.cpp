/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ClassType.h"
#include "UnknownType.h"
#include "Variable.h"
#include "Visitor.h"

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
    assert(baseClass != nullptr);
}

void* ClassType::visit(Visitor* v) { return v->visitClassType(this); }

bool ClassType::isImplicitlyConvertableTo(Type* other) const
{
    if (Type::isImplicitlyConvertableTo(other)) {
        return true;
    }

    ClassType* cls = const_cast<ClassType*>(this);

    while (cls != nullptr) {
        if (cls == other) {
            return true;
        } else {
            cls = dynamic_cast<ClassType*>(cls->baseClass());
        }
    }

    return false;
}

Type* ClassType::baseClass() const { return mBaseClass.target(); }

size_t ClassType::totalInstanceVariableCount() const
{
    size_t count = 0;

    for (auto it = body()->declarations_begin();
        it != body()->declarations_end(); it++) {
        if (dynamic_cast<Variable*>((*it)->declaredEntity())) {
            count++;
        }
    }

    if (auto base = dynamic_cast<ClassType*>(baseClass())) {
        count += base->totalInstanceVariableCount();
    }

    return count;
}

} // namespace ast
} // namespace soyac
