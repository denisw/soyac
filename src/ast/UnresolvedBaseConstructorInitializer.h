/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNRESOLVED_BASE_CONSTRUCTOR_INITIALIZER_H_
#define SOYA_UNRESOLVED_BASE_CONSTRUCTOR_INITIALIZER_H_

#include "ConstructorInitializer.h"

namespace soyac {
namespace ast {

/**
 * Represents a constructor initializer calling a to-be-determined
 * class or struct constructor.
 *
 * @see  UnresolvedBaseBaseConstructorInitializer
 */
class UnresolvedBaseConstructorInitializer : public ConstructorInitializer {
public:
    /**
     * Creates an UnresolvedBaseConstructorInitializer.
     *
     * @param arguments_begin  Start iterator for the initializer's
     *                         constructor arguments.
     * @param arguments_end    End iterator for the initializer's
     *                         constructor arguments.
     */
    template <class InputIterator>
    UnresolvedBaseConstructorInitializer(
        InputIterator arguments_begin, InputIterator arguments_end)
        : ConstructorInitializer(arguments_begin, arguments_end)
    {
    }

    /**
     * Creates an UnresolvedBaseConstructorInitializer
     * without constructor arguments.
     */
    UnresolvedBaseConstructorInitializer();

    /**
     * Visits the UnresolvedBaseConstructorInitializer.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);
};

} // namespace ast
} // namespace soyac

#endif
