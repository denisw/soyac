/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _CONSTRUCTOR_INITIALIZER_HPP
#define _CONSTRUCTOR_INITIALIZER_HPP

#include "Expression.hpp"
#include "NodeList.hpp"

namespace soyac {
namespace ast
{

class Constructor;

/**
 * Represents a constructor initializer.
 *
 * @see  UnresolvedConstructorInitializer
 * @see  UnresolvedBaseConstructorInitializer
 */
class ConstructorInitializer : public Node
{
public:
    /**
     * Iterates over the constructor initializer's arguments (constant).
     */
    typedef NodeList<Expression>::const_iterator arguments_iterator;

    /**
     * Creates a ConstructorInitializer.
     *
     * @param constructor      The called constructor.
     * @param arguments_begin  Start iterator for the initializer's
     *                         constructor arguments.
     * @param arguments_end    End iterator for the initializer's
     *                         constructor arguments.
     */
    template <class InputIterator>
    ConstructorInitializer(Constructor* constructor,
                           InputIterator arguments_begin,
                           InputIterator arguments_end)
        : mConstructor(constructor),
          mArguments(arguments_begin, arguments_end)
    {
        assert (constructor != NULL);
    }

    /**
     * Visits the ConstructorInitializer.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the constructor called by the constructor initializer.
     *
     * @return  The called constructor.
     */
    Constructor* constructor() const;

    /**
     * Returns a constant iterator pointing to the first argument of
     * the constructor initializer.
     *
     * @return  The start iterator for the initializer's arguments.
     */
    arguments_iterator arguments_begin() const;

    /**
     * Returns an iterator pointing past the last argument of the
     * constructor initializer.
     *
     * @return  The end iterator for the initializer's arguments.
     */
    arguments_iterator arguments_end() const;

protected:
    /**
     * Creates a ConstructorInitializer with constructor() returning
     * null. Used by the UnresolvedConstructorInitializer and
     * UnresolvedBaseConstructorInitializer subclasses.
     *
     * @param arguments_begin  Start iterator for the initializer's
     *                         constructor arguments.
     * @param arguments_end    End iterator for the initializer's
     *                         constructor arguments.
     */
    template <class InputIterator>
    ConstructorInitializer(InputIterator arguments_begin,
                           InputIterator arguments_end)
        : mConstructor(NULL),
          mArguments(arguments_begin, arguments_end)
    {
    }

private:
    Link<Constructor> mConstructor;
    NodeList<Expression> mArguments;
};

}}

#endif
