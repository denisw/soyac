/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _STATEMENT_WITH_BODY_TEST_HPP
#define _STATEMENT_WITH_BODY_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/StatementWithBody.hpp>
#include "DummyExpression.hpp"
#include "DummyStatement.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class StatementWithBodyTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (StatementWithBodyTest);
    CPPUNIT_TEST (testBody);
    CPPUNIT_TEST (testBodyReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (StatementWithBody);

public:
    void setUp()
    {
        mBody = new DummyStatement;
        mStmt = new StatementWithBody(mBody);
    }

    void tearDown()
    {
        delete mStmt;
    }

    void testBody()
    {
        CPPUNIT_ASSERT (mStmt->body() == mBody);
    }

    void testBodyReplaceable()
    {
        DummyStatement body2;
        mBody->replaceWith(&body2);
        CPPUNIT_ASSERT (mStmt->body() == &body2);
    }

private:
    StatementWithBody* mStmt;
    Statement* mBody;
};

#endif
