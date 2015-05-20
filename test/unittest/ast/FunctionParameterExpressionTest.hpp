/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FUNCTION_PARAMETER_EXPRESSION_TEST_HPP
#define _FUNCTION_PARAMETER_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/FunctionParameterExpression.hpp>
#include <ast/Variable.hpp>
#include "DummyType.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class FunctionParameterExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (FunctionParameterExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testTarget);
    CPPUNIT_TEST (testTargetReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (FunctionParameterExpression);

public:
    void setUp()
    {
        mType = new DummyType;
        mTarget = new FunctionParameter(Name("foo"), mType);
        mExpr = new FunctionParameterExpression(mTarget);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        FunctionParameterExpressionTestVisitor v;
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
        FunctionParameter target2(Name("bar"), new DummyType);
        mTarget->replaceWith(&target2);
        CPPUNIT_ASSERT (mExpr->target() == &target2);
    }

private:
    FunctionParameterExpression* mExpr;
    FunctionParameter* mTarget;
    Type* mType;
};

#endif
