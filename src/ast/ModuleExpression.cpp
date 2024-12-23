/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ModuleExpression.h"
#include "UnknownType.h"
#include "Visitor.h"
#include <cassert>

namespace soyac {
namespace ast {

ModuleExpression::ModuleExpression(Module* target)
    : mTarget(target)
{
    assert(target != nullptr);
}

void* ModuleExpression::visit(Visitor* v)
{
    return v->visitModuleExpression(this);
}

Type* ModuleExpression::type() const { return TYPE_UNKNOWN; }

Module* ModuleExpression::target() const { return mTarget.target(); }

} // namespace ast
} // namespace soyac
