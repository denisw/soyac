/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNRESOLVED_INSTANCE_PROPERTY_EXPRESSION_TEST_H_
#define SOYA_UNRESOLVED_INSTANCE_PROPERTY_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedInstancePropertyExpression.h>
#include <ast/Property.h>
#include <ast/UnknownType.h>
#include "DummyExpression.h"
#include "DummyType.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class UnresolvedInstancePropertyExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedInstancePropertyExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testInstance);
    CPPUNIT_TEST (testInstanceReplaceable);
    CPPUNIT_TEST (testTarget);
    CPPUNIT_TEST (testTargetReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedInstancePropertyExpression);

public:
    void setUp()
    {
        mInstance = new DummyExpression;
        mTarget = new Property("foo", new DummyType, nullptr, nullptr);
        mExpr = new UnresolvedInstancePropertyExpression(mInstance, mTarget);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        UnresolvedInstancePropertyExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == TYPE_UNKNOWN);
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
        Property target2("bar", new DummyType, nullptr, nullptr);
        mTarget->replaceWith(&target2);
        CPPUNIT_ASSERT (mExpr->target() == &target2);
    }

private:
    UnresolvedInstancePropertyExpression* mExpr;
    Expression* mInstance;
    Property* mTarget;
};

#endif
