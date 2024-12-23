/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_RETURN_STATEMENT_TEST_H_
#define SOYA_RETURN_STATEMENT_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/ReturnStatement.h>
#include "DummyExpression.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class ReturnStatementTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ReturnStatementTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testReturnValue);
    CPPUNIT_TEST (testReturnValueReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (ReturnStatement);

public:
    void setUp()
    {
        mReturnValue = new DummyExpression;
        mStmt = new ReturnStatement(mReturnValue);
    }

    void tearDown()
    {
        delete mStmt;
    }

    void testVisit()
    {
        ReturnStatementTestVisitor v;
        CPPUNIT_ASSERT (((bool) mStmt->visit(&v)) == true);
    }

    void testReturnValue()
    {
        CPPUNIT_ASSERT (mStmt->returnValue() == mReturnValue);
    }

    void testReturnValueReplaceable()
    {
        DummyExpression returnValue2;
        mReturnValue->replaceWith(&returnValue2);
        CPPUNIT_ASSERT (mStmt->returnValue() == &returnValue2);
    }

private:
    ReturnStatement* mStmt;
    Expression* mReturnValue;
};

#endif
