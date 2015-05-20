/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _BLOCK_HPP
#define _BLOCK_HPP

#include "NodeList.hpp"
#include "Statement.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents a block.
 *
 * (See the Soya Language Reference,)
 */
class Block : public Statement
{
public:
    /**
     * Iterates over the block's statements (constant).
     */
    typedef NodeList<Statement>::const_iterator statements_iterator;

    /**
     * Creates a Block.
     *
     * @param statements_begin  Start iterator for the block's statements.
     * @param statements_begin  End iterator for the block's statements.
     */
    template <class InputIterator>
    Block(InputIterator statements_begin,
          InputIterator statements_end)
    {
        for (InputIterator it = statements_begin; it != statements_end; it++)
            mStatements.push_back(*it);
    }

    /**
     * Creates an empty Block.
     */
    Block();

    /**
     * Visits the Block.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns a constant iterator pointing to the first statement
     * in the block.
     *
     * @return  The start iterator for the block's statements.
     */
    statements_iterator statements_begin() const;

    /**
     * Returns an iterator pointing past the last statement
     * in the block.
     *
     * @return  The end iterator for the block's statements.
     */
    statements_iterator statements_end() const;

    /**
     * Adds a statement to the end of the Block's statement list.
     *
     * @param s  The statement to add.
     */
    void addStatement(Statement* s);

    /**
     * Returns the "statementListChanged" signal, which is emitted
     * whenever a statement has been added to, removed from or replaced
     * within the block's statement list.
     *
     * @see  NodeList::elementChanged()
     */
    sigc::signal<void, Statement*, Statement*>& statementListChanged();

private:
    NodeList<Statement> mStatements;
};

}}

#endif
