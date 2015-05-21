/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "FunctionGroupImport.hpp"
#include "UnknownType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


void*
FunctionGroupImport::visit(Visitor* v)
{
    return v->visitFunctionGroupImport(this);
}


FunctionGroupImport::overloads_iterator
FunctionGroupImport::overloads_begin() const
{
    return mOverloads.begin();
}


FunctionGroupImport::overloads_iterator
FunctionGroupImport::overloads_end() const
{
    return mOverloads.end();
}


}}
