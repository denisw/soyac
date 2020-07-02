/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _DECLARATION_BLOCK_HPP
#define _DECLARATION_BLOCK_HPP

#include "DeclarationStatement.hpp"
#include "NodeList.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents a declaration block.
 *
 * (See the Soya Language Reference,)
 */
class DeclarationBlock : public Statement
{
public:
    /**
     * Iterates over the declarations in the declaration block (constant).
     */
    typedef NodeList<DeclarationStatement>::const_iterator declarations_iterator;

    /**
     * Creates a DeclarationBlock.
     *
     * @param declarations_start  Start iterator for the block's declarations.
     * @param declarations_end    End iterator for the block's declarations.
     */
    template <class InputIterator>
    DeclarationBlock(InputIterator declarations_begin,
                     InputIterator declarations_end)
    {
        for (InputIterator it = declarations_begin;
             it != declarations_end; it++)
        {
            mDeclarations.push_back(*it);
        }
    }

    /**
     * Creates an empty DeclarationBlock.
     */
    DeclarationBlock();

    /**
     * Visits the DeclarationBlock.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns a constant iterator pointing to the first declaration
     * statement in the declaration block.
     *
     * @return  The start iterator for the block's declarations.
     */
    declarations_iterator declarations_begin() const;

    /**
     * Returns a constant iterator pointing past the last declaration
     * statement in the declaration block.
     *
     * @return  The end iterator for the block's declarations.
     */
    declarations_iterator declarations_end() const;

    /**
     * Adds a declaration statement to the end of the block's
     * declaration list.
     *
     * @param decl  The declaration statement to add.
     */
    void addDeclaration(DeclarationStatement* decl);

    /**
     * Returns the "declarationListChanged" signal, which is emitted
     * whenever a declaration statement has been added to, removed from
     * or replaced within the block's declaration list.
     *
     * @see  NodeList::elementChanged()
     */
    boost::signals2::signal<void(DeclarationStatement*, DeclarationStatement*)>&
    declarationListChanged();

private:
    NodeList<DeclarationStatement> mDeclarations;
};

}}

#endif
