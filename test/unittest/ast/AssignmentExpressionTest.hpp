/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _ASSIGNMENT_EXPRESSION_TEST_HPP
#define _ASSIGNMENT_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/AssignmentExpression.hpp>
#include "DummyExpression.hpp"
#include "DummyType.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class AssignmentExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (AssignmentExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testLeftHand);
    CPPUNIT_TEST (testLeftHandReplaceable);
    CPPUNIT_TEST (testRightHand);
    CPPUNIT_TEST (testRightHandReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (AssignmentExpression);

public:
    void setUp()
    {
        mLeftHandType = new DummyType;
        mLeftHand = new DummyExpression(mLeftHandType);
        mRightHand = new DummyExpression;
        mExpr = new AssignmentExpression(mLeftHand, mRightHand);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        AssignmentExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == mLeftHandType);
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
    AssignmentExpression* mExpr;
    Expression* mLeftHand;
    Expression* mRightHand;
    Type* mLeftHandType;
};

#endif
