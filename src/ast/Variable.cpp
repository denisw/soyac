/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "Variable.h"
#include "UnknownType.h"
#include "Visitor.h"
#include <cassert>

namespace soyac {
namespace ast {

Variable::Variable(const Name& name, Type* type, Expression* initializer)
    : DeclaredEntity(name)
    , mType(type)
    , mInitializer(initializer)
{
    assert(type != nullptr);
}

void* Variable::visit(Visitor* v) { return v->visitVariable(this); }

Type* Variable::type() const
{
    if (mType.target() == TYPE_UNKNOWN && initializer() != nullptr) {
        return initializer()->type();
    } else {
        return mType.target();
    }
}

Expression* Variable::initializer() const { return mInitializer.target(); }

} // namespace ast
} // namespace soyac
