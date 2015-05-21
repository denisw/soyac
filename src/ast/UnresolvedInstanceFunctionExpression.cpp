/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedInstanceFunctionExpression.hpp"
#include "UnknownType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


void*
UnresolvedInstanceFunctionExpression::visit(Visitor* v)
{
    return v->visitUnresolvedInstanceFunctionExpression(this);
}


Type*
UnresolvedInstanceFunctionExpression::type() const
{
    return TYPE_UNKNOWN;
}


Expression*
UnresolvedInstanceFunctionExpression::instance() const
{
    return mInstance.target();
}


UnresolvedInstanceFunctionExpression::overloads_iterator
UnresolvedInstanceFunctionExpression::overloads_begin() const
{
    return mOverloads.begin();
}


UnresolvedInstanceFunctionExpression::overloads_iterator
UnresolvedInstanceFunctionExpression::overloads_end() const
{
    return mOverloads.end();
}


}}
