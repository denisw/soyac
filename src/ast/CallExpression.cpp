/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "CallExpression.hpp"
#include "Function.hpp"
#include "FunctionExpression.hpp"
#include "FunctionType.hpp"
#include "InstanceFunctionExpression.hpp"
#include "UnknownType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast {

CallExpression::CallExpression(Expression* callee)
    : mCallee(callee)
{
    assert(callee != NULL);
}

void* CallExpression::visit(Visitor* v) { return v->visitCallExpression(this); }

Type* CallExpression::type() const
{
    if (dynamic_cast<FunctionType*>(callee()->type()) != NULL) {
        return ((FunctionType*)callee()->type())->returnType();
    } else {
        return TYPE_UNKNOWN;
    }
}

Expression* CallExpression::callee() const { return mCallee.target(); }

CallExpression::arguments_iterator CallExpression::arguments_begin() const
{
    return mArguments.begin();
}

CallExpression::arguments_iterator CallExpression::arguments_end() const
{
    return mArguments.end();
}

} // namespace ast
} // namespace soyac
