/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _OBJECT_CREATION_EXPRESSION_HPP
#define _OBJECT_CREATION_EXPRESSION_HPP

#include <cassert>
#include "Constructor.hpp"
#include "Expression.hpp"
#include "NodeList.hpp"
#include "Type.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents an object creation expression.
 *
 * @see ArrayCreationExpression
 */
class ObjectCreationExpression : public Expression
{
public:
    /**
     * Iterates over the creation expression's constructor arguments
     * (constant).
     */
    typedef NodeList<Expression>::const_iterator arguments_iterator;

    /**
     * Creates an ObjectCreationExpression.
     *
     * @param type             The created object's type.
     * @param constructor      The called constructor.
     * @param arguments_begin  Start iterator for the constructor arguments.
     * @param arguments_end    End iterator for the constructor arguments.
     */
    template <class InputIterator>
    ObjectCreationExpression(Type* type,
                             Constructor* constructor,
                             InputIterator arguments_begin,
                             InputIterator arguments_end)
        : mType(type),
          mConstructor(constructor)
    {
        assert (type != NULL);
        assert (constructor != NULL);

        for (InputIterator it = arguments_begin; it != arguments_end; it++)
            mArguments.push_back(*it);
    }

    /**
     * Visits the ObjectCreationExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    Type* type() const;

    /**
     * Returns the constructor called by the creation expression.
     *
     * @return  The called constructor.
     */
    Constructor* constructor() const;

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
    Link<Constructor> mConstructor;
    NodeList<Expression> mArguments;
};

}}

#endif
