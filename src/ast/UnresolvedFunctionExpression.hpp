/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_FUNCTION_EXPRESSION_HPP
#define _UNRESOLVED_FUNCTION_EXPRESSION_HPP

#include "Function.hpp"
#include "Expression.hpp"

namespace soyac {
namespace ast
{

/**
 * Placeholder for an expression which denotes an undetermined overload of
 * a non-instance function.
 *
 * UnresolvedFunctionExpression instances are replaced with semantically
 * meaningful equivalents by the BasicAnalyzer.
 */
class UnresolvedFunctionExpression : public Expression
{
public:
    /**
     * Iterates over the function overloads possibly denoted by the
     * expression (constant).
     */
    typedef NodeList<Function>::const_iterator overloads_iterator;

    /**
     * Creates an UnresolvedFunctionExpression.
     *
     * @param overloads_begin  Start iterator for the possibly denoted
     *                         function overloads.
     * @param overloads_begin  End iterator for the possibly denoted
     *                         function overloads.
     */
    template <class InputIterator>
    UnresolvedFunctionExpression(InputIterator overloads_begin,
                                 InputIterator overloads_end)
    {
        for (InputIterator it = overloads_begin; it != overloads_end; it++)
            mOverloads.push_back(*it);
    }

    /**
     * Visits the UnresolvedFunctionExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns a constant iterator pointing to the first function overload
     * possibly denoted by the expression.
     *
     * @return  The start iterator for the possible overloads.
     */
    overloads_iterator overloads_begin() const;

    /**
     * Returns a constant iterator pointing past the last function overload
     * possibly denoted by the expression.
     *
     * @return  The start iterator for the possible overloads.
     */
    overloads_iterator overloads_end() const;

private:
    NodeList<Function> mOverloads;
};

}}

#endif
