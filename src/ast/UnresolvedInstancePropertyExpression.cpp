/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "UnresolvedInstancePropertyExpression.hpp"
#include "UnknownType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


UnresolvedInstancePropertyExpression::UnresolvedInstancePropertyExpression(
  Expression* instance,
  Property* target)
    : mInstance(instance),
      mTarget(target)
{
    assert (instance != NULL);
    assert (target != NULL);
}


void*
UnresolvedInstancePropertyExpression::visit(Visitor* v)
{
    return v->visitUnresolvedInstancePropertyExpression(this);
}


Type*
UnresolvedInstancePropertyExpression::type() const
{
    return TYPE_UNKNOWN;
}


Expression*
UnresolvedInstancePropertyExpression::instance() const
{
    return mInstance.target();
}


Property*
UnresolvedInstancePropertyExpression::target() const
{
    return mTarget.target();
}


}}
