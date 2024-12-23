/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "EntityImport.h"
#include "Visitor.h"
#include <cassert>

namespace soyac {
namespace ast {

EntityImport::EntityImport(DeclaredEntity* imported)
    : mImported(imported)
{
    assert(imported != nullptr);
}

void* EntityImport::visit(Visitor* v) { return v->visitEntityImport(this); }

DeclaredEntity* EntityImport::imported() const { return mImported.target(); }

} // namespace ast
} // namespace soyac
