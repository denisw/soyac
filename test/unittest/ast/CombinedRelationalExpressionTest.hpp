/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _COMBINED_RELATIONAL_EXPRESSION_TEST_HPP
#define _COMBINED_RELATIONAL_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/CombinedRelationalExpression.hpp>
#include <ast/BooleanType.hpp>
#include "DummyDeclaredEntity.hpp"
#include "DummyExpression.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class CombinedRelationalExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (CombinedRelationalExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testLeftHand);
    CPPUNIT_TEST (testLeftHandReplaceable);
    CPPUNIT_TEST (testRightHand);
    CPPUNIT_TEST (testRightHandReplaceable);
    CPPUNIT_TEST (testEqualsCallee);
    CPPUNIT_TEST (testEqualsCalleeReplaceable);
    CPPUNIT_TEST (testComparisonCallee);
    CPPUNIT_TEST (testComparisonCalleeReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (CombinedRelationalExpression);

public:
    void setUp()
    {
        mLeftHand = new DummyExpression;
        mRightHand = new DummyExpression;
        mEqualsCallee = new DummyDeclaredEntity("equals");
        mComparisonCallee = new DummyDeclaredEntity("lessThan");

        mExpr = new CombinedRelationalExpression(mLeftHand,
                                                 mRightHand,
                                                 mEqualsCallee,
                                                 mComparisonCallee);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        CombinedRelationalExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == TYPE_BOOL);
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

    void testEqualsCallee()
    {
        CPPUNIT_ASSERT (mExpr->equalsCallee() == mEqualsCallee);
    }

    void testEqualsCalleeReplaceable()
    {
        DummyDeclaredEntity equalsCallee2;
        mEqualsCallee->replaceWith(&equalsCallee2);
        CPPUNIT_ASSERT (mExpr->equalsCallee() == &equalsCallee2);
    }

    void testComparisonCallee()
    {
        CPPUNIT_ASSERT (mExpr->comparisonCallee() == mComparisonCallee);
    }

    void testComparisonCalleeReplaceable()
    {
        DummyDeclaredEntity comparisonCallee2;
        mComparisonCallee->replaceWith(&comparisonCallee2);
        CPPUNIT_ASSERT (mExpr->comparisonCallee() == &comparisonCallee2);
    }

private:
    CombinedRelationalExpression* mExpr;
    Expression* mLeftHand;
    Expression* mRightHand;
    DeclaredEntity* mEqualsCallee;
    DeclaredEntity* mComparisonCallee;
};

#endif
