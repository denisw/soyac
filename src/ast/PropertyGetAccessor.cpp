/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "PropertyGetAccessor.h"
#include "UnknownType.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

PropertyGetAccessor::PropertyGetAccessor(Block* body)
    : Function("get", new UnknownType(""), body)
{
}

void* PropertyGetAccessor::visit(Visitor* v)
{
    return v->visitPropertyGetAccessor(this);
}

} // namespace ast
} // namespace soyac
