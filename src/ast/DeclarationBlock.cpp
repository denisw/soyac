/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "DeclarationBlock.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

DeclarationBlock::DeclarationBlock() { }

void* DeclarationBlock::visit(Visitor* v)
{
    return v->visitDeclarationBlock(this);
}

DeclarationBlock::declarations_iterator
DeclarationBlock::declarations_begin() const
{
    return mDeclarations.begin();
}

DeclarationBlock::declarations_iterator
DeclarationBlock::declarations_end() const
{
    return mDeclarations.end();
}

void DeclarationBlock::addDeclaration(DeclarationStatement* decl)
{
    mDeclarations.push_back(decl);
}

boost::signals2::signal<void(DeclarationStatement*, DeclarationStatement*)>&
DeclarationBlock::declarationListChanged()
{
    return mDeclarations.changed();
}

} // namespace ast
} // namespace soyac
