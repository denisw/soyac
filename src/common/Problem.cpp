/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "Problem.h"

namespace soyac {

Problem::Problem(Problem::Type type, const soyac::ast::Location& location,
    const std::string& description)
    : mType(type)
    , mLocation(location)
    , mDescription(description)
{
}

Problem::Type Problem::type() const { return mType; }

const soyac::ast::Location& Problem::location() const { return mLocation; }

const std::string& Problem::description() const { return mDescription; }

} // namespace soyac
