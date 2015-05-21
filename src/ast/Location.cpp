/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "Location.hpp"

namespace soyac {
namespace ast
{


Location::Location()
    : mFileName("<unknown>"),
      mStartLine(0),
      mStartColumn(0),
      mEndLine(0),
      mEndColumn(0)
{
}


Location::Location(const std::string& fileName, int startLine, int startColumn,
                   int endLine, int endColumn)
    : mFileName (fileName),
      mStartLine(startLine),
      mStartColumn(startColumn),
      mEndLine(endLine),
      mEndColumn(endColumn)
{
}


const std::string&
Location::fileName() const
{
    return mFileName;
}


int
Location::startLine() const
{
    return mStartLine;
}


int
Location::startColumn() const
{
    return mStartColumn;
}


int
Location::endLine() const
{
    return mEndLine;
}


int
Location::endColumn() const
{
    return mEndColumn;
}


}}
