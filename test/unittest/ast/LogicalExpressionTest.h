/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_LOGICAL_EXPRESSION_TEST_H_
#define SOYA_LOGICAL_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/BooleanType.h>
#include <ast/LogicalExpression.h>
#include "DummyExpression.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class LogicalExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (LogicalExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testKind);
    CPPUNIT_TEST (testLeftHand);
    CPPUNIT_TEST (testLeftHandReplaceable);
    CPPUNIT_TEST (testRightHand);
    CPPUNIT_TEST (testRightHandReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (LogicalExpression);

public:
    void setUp()
    {
        mLeftHand = new DummyExpression;
        mRightHand = new DummyExpression;
        mExpr = new LogicalExpression(LogicalExpression::AND,
                                      mLeftHand, mRightHand);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        LogicalExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == TYPE_BOOL);
    }

    void testKind()
    {
        CPPUNIT_ASSERT (mExpr->kind() == LogicalExpression::AND);
    }

    void testLeftHand()
    {
        CPPUNIT_ASSERT (mExpr->leftHand() == mLeftHand);
    }

    void testLeftHandReplaceable()
    {
        DummyExpression leftHand2;
        mLeftHand->replaceWith(&leftHand2);
        CPPUNIT_ASSERT (mExpr->leftHand() == &leftHand2);
    }

    void testRightHand()
    {
        CPPUNIT_ASSERT (mExpr->rightHand() == mRightHand);
    }

    void testRightHandReplaceable()
    {
        DummyExpression rightHand2;
        mRightHand->replaceWith(&rightHand2);
        CPPUNIT_ASSERT (mExpr->rightHand() == &rightHand2);
    }

private:
    LogicalExpression* mExpr;
    Expression* mLeftHand;
    Expression* mRightHand;
};

#endif
