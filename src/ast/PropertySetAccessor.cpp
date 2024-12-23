/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "PropertySetAccessor.hpp"
#include "FunctionParameter.hpp"
#include "Property.hpp"
#include "UnknownType.hpp"
#include "Visitor.hpp"
#include "VoidType.hpp"
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
