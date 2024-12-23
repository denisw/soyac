/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "FunctionParameter.h"
#include "Visitor.h"
#include <cassert>

namespace soyac {
namespace ast {

FunctionParameter::FunctionParameter(const Name& name, Type* type)
    : DeclaredEntity(name)
    , mType(type)
{
    assert(type != nullptr);
}

void* FunctionParameter::visit(Visitor* v)
{
    return v->visitFunctionParameter(this);
}

Type* FunctionParameter::type() const { return mType.target(); }

} // namespace ast
} // namespace soyac
