/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "DoStatement.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


DoStatement::DoStatement(Expression* condition,
                         Statement* body)
    : StatementWithBody(body),
      mCondition(condition)
{
    assert (condition != NULL);
}


void*
DoStatement::visit(Visitor* v)
{
    return v->visitDoStatement(this);
}


Expression*
DoStatement::condition() const
{
    return mCondition.target();
}


}}
