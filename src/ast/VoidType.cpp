/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "VoidType.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

VoidType* VoidType::sInstance = nullptr;

VoidType::VoidType()
    : BuiltInType("void")
{
}

VoidType* VoidType::get()
{
    if (sInstance == nullptr) {
        sInstance = new VoidType;
        sInstance->ref();
    }

    return sInstance;
}

void* VoidType::visit(Visitor* v) { return v->visitVoidType(this); }

} // namespace ast
} // namespace soyac
