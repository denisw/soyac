/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _PROBLEM_HPP
#define _PROBLEM_HPP

#include <ast/Location.hpp>
#include <string>

namespace soyac {

/**
 * Represents a problem with a source file found by a compiler pass.
 */
class Problem {
public:
    /**
     * The problem's type (whether it is an error or a waring).
     */
    enum Type { ERROR, WARNING };

    /**
     * Creates a Problem.
     *
     * @param type         The problem's type.
     * @param location     The location where the problem was found.
     * @param description  The problem's description.
     */
    Problem(Type type, const soyac::ast::Location& location,
        const std::string& description);

    /**
     * Returns the problems type. There are two types of problems: errors
     * and warnings. Errors identify that the processed abstract syntax
     * tree does not conform to the Soya Language Reference, while warnings
     * are only informatory and try to preserve a user from common mistakes.
     *
     * @return  The problem's type.
     */
    Type type() const;

    /**
     * Returns the location where the problem was found.
     *
     * @return  The problem's location.
     */
    const soyac::ast::Location& location() const;

    /**
     * Returns a string describing the problem.
     *
     * @return  The problem's description.
     */
    const std::string& description() const;

private:
    Type mType;
    soyac::ast::Location mLocation;
    std::string mDescription;
};

} // namespace soyac

#endif
