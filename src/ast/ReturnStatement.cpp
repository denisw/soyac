/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "ReturnStatement.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


ReturnStatement::ReturnStatement(Expression* returnValue)
    : mReturnValue(returnValue)
{
    assert (returnValue != NULL);
}


void*
ReturnStatement::visit(Visitor* v)
{
    return v->visitReturnStatement(this);
}


Expression*
ReturnStatement::returnValue() const
{
    return mReturnValue.target();
}


}}
