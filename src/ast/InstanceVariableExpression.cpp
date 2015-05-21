/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "InstanceVariableExpression.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


InstanceVariableExpression::InstanceVariableExpression(Expression* instance,
                                                       Variable* target)
    : mInstance(instance),
      mTarget(target)
{
    assert (instance != NULL);
    assert (target != NULL);
}


void*
InstanceVariableExpression::visit(Visitor* v)
{
    return v->visitInstanceVariableExpression(this);
}


Type*
InstanceVariableExpression::type() const
{
    return target()->type();
}


Expression*
InstanceVariableExpression::instance() const
{
    return mInstance.target();
}


Variable*
InstanceVariableExpression::target() const
{
    return mTarget.target();
}


}}
