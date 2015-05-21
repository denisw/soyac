/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "UnknownArrayType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


UnknownArrayType::UnknownArrayType(UnknownType* elementType)
    : UnknownType(elementType->name().str() + "[]"),
      mElementType(elementType)
{
    assert (elementType != NULL);
}


void*
UnknownArrayType::visit(Visitor* v)
{
    return v->visitUnknownArrayType(this);
}


UnknownType*
UnknownArrayType::elementType() const
{
    return mElementType.target();
}


}}
