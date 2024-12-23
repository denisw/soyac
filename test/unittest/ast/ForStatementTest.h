/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_FOR_STATEMENT_TEST_H_
#define SOYA_FOR_STATEMENT_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/ForStatement.h>
#include "DummyExpression.h"
#include "DummyStatement.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class ForStatementTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ForStatementTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testCondition);
    CPPUNIT_TEST (testConditionReplaceable);
    CPPUNIT_TEST (testInitializers);
    CPPUNIT_TEST (testIterators);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (ForStatement);

public:
    void setUp()
    {
        mCondition = new DummyExpression;

        mInitializers = new Statement*[3];
        mInitializers[0] = new DummyStatement;
        mInitializers[1] = new DummyStatement;
        mInitializers[2] = new DummyStatement;

        mIterators = new Statement*[3];
        mIterators[0] = new DummyStatement;
        mIterators[1] = new DummyStatement;
        mIterators[2] = new DummyStatement;

        mStmt = new ForStatement(
          mCondition,
          mInitializers, mInitializers + 3,
          mIterators, mIterators + 3,
          new DummyStatement);
    }

    void tearDown()
    {
        delete mStmt;
        delete [] mInitializers;
        delete [] mIterators;
    }

    void testVisit()
    {
        ForStatementTestVisitor v;
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

    void testInitializers()
    {
        ForStatement::initializers_iterator it = mStmt->initializers_begin();

        CPPUNIT_ASSERT (*it == mInitializers[0]);
        it++;
        CPPUNIT_ASSERT (*it == mInitializers[1]);
        it++;
        CPPUNIT_ASSERT (*it == mInitializers[2]);
        it++;
        CPPUNIT_ASSERT (it == mStmt->initializers_end());        
    }

    void testIterators()
    {
        ForStatement::iterators_iterator it = mStmt->iterators_begin();

        CPPUNIT_ASSERT (*it == mIterators[0]);
        it++;
        CPPUNIT_ASSERT (*it == mIterators[1]);
        it++;
        CPPUNIT_ASSERT (*it == mIterators[2]);
        it++;
        CPPUNIT_ASSERT (it == mStmt->iterators_end());    
    }

private:
    ForStatement* mStmt;
    Expression* mCondition;
    Statement** mInitializers;
    Statement** mIterators;
};

#endif
