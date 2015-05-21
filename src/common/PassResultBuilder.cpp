/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "PassResultBuilder.hpp"
#include "Problem.hpp"

namespace soyac
{


PassResultBuilder::PassResultBuilder()
    : mPassResult(NULL)
{
}

void
PassResultBuilder::addError(const soyac::ast::Location& location,
                            const std::string& description)
{
    if (mPassResult == NULL)
        mPassResult = new PassResult;

    mPassResult->mProblems.push_back(
      new Problem(Problem::ERROR, location, description));
}


void
PassResultBuilder::addError(const soyac::ast::Location& location,
                            const boost::format& description)
{
    addError(location, description.str());
}


void
PassResultBuilder::addError(const soyac::ast::Node* source,
                            const std::string& description)
{
    addError(source->location(), description);
}


void
PassResultBuilder::addError(const soyac::ast::Node* source,
                            const boost::format& description)
{
    addError(source->location(), description.str());
}


void
PassResultBuilder::addWarning(const soyac::ast::Node* source,
                              const std::string& description)
{
    if (mPassResult == NULL)
        mPassResult = new PassResult;

    mPassResult->mProblems.push_back(
      new Problem(Problem::WARNING, source->location(), description));
}


void
PassResultBuilder::addWarning(const soyac::ast::Node* source,
                              const boost::format& description)
{
    addWarning(source, description.str());
}


PassResult*
PassResultBuilder::result() const
{
    return mPassResult;
}


}
