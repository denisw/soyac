/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _LOGICAL_NOT_EXPRESSION_TEST_HPP
#define _LOGICAL_NOT_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/BooleanType.hpp>
#include <ast/LogicalNotExpression.hpp>
#include "DummyExpression.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class LogicalNotExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (LogicalNotExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testOperand);
    CPPUNIT_TEST (testOperandReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (LogicalNotExpression);

public:
    void setUp()
    {
        mOperand = new DummyExpression;
        mExpr = new LogicalNotExpression(mOperand);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        LogicalNotExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == TYPE_BOOL);
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
    LogicalNotExpression* mExpr;
    Expression* mOperand;
};

#endif
