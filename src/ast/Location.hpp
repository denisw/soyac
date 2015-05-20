/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _LOCATION_HPP
#define _LOCATION_HPP

#include <string>

namespace soyac {
namespace ast
{

/**
 * Represents a location inside a source file.
 */
class Location
{
public:
    /**
     * Creates an unspecified Location.
     */
    Location();

    /**
     * Creates a Location.
     *
     * @param fileName     The name of the source file.
     * @param startLine    The number of the starting line.
     * @param startColumn  The number of the starting column.
     * @param endLine      The number of the ending line.
     * @param endColumn    The number of the ending column.
     */
    Location(const std::string& fileName, int startLine, int startColumn,
             int endLine, int endColumn);

    /**
     * Returns the name of the Location's source file.
     *
     * @return  The source file name.
     */
    const std::string& fileName() const;

    /**
     * Returns the number of the starting line.
     *
     * @return  The starting line number.
     */
    int startLine() const;

    /**
     * Returns the number of the starting column.
     *
     * @return  The starting column number.
     */
    int startColumn() const;

    /**
     * Returns the number of the ending line.
     *
     * @return  The ending line number.
     */
    int endLine() const;

    /**
     * Returns the number of the ending column.
     *
     * @return  The ending column number.
     */
    int endColumn() const;

private:
    std::string mFileName;
    int mStartLine;
    int mStartColumn;
    int mEndLine;
    int mEndColumn;
};


}}

#endif
