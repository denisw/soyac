/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _WHILE_STATEMENT_TEST_HPP
#define _WHILE_STATEMENT_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/WhileStatement.hpp>
#include "DummyExpression.hpp"
#include "DummyStatement.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class WhileStatementTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (WhileStatementTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testCondition);
    CPPUNIT_TEST (testConditionReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (WhileStatement);

public:
    void setUp()
    {
        mCondition = new DummyExpression;
        mStmt = new WhileStatement(mCondition, new DummyStatement);
    }

    void tearDown()
    {
        delete mStmt;
    }

    void testVisit()
    {
        WhileStatementTestVisitor v;
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

private:
    WhileStatement* mStmt;
    Expression* mCondition;
};

#endif
