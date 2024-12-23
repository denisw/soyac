/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedSimpleNameExpression.h"
#include "UnknownType.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

UnresolvedSimpleNameExpression::UnresolvedSimpleNameExpression(const Name& name)
    : mName(name)
{
    assert(name.isSimple());
}

void* UnresolvedSimpleNameExpression::visit(Visitor* v)
{
    return v->visitUnresolvedSimpleNameExpression(this);
}

Type* UnresolvedSimpleNameExpression::type() const { return TYPE_UNKNOWN; }

const Name& UnresolvedSimpleNameExpression::name() const { return mName; }

} // namespace ast
} // namespace soyac
