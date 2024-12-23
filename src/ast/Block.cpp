/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "Block.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

Block::Block() { }

void* Block::visit(Visitor* v) { return v->visitBlock(this); }

Block::statements_iterator Block::statements_begin() const
{
    return mStatements.begin();
}

Block::statements_iterator Block::statements_end() const
{
    return mStatements.end();
}

void Block::addStatement(Statement* s) { mStatements.push_back(s); }

boost::signals2::signal<void(Statement*, Statement*)>&
Block::statementListChanged()
{
    return mStatements.changed();
}

} // namespace ast
} // namespace soyac
