/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "StructType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


StructType::StructType(const Name& name, DeclarationBlock* body)
    : UserDefinedType(name, body)
{
}


void*
StructType::visit(Visitor* v)
{
    return v->visitStructType(this);
}


}}
