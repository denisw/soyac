/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "BooleanType.h"
#include "Function.h"
#include "FunctionParameter.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

BooleanType* BooleanType::sInstance = nullptr;

BooleanType::BooleanType()
    : BuiltInType("bool")
{
    FunctionParameter* p = new FunctionParameter(Name("x"), this);

    Function* equals = new Function(Name("equals"), this, &p, &p + 1);
    addMember(equals);
}

BooleanType* BooleanType::get()
{
    if (sInstance == nullptr) {
        sInstance = new BooleanType;
        sInstance->ref();
    }

    return sInstance;
}

void* BooleanType::visit(Visitor* v) { return v->visitBooleanType(this); }

} // namespace ast
} // namespace soyac
