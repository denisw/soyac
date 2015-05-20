/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _CALL_EXPRESSION_HPP
#define _CALL_EXPRESSION_HPP

#include <cassert>
#include "Expression.hpp"
#include "NodeList.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents a call expression.
 *
 * (See the Soya Language Reference,)
 */
class CallExpression : public Expression
{
public:
    /**
     * Iterates over the call expression's arguments (constant).
     */
    typedef NodeList<Expression>::const_iterator arguments_iterator;

    /**
     * Creates a CallExpression.
     *
     * @param callee           The callee.
     * @param arguments_begin  Start iterator for the call expression's
     *                         arguments.
     * @param arguments_end    End iterator for the call expression's
     *                         arguments.
     */
    template <class InputIterator>
    CallExpression(Expression* callee,
                   InputIterator arguments_begin,
                   InputIterator arguments_end)
        : mCallee(callee)
    {
        assert (callee != NULL);

        for (InputIterator it = arguments_begin; it != arguments_end; it++)
            mArguments.push_back(*it);
    }

    /**
     * Creates a CallExpression without arguments.
     *
     * @param callee  The callee.
     */
    CallExpression(Expression* callee);

    /**
     * Visits the CallExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the Expression which evaluates to the function called by the
     * call expression.
     *
     * @returns  The callee.
     */
    Expression* callee() const;

    /**
     * Returns a constant iterator pointing to the first argument of
     * the call expression.
     *
     * @return  The start iterator for the call expression's arguments.
     */
    arguments_iterator arguments_begin() const;

    /**
     * Returns an iterator pointing past the last argument of the
     * call expression.
     *
     * @return  The end iterator for the call expression's arguments.
     */
    arguments_iterator arguments_end() const;

private:
    Link<Expression> mCallee;
    NodeList<Expression> mArguments;
};

}}

#endif
