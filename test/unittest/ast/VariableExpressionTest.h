/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_VARIABLE_EXPRESSION_TEST_H_
#define SOYA_VARIABLE_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/VariableExpression.h>
#include <ast/Variable.h>
#include "DummyType.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class VariableExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (VariableExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testTarget);
    CPPUNIT_TEST (testTargetReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (VariableExpression);

public:
    void setUp()
    {
        mType = new DummyType;
        mTarget = new Variable(Name("foo"), mType);
        mExpr = new VariableExpression(mTarget);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        VariableExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == mType);
    }

    void testTarget()
    {
        CPPUNIT_ASSERT (mExpr->target() == mTarget);
    }

    void testTargetReplaceable()
    {
        Variable target2(Name("bar"), new DummyType);
        mTarget->replaceWith(&target2);
        CPPUNIT_ASSERT (mExpr->target() == &target2);
    }

private:
    VariableExpression* mExpr;
    Variable* mTarget;
    Type* mType;
};

#endif
