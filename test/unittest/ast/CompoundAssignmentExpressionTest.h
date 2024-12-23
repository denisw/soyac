/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_COMPOUND_ASSIGNMENT_EXPRESSION_TEST_H_
#define SOYA_COMPOUND_ASSIGNMENT_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/CompoundAssignmentExpression.h>
#include <ast/BooleanType.h>
#include "DummyDeclaredEntity.h"
#include "DummyExpression.h"
#include "DummyType.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class CompoundAssignmentExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (CompoundAssignmentExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testLeftHand);
    CPPUNIT_TEST (testLeftHandReplaceable);
    CPPUNIT_TEST (testRightHand);
    CPPUNIT_TEST (testRightHandReplaceable);
    CPPUNIT_TEST (testCallee);
    CPPUNIT_TEST (testCalleeReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (CompoundAssignmentExpression);

public:
    void setUp()
    {
        mLeftHand = new DummyExpression(new DummyType);
        mRightHand = new DummyExpression;
        mCallee = new DummyDeclaredEntity("plus");

        mExpr =
          new CompoundAssignmentExpression(mLeftHand, mRightHand, mCallee);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        CompoundAssignmentExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == mLeftHand->type());
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

    void testCallee()
    {
        CPPUNIT_ASSERT (mExpr->callee() == mCallee);
    }

    void testCalleeReplaceable()
    {
        DummyDeclaredEntity callee2;
        mCallee->replaceWith(&callee2);
        CPPUNIT_ASSERT (mExpr->callee() == &callee2);
    }

private:
    CompoundAssignmentExpression* mExpr;
    Expression* mLeftHand;
    Expression* mRightHand;
    DeclaredEntity* mCallee;
};

#endif
