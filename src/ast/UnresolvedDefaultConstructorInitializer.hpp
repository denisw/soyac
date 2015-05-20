/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_DEFAULT_CONSTRUCTOR_INITIALIZER_HPP
#define _UNRESOLVED_DEFAULT_CONSTRUCTOR_INITIALIZER_HPP

#include "ConstructorInitializer.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents a constructor initializer with to-be-determined
 * default behavior (depending on whether the constructor
 * belongs to a struct or a class).
 *
 * @see  UnresolvedBaseConstructorInitializer
 */
class UnresolvedDefaultConstructorInitializer : public ConstructorInitializer
{
public:
    /**
     * Creates an UnresolvedDefaultConstructorInitializer.
     */
    UnresolvedDefaultConstructorInitializer();

    /**
     * Visits the UnresolvedDefaultConstructorInitializer.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);
};

}}

#endif
