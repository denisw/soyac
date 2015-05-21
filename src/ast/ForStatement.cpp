/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ForStatement.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


void*
ForStatement::visit(Visitor* v)
{
    return v->visitForStatement(this);
}


Expression*
ForStatement::condition() const
{
    return mCondition.target();
}


ForStatement::initializers_iterator
ForStatement::initializers_begin() const
{
    return mInitializers.begin();
}


ForStatement::initializers_iterator
ForStatement::initializers_end() const
{
    return mInitializers.end();
}


ForStatement::iterators_iterator
ForStatement::iterators_begin() const
{
    return mIterators.begin();
}


ForStatement::iterators_iterator
ForStatement::iterators_end() const
{
    return mIterators.end();
}


}}
