/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "IfStatement.h"
#include "Visitor.h"
#include <cassert>

namespace soyac {
namespace ast {

IfStatement::IfStatement(
    Expression* condition, Statement* body, Statement* elseBody)
    : StatementWithBody(body)
    , mCondition(condition)
    , mElseBody(elseBody)
{
    assert(condition != nullptr);
}

void* IfStatement::visit(Visitor* v) { return v->visitIfStatement(this); }

Expression* IfStatement::condition() const { return mCondition.target(); }

Statement* IfStatement::elseBody() const { return mElseBody.target(); }

} // namespace ast
} // namespace soyac
