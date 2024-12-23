/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "PropertySetAccessor.h"
#include "FunctionParameter.h"
#include "Property.h"
#include "UnknownType.h"
#include "Visitor.h"
#include "VoidType.h"
#include <cassert>

namespace soyac {
namespace ast {

PropertySetAccessor::PropertySetAccessor(Block* body)
    : Function("set", TYPE_VOID,
          new FunctionParameter("value", new UnknownType("")), body)
{
}

void* PropertySetAccessor::visit(Visitor* v)
{
    return v->visitPropertySetAccessor(this);
}

FunctionParameter* PropertySetAccessor::valueParameter()
{
    return *parameters_begin();
}

} // namespace ast
} // namespace soyac
