/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNRESOLVED_BINARY_EXPRESSION_TEST_H_
#define SOYA_UNRESOLVED_BINARY_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedBinaryExpression.h>
#include <ast/UnknownType.h>
#include "DummyExpression.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class UnresolvedBinaryExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedBinaryExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testKind);
    CPPUNIT_TEST (testLeftHand);
    CPPUNIT_TEST (testLeftHandReplaceable);
    CPPUNIT_TEST (testRightHand);
    CPPUNIT_TEST (testRightHandReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedBinaryExpression);

public:
    void setUp()
    {
        mLeftHand = new DummyExpression;
        mRightHand = new DummyExpression;
        mExpr = new UnresolvedBinaryExpression(UnresolvedBinaryExpression::PLUS,
                                               mLeftHand, mRightHand);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        UnresolvedBinaryExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == TYPE_UNKNOWN);
    }

    void testKind()
    {
        CPPUNIT_ASSERT (mExpr->kind() == UnresolvedBinaryExpression::PLUS);
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
    UnresolvedBinaryExpression* mExpr;
    Expression* mLeftHand;
    Expression* mRightHand;
};

#endif
