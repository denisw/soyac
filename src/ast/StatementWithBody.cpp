/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "StatementWithBody.hpp"

namespace soyac {
namespace ast
{


StatementWithBody::StatementWithBody(Statement* body)
    : mBody(body)
{
    assert (body != NULL);
}


Statement*
StatementWithBody::body() const
{
    return mBody.target();
}


}}
