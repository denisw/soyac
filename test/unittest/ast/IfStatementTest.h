/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_IF_STATEMENT_TEST_H_
#define SOYA_IF_STATEMENT_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/IfStatement.h>
#include "DummyExpression.h"
#include "DummyStatement.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class IfStatementTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (IfStatementTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testCondition);
    CPPUNIT_TEST (testConditionReplaceable);
    CPPUNIT_TEST (testElseBody);
    CPPUNIT_TEST (testElseBodyReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (IfStatement);

public:
    void setUp()
    {
        mCondition = new DummyExpression;
        mElseBody = new DummyStatement;
        mStmt = new IfStatement(mCondition, new DummyStatement, mElseBody);
    }

    void tearDown()
    {
        delete mStmt;
    }

    void testVisit()
    {
        IfStatementTestVisitor v;
        CPPUNIT_ASSERT (((bool) mStmt->visit(&v)) == true);
    }

    void testCondition()
    {
        CPPUNIT_ASSERT (mStmt->condition() == mCondition);
    }

    void testConditionReplaceable()
    {
        DummyExpression condition2;
        mCondition->replaceWith(&condition2);
        CPPUNIT_ASSERT (mStmt->condition() == &condition2);
    }

    void testElseBody()
    {
        CPPUNIT_ASSERT (mStmt->elseBody() == mElseBody);
    }

    void testElseBodyReplaceable()
    {
        DummyStatement body2;
        mElseBody->replaceWith(&body2);
        CPPUNIT_ASSERT (mStmt->elseBody() == &body2);
    }

private:
    IfStatement* mStmt;
    Expression* mCondition;
    Statement* mElseBody;
};

#endif
