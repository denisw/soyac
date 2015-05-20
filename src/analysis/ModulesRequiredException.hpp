/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _MODULES_REQUIRED_EXCEPTION_HPP
#define _MODULES_REQUIRED_EXCEPTION_HPP

#include <cassert>
#include <exception>
#include <list>
#include <string>

namespace soyac {
namespace analysis
{

/**
 * Signals that a source file requires particular modules to be
 * processed before it can be compiled.
 */
class ModulesRequiredException : public std::exception
{
public:
    /**
     * Iterates over the required modules' names (constant).
     */
    typedef std::list<std::string>::const_iterator const_modules_iterator;

    /**
     * Creates a ModulesRequiredException.
     *
     * @param moduleName  The names of the required modules.
     */
    ModulesRequiredException(const std::list<std::string>& modules)
        : mModules(modules)
    {
        assert (!mModules.empty());
    }

    /**
     * ModulesRequiredException destructor.
     */
    virtual ~ModulesRequiredException() throw () {}

    /**
     * Returns an iterator pointing to the first required module's name.
     *
     * @return  The start iterator.
     */
    const_modules_iterator modules_begin() const
    {
        return mModules.begin();
    }

    /**
     * Returns an iterator pointing past the last required module's name.
     *
     * @return  The end iterator.
     */
    const_modules_iterator modules_end() const
    {
        return mModules.end();
    }

private:
    std::list<std::string> mModules;
};

}}

#endif
