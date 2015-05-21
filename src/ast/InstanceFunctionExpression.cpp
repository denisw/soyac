/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "InstanceFunctionExpression.hpp"
#include "FunctionType.hpp"
#include "UnknownType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


InstanceFunctionExpression::InstanceFunctionExpression(Expression* instance,
                                                       Function* target)
    : mInstance(instance),
      mTarget(target)
{
    assert (instance != NULL);
    assert (target != NULL);
}


void*
InstanceFunctionExpression::visit(Visitor* v)
{
    return v->visitInstanceFunctionExpression(this);
}


Type*
InstanceFunctionExpression::type() const
{
    return target()->type();
}


Expression*
InstanceFunctionExpression::instance() const
{
    return mInstance.target();
}


Function*
InstanceFunctionExpression::target() const
{
    return mTarget.target();
}


}}
