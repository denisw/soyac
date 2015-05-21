/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "DeclarationStatement.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


DeclarationStatement::DeclarationStatement(DeclaredEntity* decl)
    : mDeclaredEntity(decl)
{
    assert (decl != NULL);
}


void*
DeclarationStatement::visit(Visitor* v)
{
    return v->visitDeclarationStatement(this);
}


DeclaredEntity*
DeclarationStatement::declaredEntity() const
{
    return mDeclaredEntity.target();
}


}}
