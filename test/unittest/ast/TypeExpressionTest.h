/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_TYPE_EXPRESSION_TEST_H_
#define SOYA_TYPE_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/TypeExpression.h>
#include <ast/UnknownType.h>
#include "DummyType.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class TypeExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (TypeExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testTarget);
    CPPUNIT_TEST (testTargetReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (TypeExpression);

public:
    void setUp()
    {
        mTarget = new DummyType;
        mExpr = new TypeExpression(mTarget);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        TypeExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == TYPE_UNKNOWN);
    }

    void testTarget()
    {
        CPPUNIT_ASSERT (mExpr->target() == mTarget);
    }

    void testTargetReplaceable()
    {
        DummyType target2;
        mTarget->replaceWith(&target2);
        CPPUNIT_ASSERT (mExpr->target() == &target2);
    }

private:
    TypeExpression* mExpr;
    Type* mTarget;
};

#endif
