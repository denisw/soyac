/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ArrayType.hpp"
#include "Function.hpp"
#include "FunctionParameter.hpp"
#include "IntegerType.hpp"
#include "Property.hpp"
#include "PropertyGetAccessor.hpp"
#include "Visitor.hpp"
#include "VoidType.hpp"
#include <cassert>

namespace soyac {
namespace ast {

std::map<Type*, ArrayType*> ArrayType::mInstances;

ArrayType* ArrayType::get(Type* elementType)
{
    ArrayType* ret = mInstances[elementType];

    if (ret == NULL) {
        ret = new ArrayType(elementType);
        ret->ref();
        mInstances[elementType] = ret;
    }

    return ret;
}

ArrayType::ArrayType(Type* elementType)
    : Type(elementType->name().str() + "[]")
    , mElementType(elementType)
{
    assert(elementType != NULL);

    FunctionParameter* params[2] = { new FunctionParameter("index", TYPE_LONG),
        new FunctionParameter("x", elementType) };

    mGetElementMethod
        = new Function("getElement", elementType, params, params + 1);
    mGetElementMethod->ref();

    mSetElementMethod
        = new Function("setElement", TYPE_VOID, params, params + 2);
    mSetElementMethod->ref();

    mLengthProperty = new Property(
        "length", TYPE_LONG, new PropertyGetAccessor(NULL), NULL);
    mLengthProperty->ref();
}

void* ArrayType::visit(Visitor* v) { return v->visitArrayType(this); }

Type* ArrayType::elementType() const { return mElementType; }

Function* ArrayType::getElementMethod() const { return mGetElementMethod; }

Function* ArrayType::setElementMethod() const { return mSetElementMethod; }

Property* ArrayType::lengthProperty() const { return mLengthProperty; }

} // namespace ast
} // namespace soyac
