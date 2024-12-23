/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2008 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_PROBLEM_TEST_H_
#define SOYA_PROBLEM_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <common/Problem.h>

using namespace soyac;

class ProblemTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ProblemTest);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testLocation);
    CPPUNIT_TEST (testDescription);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp()
    {
        soyac::ast::Location location("foo.soya", 3, 5, 7, 11);
        mProblem = new Problem(Problem::WARNING, location, "bar");
    }

    void tearDown()
    {
        delete mProblem;
    }

    void testType()
    {
        CPPUNIT_ASSERT (mProblem->type() == Problem::WARNING);
    }

    void testLocation()
    {
        CPPUNIT_ASSERT (mProblem->location().fileName() == "foo.soya");
        CPPUNIT_ASSERT (mProblem->location().startLine() == 3);
        CPPUNIT_ASSERT (mProblem->location().startColumn() == 5);
        CPPUNIT_ASSERT (mProblem->location().endLine() == 7);
        CPPUNIT_ASSERT (mProblem->location().endColumn() == 11);
    }

    void testDescription()
    {
        CPPUNIT_ASSERT (mProblem->description() == "bar");
    }

private:
    Problem* mProblem;
};

#endif
