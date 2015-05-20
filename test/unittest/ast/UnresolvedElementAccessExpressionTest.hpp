/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_ELEMENT_ACCESS_EXPRESSION_TEST_HPP
#define _UNRESOLVED_ELEMENT_ACCESS_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedElementAccessExpression.hpp>
#include <ast/UnknownType.hpp>
#include "DummyExpression.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class UnresolvedElementAccessExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedElementAccessExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testOperand);
    CPPUNIT_TEST (testOperandReplaceable);
    CPPUNIT_TEST (testIndex);
    CPPUNIT_TEST (testIndexReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedElementAccessExpression);

public:
    void setUp()
    {
        mOperand = new DummyExpression;
        mIndex = new DummyExpression;
        mExpr = new UnresolvedElementAccessExpression(mOperand, mIndex);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        UnresolvedElementAccessExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == TYPE_UNKNOWN);
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

    void testIndex()
    {
        CPPUNIT_ASSERT (mExpr->index() == mIndex);
    }

    void testIndexReplaceable()
    {
        DummyExpression index2;
        mIndex->replaceWith(&index2);
        CPPUNIT_ASSERT (mExpr->index() == &index2);
    }

private:
    UnresolvedElementAccessExpression* mExpr;
    Expression* mOperand;
    Expression* mIndex;
};

#endif
