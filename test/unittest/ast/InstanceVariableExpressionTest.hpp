/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _INSTANCE_VARIABLE_EXPRESSION_TEST_HPP
#define _INSTANCE_VARIABLE_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/InstanceVariableExpression.hpp>
#include <ast/Variable.hpp>
#include <ast/UnknownType.hpp>
#include "DummyExpression.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class InstanceVariableExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (InstanceVariableExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testInstance);
    CPPUNIT_TEST (testInstanceReplaceable);
    CPPUNIT_TEST (testTarget);
    CPPUNIT_TEST (testTargetReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (InstanceVariableExpression);

public:
    void setUp()
    {
        mInstance = new DummyExpression;
        mTarget = new Variable("foo", new DummyType);
        mExpr = new InstanceVariableExpression(mInstance, mTarget);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        InstanceVariableExpressionTestVisitor v;
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
        Variable target2("bar", new DummyType);
        mTarget->replaceWith(&target2);
        CPPUNIT_ASSERT (mExpr->target() == &target2);
    }

private:
    Expression* mInstance;
    Variable* mTarget;
    InstanceVariableExpression* mExpr;
};

#endif
