/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "FloatingPointType.h"
#include "BooleanType.h"
#include "Function.h"
#include "FunctionParameter.h"
#include "IntegerType.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

FloatingPointType* FloatingPointType::sFloat = nullptr;
FloatingPointType* FloatingPointType::sDouble = nullptr;

FloatingPointType* FloatingPointType::getFloat()
{
    if (sFloat == nullptr) {
        sFloat = new FloatingPointType(false);
        sFloat->ref();
    }

    return sFloat;
}

FloatingPointType* FloatingPointType::getDouble()
{
    if (sDouble == nullptr) {
        sDouble = new FloatingPointType(true);
        sDouble->ref();
    }

    return sDouble;
}

FloatingPointType::FloatingPointType(bool isDouble)
    : BuiltInType(isDouble ? Name("double") : Name("float"))
{
    /**
     * Create and add the floating-point type's methods.
     */

    const std::string arithmetic[4] = { std::string("plus"),
        std::string("minus"), std::string("mul"), std::string("div") };

    const std::string relational[3] = { std::string("equals"),
        std::string("lessThan"), std::string("greaterThan") };

    for (int i = 0; i < 4; i++) {
        FunctionParameter* p = new FunctionParameter("x", this);

        Function* method = new Function(arithmetic[i], this, &p, &p + 1);
        addMember(method);

        if (!isDouble) {
            FunctionParameter* p = new FunctionParameter("x", TYPE_DOUBLE);

            Function* method
                = new Function(arithmetic[i], TYPE_DOUBLE, &p, &p + 1);
            addMember(method);
        }
    }

    for (int i = 0; i < 3; i++) {
        FunctionParameter* p = new FunctionParameter("x", this);

        Function* method = new Function(relational[i], TYPE_BOOL, &p, &p + 1);
        addMember(method);

        if (!isDouble) {
            FunctionParameter* p = new FunctionParameter("x", TYPE_DOUBLE);

            Function* method
                = new Function(relational[i], TYPE_BOOL, &p, &p + 1);
            addMember(method);
        }
    }
}

void* FloatingPointType::visit(Visitor* v)
{
    return v->visitFloatingPointType(this);
}

bool FloatingPointType::isConvertableTo(Type* other) const
{
    return (other == TYPE_FLOAT || other == TYPE_DOUBLE
        || dynamic_cast<IntegerType*>(other) != nullptr);
}

bool FloatingPointType::isImplicitlyConvertableTo(Type* other) const
{
    if (Type::isImplicitlyConvertableTo(other)) {
        return true;
    } else {
        return (this == TYPE_FLOAT && other == TYPE_DOUBLE);
    }
}

} // namespace ast
} // namespace soyac
