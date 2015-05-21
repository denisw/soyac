/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedEntityImport.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


UnresolvedEntityImport::UnresolvedEntityImport(const Name& importedName)
    : mImportedName(importedName)
{
}


void*
UnresolvedEntityImport::visit(Visitor* v)
{
    return v->visitUnresolvedEntityImport(this);
}


const Name&
UnresolvedEntityImport::importedName() const
{
    return mImportedName;
}


}}
