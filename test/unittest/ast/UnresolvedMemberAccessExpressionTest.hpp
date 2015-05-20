/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_MEMBER_ACCESS_EXPRESSION_TEST_HPP
#define _UNRESOLVED_MEMBER_ACCESS_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedMemberAccessExpression.hpp>
#include "DummyExpression.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class UnresolvedMemberAccessExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedMemberAccessExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testOperand);
    CPPUNIT_TEST (testOperandReplaceable);
    CPPUNIT_TEST (testMemberName);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedMemberAccessExpression);

public:
    void setUp()
    {
        mOperand = new DummyExpression;
        mExpr = new UnresolvedMemberAccessExpression(mOperand, Name("foo"));
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        UnresolvedMemberAccessExpressionTestVisitor v;
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

    void testMemberName()
    {
        CPPUNIT_ASSERT (mExpr->memberName() == Name("foo"));
    }

private:
    UnresolvedMemberAccessExpression* mExpr;
    Expression* mOperand;
};

#endif
