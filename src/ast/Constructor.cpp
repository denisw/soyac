/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "Constructor.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

Constructor::Constructor(ConstructorInitializer* initializer, Block* body)
    : Function(CONSTRUCTOR_NAME, TYPE_VOID, body)
    , mInitializer(initializer)
{
    assert(initializer != nullptr);
    assert(body != nullptr);
}

void* Constructor::visit(Visitor* v) { return v->visitConstructor(this); }

ConstructorInitializer* Constructor::initializer() const
{
    return mInitializer.target();
}

} // namespace ast
} // namespace soyac
