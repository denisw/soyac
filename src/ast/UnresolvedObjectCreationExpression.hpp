/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_OBJECT_CREATION_EXPRESSION_HPP
#define _UNRESOLVED_OBJECT_CREATION_EXPRESSION_HPP

#include <cassert>
#include "Expression.hpp"
#include "NodeList.hpp"
#include "Type.hpp"
#include "UnknownType.hpp"

namespace soyac {
namespace ast
{


/**
 * Represents an expression creating an object of a to-be-determined
 * type.
 *
 * @see ObjectCreationExpression
 */
class UnresolvedObjectCreationExpression : public Expression
{
public:
    /**
     * Iterates over the creation expression's constructor arguments
     * (constant).
     */
    typedef NodeList<Expression>::const_iterator arguments_iterator;

    /**
     * Creates an UnresolvedObjectCreationExpression.
     *
     * @param type             The created object's type.
     * @param arguments_begin  Start iterator for the constructor arguments.
     * @param arguments_end    End iterator for the constructor arguments.
     */
    template <class InputIterator>
    UnresolvedObjectCreationExpression(UnknownType* type,
                                       InputIterator arguments_begin,
                                       InputIterator arguments_end)
        : mType(type)
    {
        assert (type != NULL);

        for (InputIterator it = arguments_begin; it != arguments_end; it++)
            mArguments.push_back(*it);
    }

    /**
     * Visits the UnresolvedObjectCreationExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    Type* type() const;

    /**
     * Returns a constant iterator pointing to the first constructor
     * argument of the creation expression.
     *
     * @return  The start iterator for the constructor arguments.
     */
    arguments_iterator arguments_begin() const;

    /**
     * Returns a constant iterator pointing past the last constructor
     * argument of the creation expression.
     *
     * @return  The end iterator for the constructor arguments.
     */
    arguments_iterator arguments_end() const;

private:
    Link<Type> mType;
    NodeList<Expression> mArguments;
};

}}

#endif
