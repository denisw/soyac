/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedDefaultConstructorInitializer.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast {

UnresolvedDefaultConstructorInitializer::
    UnresolvedDefaultConstructorInitializer()
    : ConstructorInitializer((Expression**)NULL, (Expression**)NULL)
{
}

void* UnresolvedDefaultConstructorInitializer::visit(Visitor* v)
{
    return v->visitUnresolvedDefaultConstructorInitializer(this);
}

} // namespace ast
} // namespace soyac
