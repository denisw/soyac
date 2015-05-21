/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ConstructorInitializer.hpp"
#include "Constructor.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


void*
ConstructorInitializer::visit(Visitor* v)
{
    return v->visitConstructorInitializer(this);
}


Constructor*
ConstructorInitializer::constructor() const
{
    return mConstructor.target();
}


ConstructorInitializer::arguments_iterator
ConstructorInitializer::arguments_begin() const
{
    return mArguments.begin();
}


ConstructorInitializer::arguments_iterator
ConstructorInitializer::arguments_end() const
{
    return mArguments.end();
}


}}
