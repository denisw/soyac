/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_INSTANCE_PROPERTY_EXPRESSION_TEST_HPP
#define _UNRESOLVED_INSTANCE_PROPERTY_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedInstancePropertyExpression.hpp>
#include <ast/Property.hpp>
#include <ast/UnknownType.hpp>
#include "DummyExpression.hpp"
#include "DummyType.hpp"
#include "UnitTestUtil.hpp"

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
        mTarget = new Property("foo", new DummyType, NULL, NULL);
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
        Property target2("bar", new DummyType, NULL, NULL);
        mTarget->replaceWith(&target2);
        CPPUNIT_ASSERT (mExpr->target() == &target2);
    }

private:
    UnresolvedInstancePropertyExpression* mExpr;
    Expression* mInstance;
    Property* mTarget;
};

#endif
