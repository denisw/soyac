/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "WhileStatement.h"
#include "Visitor.h"
#include <cassert>

namespace soyac {
namespace ast {

WhileStatement::WhileStatement(Expression* condition, Statement* body)
    : StatementWithBody(body)
    , mCondition(condition)
{
    assert(condition != nullptr);
}

void* WhileStatement::visit(Visitor* v) { return v->visitWhileStatement(this); }

Expression* WhileStatement::condition() const { return mCondition.target(); }

} // namespace ast
} // namespace soyac
