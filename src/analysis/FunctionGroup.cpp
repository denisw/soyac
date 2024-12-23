/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "FunctionGroup.hpp"

namespace soyac {
namespace analysis {

FunctionGroup::overloads_iterator FunctionGroup::overloads_begin() const
{
    return mOverloads.begin();
}

FunctionGroup::overloads_iterator FunctionGroup::overloads_end() const
{
    return mOverloads.end();
}

void FunctionGroup::addOverload(Function* func)
{
    assert(func->name() == (*overloads_begin())->name());
    assert(func->parent() == (*overloads_begin())->parent());

    mOverloads.push_back(func);
}

} // namespace analysis
} // namespace soyac
