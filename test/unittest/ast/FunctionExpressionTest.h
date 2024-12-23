/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_FUNCTION_EXPRESSION_TEST_H_
#define SOYA_FUNCTION_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/FunctionExpression.h>
#include <ast/Function.h>
#include <ast/FunctionType.h>
#include <ast/UnknownType.h>
#include <ast/VoidType.h>
#include "UnitTestUtil.h"

using namespace soyac::ast;

class FunctionExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (FunctionExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testTarget);
    CPPUNIT_TEST (testTargetReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (FunctionExpression);

public:
    void setUp()
    {
        mTarget = new Function("foo", TYPE_VOID);
        mExpr = new FunctionExpression(mTarget);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        FunctionExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == mTarget->type());
    }

    void testTarget()
    {
        CPPUNIT_ASSERT (mExpr->target() == mTarget);
    }

    void testTargetReplaceable()
    {
        Function target2("bar", TYPE_VOID);
        mTarget->replaceWith(&target2);
        CPPUNIT_ASSERT (mExpr->target() == &target2);
    }

private:
    Function* mTarget;
    FunctionExpression* mExpr;
};

#endif
