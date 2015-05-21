/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "UnknownFunctionType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


void*
UnknownFunctionType::visit(Visitor* v)
{
    return v->visitUnknownFunctionType(this);
}


Type*
UnknownFunctionType::returnType() const
{
    return mReturnType.target();
}


UnknownFunctionType::parameterTypes_iterator
UnknownFunctionType::parameterTypes_begin() const
{
    return mParameterTypes.begin();
}


UnknownFunctionType::parameterTypes_iterator
UnknownFunctionType::parameterTypes_end() const
{
    return mParameterTypes.end();
}

}}
