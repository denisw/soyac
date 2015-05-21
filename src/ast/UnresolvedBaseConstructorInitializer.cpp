/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedBaseConstructorInitializer.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{

UnresolvedBaseConstructorInitializer::UnresolvedBaseConstructorInitializer()
    : ConstructorInitializer((Expression**) NULL, (Expression**) NULL)
{
}


void*
UnresolvedBaseConstructorInitializer::visit(Visitor* v)
{
    return v->visitUnresolvedBaseConstructorInitializer(this);
}


}}
