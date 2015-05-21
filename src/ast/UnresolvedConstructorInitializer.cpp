/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedConstructorInitializer.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


UnresolvedConstructorInitializer::UnresolvedConstructorInitializer()
    : ConstructorInitializer((Expression**) NULL, (Expression**) NULL)
{
}


void*
UnresolvedConstructorInitializer::visit(Visitor* v)
{
    return v->visitUnresolvedConstructorInitializer(this);
}


}}
