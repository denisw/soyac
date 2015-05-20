/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _CALL_EXPRESSION_TEST_HPP
#define _CALL_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/CallExpression.hpp>
#include <ast/Function.hpp>
#include <ast/FunctionExpression.hpp>
#include "DummyExpression.hpp"
#include "DummyType.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class CallExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (CallExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testCallee);
    CPPUNIT_TEST (testCalleeReplaceable);
    CPPUNIT_TEST (testArguments);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (CallExpression);

public:
    void setUp()
    {
        mReturnType = new DummyType;

        Function* func = new Function(Name("foo"), mReturnType);
        mCallee = new FunctionExpression(func);

        mArgs = new Expression*[3];
        mArgs[0] = new DummyExpression;
        mArgs[1] = new DummyExpression;
        mArgs[2] = new DummyExpression;

        mExpr = new CallExpression(mCallee, mArgs, mArgs + 3);
    }

    void tearDown()
    {
        delete mExpr;
        delete [] mArgs;
    }

    void testVisit()
    {
        CallExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == mReturnType);
    }

    void testCallee()
    {
        CPPUNIT_ASSERT (mExpr->callee() == mCallee);
    }

    void testCalleeReplaceable()
    {
        DummyExpression callee2;
        mCallee->replaceWith(&callee2);
        CPPUNIT_ASSERT (mExpr->callee() == &callee2);
    }

    void testArguments()
    {
        CallExpression::arguments_iterator it = mExpr->arguments_begin();

        CPPUNIT_ASSERT (*it == mArgs[0]);
        it++;
        CPPUNIT_ASSERT (*it == mArgs[1]);
        it++;
        CPPUNIT_ASSERT (*it == mArgs[2]);
        it++;
        CPPUNIT_ASSERT (it == mExpr->arguments_end());
    }

private:
    CallExpression* mExpr;
    Expression* mCallee;
    Expression** mArgs;
    Type* mReturnType;
};

#endif
