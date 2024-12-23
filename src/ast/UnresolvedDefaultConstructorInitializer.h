/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNRESOLVED_DEFAULT_CONSTRUCTOR_INITIALIZER_H_
#define SOYA_UNRESOLVED_DEFAULT_CONSTRUCTOR_INITIALIZER_H_

#include "ConstructorInitializer.h"

namespace soyac {
namespace ast {

/**
 * Represents a constructor initializer with to-be-determined
 * default behavior (depending on whether the constructor
 * belongs to a struct or a class).
 *
 * @see  UnresolvedBaseConstructorInitializer
 */
class UnresolvedDefaultConstructorInitializer : public ConstructorInitializer {
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

} // namespace ast
} // namespace soyac

#endif
