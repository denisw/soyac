/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "EnumType.hpp"
#include "BooleanType.hpp"
#include "Function.hpp"
#include "FunctionParameter.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


void
EnumType::initEqualsMethod()
{
    FunctionParameter* param = new FunctionParameter("x", this);

    mEqualsMethod = new Function("equals", TYPE_BOOL, &param, &param + 1);
    mEqualsMethod->ref();
}


void*
EnumType::visit(Visitor* v)
{
    return v->visitEnumType(this);
}


bool
EnumType::isConvertableTo(Type* other) const
{
    if (isImplicitlyConvertableTo(other))
        return true;
    else
        return underlyingType()->isConvertableTo(other);
}


IntegerType*
EnumType::underlyingType() const
{
    return mUnderlyingType.target();
}


EnumType::values_iterator
EnumType::values_begin() const
{
    return mValues.begin();
}


EnumType::values_iterator
EnumType::values_end() const
{
    return mValues.end();
}


Function*
EnumType::equalsMethod() const
{
    return mEqualsMethod;
}


void
EnumType::onValuesChanged(EnumConstant* oldVal, EnumConstant* newVal)
{
    if (oldVal != NULL)
        removeChild(oldVal);

    if (newVal != NULL)
        addChild(newVal);
}

}}
