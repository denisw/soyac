/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_INSTANCE_FUNCTION_EXPRESSION_HPP
#define _UNRESOLVED_INSTANCE_FUNCTION_EXPRESSION_HPP

#include <cassert>
#include "Function.hpp"
#include "Expression.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents an expression denoting a to-be-determined overload
 * of an instance function.
 *
 * UnresolvedInstanceFunctionExpression instances are replaced with
 * semantically unambiguous equivalents by the BasicAnalyzer.
 */
class UnresolvedInstanceFunctionExpression : public Expression
{
public:
    /**
     * Iterates over the instance method overloads possibly denoted by the
     * expression (constant).
     */
    typedef NodeList<Function>::const_iterator overloads_iterator;

    /**
     * Creates a UnresolvedInstanceFunctionExpression.
     *
     * @param instance          The denoted instance.
     * @param overloads_begin   Start iterator for the possibly denoted
     *                          method overloads.
     * @param overloads_end     End iterator for the possibly denoted
     *                          method overloads.
     */
    template <class InputIterator>
    UnresolvedInstanceFunctionExpression(Expression* instance,
                                         InputIterator overloads_begin,
                                         InputIterator overloads_end)
        : mInstance(instance)
    {
        assert (instance != NULL);

        for (InputIterator it = overloads_begin; it != overloads_end; it++)
            mOverloads.push_back(*it);
    }

    /**
     * Visits the UnresolvedInstanceFunctionExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    virtual Type* type() const;

    /**
     * Returns the expression yielding the instance whose function is
     * denoted by the UnresolvedInstanceFunctionExpression.
     *
     * @returns  The denoted instance.
     */
    Expression* instance() const;

    /**
     * Returns a constant iterator pointing to the first method overload
     * possibly denoted by the expression.
     *
     * @return  The start iterator for the possible overloads.
     */
    overloads_iterator overloads_begin() const;

    /**
     * Returns a constant iterator pointing past the last method overload
     * possibly denoted by the expression.
     *
     * @return  The start iterator for the possible overloads.
     */
    overloads_iterator overloads_end() const;

private:
    Link<Expression> mInstance;
    NodeList<Function> mOverloads;
};

}}

#endif
