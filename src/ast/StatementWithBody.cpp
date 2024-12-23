/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "StatementWithBody.h"
#include <cassert>

namespace soyac {
namespace ast {

StatementWithBody::StatementWithBody(Statement* body)
    : mBody(body)
{
    assert(body != nullptr);
}

Statement* StatementWithBody::body() const { return mBody.target(); }

} // namespace ast
} // namespace soyac
