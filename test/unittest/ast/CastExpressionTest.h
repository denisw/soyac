/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_CAST_EXPRESSION_TEST_H_
#define SOYA_CAST_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/CastExpression.h>
#include "DummyExpression.h"
#include "DummyType.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class CastExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (CastExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testTypeReplaceable);
    CPPUNIT_TEST (testOperand);
    CPPUNIT_TEST (testOperandReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (CastExpression);

public:
    void setUp()
    {
        mOperand = new DummyExpression;
        mType = new DummyType;
        mExpr = new CastExpression(mOperand, mType);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        CastExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == mType);
    }

    void testTypeReplaceable()
    {
        DummyType type2;
        mType->replaceWith(&type2);
        CPPUNIT_ASSERT (mExpr->type() == &type2);
    }

    void testOperand()
    {
        CPPUNIT_ASSERT (mExpr->operand() == mOperand);
    }

    void testOperandReplaceable()
    {
        DummyExpression operand2;
        mOperand->replaceWith(&operand2);
        CPPUNIT_ASSERT (mExpr->operand() == &operand2);
    }

private:
    CastExpression* mExpr;
    Expression* mOperand;
    Type* mType;
};

#endif
