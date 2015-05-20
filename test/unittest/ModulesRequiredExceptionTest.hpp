/*
 * soyac - The Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _MODULES_REQUIRED_EXCEPTION_TEST_HPP
#define _MODULES_REQUIRED_EXCEPTION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <list>
#include <ModulesRequiredException.hpp>

using soyac::ModulesRequiredException;

class ModulesRequiredExceptionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ModulesRequiredExceptionTest);
    CPPUNIT_TEST (testModules);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp()
    {
        std::list<std::string> modules;
        modules.push_back("foo");
        modules.push_back("foo::bar");
        modules.push_back("bar::baz");

        mException = new ModulesRequiredException(modules);
    }

    void tearDown()
    {
        delete mException;
    }

    void testModules()
    {
        ModulesRequiredException::const_modules_iterator it = mException->modules_begin();

        CPPUNIT_ASSERT (*it == "foo");
        it++;
        CPPUNIT_ASSERT (*it == "foo::bar");
        it++;
        CPPUNIT_ASSERT (*it == "bar::baz");
        it++;
        CPPUNIT_ASSERT (it == mException->modules_end());
    }

private:
    ModulesRequiredException* mException;
};

#endif
