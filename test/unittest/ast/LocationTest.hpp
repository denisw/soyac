/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _LOCATION_TEST_HPP
#define _LOCATION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Location.hpp>

using namespace soyac::ast;

class LocationTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (LocationTest);
    CPPUNIT_TEST (testFileName);
    CPPUNIT_TEST (testStartLine);
    CPPUNIT_TEST (testStartColumn);
    CPPUNIT_TEST (testEndLine);
    CPPUNIT_TEST (testEndColumn);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp()
    {
        mLocation = new Location("foo.soya", 3, 5, 7, 11);
    }

    void tearDown()
    {
        delete mLocation;
    }

    void testFileName()
    {
        CPPUNIT_ASSERT (mLocation->fileName() == "foo.soya");
    }

    void testStartLine()
    {
        CPPUNIT_ASSERT (mLocation->startLine() == 3);
    }

    void testStartColumn()
    {
        CPPUNIT_ASSERT (mLocation->startColumn() == 5);
    }

    void testEndLine()
    {
        CPPUNIT_ASSERT (mLocation->endLine() == 7);
    }

    void testEndColumn()
    {
        CPPUNIT_ASSERT (mLocation->endColumn() == 11);
    }

private:
    Location* mLocation;
};

#endif
