/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FOR_STATEMENT_HPP
#define _FOR_STATEMENT_HPP

#include <cassert>
#include "Expression.hpp"
#include "NodeList.hpp"
#include "StatementWithBody.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents an "for" statement.
 *
 * (See the Soya Language Reference,)
 */
class ForStatement : public StatementWithBody
{
public:
    /**
     * Iterates over the "for" statement's initializer statements (constant).
     */
    typedef NodeList<Statement>::const_iterator initializers_iterator;

    /**
     * Iterates over the "for" statement's iterator statements (constant).
     */
    typedef NodeList<Statement>::const_iterator iterators_iterator;

    /**
     * Creates a ForStatement.
     *
     * @param condition           The loop condition.
     * @param initializers_begin  Start iterator for the "for" statement's
     *                            initializer statements.
     * @param initializers_begin  End iterator for the "for" statement's
     *                            initializer statements.
     * @param iterators_begin     Start iterator for the "for" statement's
     *                            iterator statements.
     * @param iterators_begin     End iterator for the "for" statement's
     *                            iterator statements.
     * @param body                The "for" statement's body.
     */
    template <class InputIterator1, class InputIterator2>
    ForStatement(Expression* condition,
                 InputIterator1 initializers_begin,
                 InputIterator1 initializers_end,
                 InputIterator2 iterators_begin,
                 InputIterator2 iterators_end,
                 Statement* body)
        : StatementWithBody(body),
          mCondition(condition)
    {
        for (InputIterator1 it = initializers_begin;
             it != initializers_end; it++)
        {
            mInitializers.push_back(*it);
        }


        for (InputIterator2 it = iterators_begin;
             it != iterators_end; it++)
        {
            mIterators.push_back(*it);
        }
    }

    /**
     * Visits the ForStatement.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the condition expression of the "for" statement.
     *
     * @return  The loop condition.
     */
    Expression* condition() const;

    /**
     * Returns a constant iterator pointing to the first initializer
     * statement of the "for" statement.
     *
     * @return  The start iterator for the "for" statement's initializer
     *          statements.
     */
    initializers_iterator initializers_begin() const;

    /**
     * Returns a constant iterator pointing past the last initializer
     * statement of the "for" statement.
     *
     * @return  The end iterator for the "for" statement's initializer
     *          statements.
     */
    initializers_iterator initializers_end() const;

    /**
     * Returns a constant iterator pointing to the first iterator
     * statement of the "for" statement.
     *
     * @return  The start iterator for the "for" statement's iterator
     *          statements.
     */
    iterators_iterator iterators_begin() const;

    /**
     * Returns a constant iterator pointing past the last iterator
     * statement of the "for" statement.
     *
     * @return  The end iterator for the "for" statement's iterator
     *          statements.
     */
    iterators_iterator iterators_end() const;

private:
    NodeList<Statement> mInitializers;
    Link<Expression> mCondition;
    NodeList<Statement> mIterators;
};

}}

#endif
