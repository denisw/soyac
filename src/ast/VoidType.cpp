/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "VoidType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{

VoidType* VoidType::sInstance = NULL;


VoidType::VoidType()
    : BuiltInType("void")
{
}


VoidType*
VoidType::get()
{
    if (sInstance == NULL)
    {
        sInstance = new VoidType;
        sInstance->ref();
    }

    return sInstance;
}


void*
VoidType::visit(Visitor* v)
{
    return v->visitVoidType(this);
}


}}
