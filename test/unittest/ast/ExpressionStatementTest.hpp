/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _EXPRESSION_STATEMENT_TEST_HPP
#define _EXPRESSION_STATEMENT_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/ExpressionStatement.hpp>
#include "DummyExpression.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class ExpressionStatementTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ExpressionStatementTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testExpression);
    CPPUNIT_TEST (testExpressionReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (ExpressionStatement);

public:
    void setUp()
    {
        mExpr = new DummyExpression();
        mStmt = new ExpressionStatement(mExpr);
    }

    void tearDown()
    {
        delete mStmt;
    }

    void testVisit()
    {
        ExpressionStatementTestVisitor v;
        CPPUNIT_ASSERT (((bool) mStmt->visit(&v)) == true);
    }

    void testExpression()
    {
        CPPUNIT_ASSERT (mStmt->expression() == mExpr);
    }

    void testExpressionReplaceable()
    {
        DummyExpression expr2;
        mExpr->replaceWith(&expr2);
        CPPUNIT_ASSERT (mStmt->expression() == &expr2);
    }

private:
    ExpressionStatement* mStmt;
    DummyExpression* mExpr;
};

#endif
