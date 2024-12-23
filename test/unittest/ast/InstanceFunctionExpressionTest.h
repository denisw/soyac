/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_INSTANCE_FUNCTION_EXPRESSION_TEST_H_
#define SOYA_INSTANCE_FUNCTION_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/InstanceFunctionExpression.h>
#include <ast/Function.h>
#include <ast/FunctionType.h>
#include <ast/UnknownType.h>
#include <ast/VoidType.h>
#include "DummyExpression.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class InstanceFunctionExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (InstanceFunctionExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testInstance);
    CPPUNIT_TEST (testInstanceReplaceable);
    CPPUNIT_TEST (testTarget);
    CPPUNIT_TEST (testTargetReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (InstanceFunctionExpression);

public:
    void setUp()
    {
        mInstance = new DummyExpression;
        mTarget = new Function("foo", TYPE_VOID);
        mExpr = new InstanceFunctionExpression(mInstance, mTarget);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        InstanceFunctionExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == mTarget->type());
    }

    void testInstance()
    {
        CPPUNIT_ASSERT (mExpr->instance() == mInstance);
    }

    void testInstanceReplaceable()
    {
        DummyExpression instance2;
        mInstance->replaceWith(&instance2);
        CPPUNIT_ASSERT (mExpr->instance() == &instance2);
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
    Expression* mInstance;
    Function* mTarget;
    InstanceFunctionExpression* mExpr;
};

#endif
