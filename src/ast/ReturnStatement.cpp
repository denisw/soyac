/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ReturnStatement.h"
#include "Visitor.h"
#include <cassert>

namespace soyac {
namespace ast {

ReturnStatement::ReturnStatement(Expression* returnValue)
    : mReturnValue(returnValue)
{
    assert(returnValue != nullptr);
}

void* ReturnStatement::visit(Visitor* v)
{
    return v->visitReturnStatement(this);
}

Expression* ReturnStatement::returnValue() const
{
    return mReturnValue.target();
}

} // namespace ast
} // namespace soyac
