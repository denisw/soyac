/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ExpressionStatement.hpp"
#include "Visitor.hpp"
#include <cassert>

namespace soyac {
namespace ast {

ExpressionStatement::ExpressionStatement(Expression* expr)
    : mExpression(expr)
{
    assert(expr != NULL);
}

void* ExpressionStatement::visit(Visitor* v)
{
    return v->visitExpressionStatement(this);
}

Expression* ExpressionStatement::expression() const
{
    return mExpression.target();
}

} // namespace ast
} // namespace soyac
