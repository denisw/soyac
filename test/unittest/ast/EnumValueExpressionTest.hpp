/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _ENUM_VALUE_EXPRESSION_TEST_HPP
#define _ENUM_VALUE_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/EnumValueExpression.hpp>
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class EnumValueExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (EnumValueExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testTarget);
    CPPUNIT_TEST (testTargetReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (EnumValueExpression);

public:
    void setUp()
    {
        mTarget = new EnumConstant("BAR", IntegerValue(0, false));
        mType = new EnumType("Foo", TYPE_INT, &mTarget, &mTarget + 1);

        mExpr = new EnumValueExpression(mType, mTarget);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        EnumValueExpressionTestVisitor v;
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
        EnumConstant target2("BAZ", IntegerValue(1, false));
        mTarget->replaceWith(&target2);
        CPPUNIT_ASSERT (mExpr->target() == &target2);
    }

private:
    EnumValueExpression* mExpr;
    EnumType* mType;
    EnumConstant* mTarget;
};

#endif
