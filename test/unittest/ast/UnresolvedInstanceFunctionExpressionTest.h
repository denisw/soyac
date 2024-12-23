/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNRESOLVED_METHOD_ACCESS_EXPRESSION_TEST_H_
#define SOYA_UNRESOLVED_METHOD_ACCESS_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Function.h>
#include <ast/IntegerType.h>
#include <ast/BooleanType.h>
#include <ast/UnresolvedInstanceFunctionExpression.h>
#include <ast/UnknownType.h>
#include "DummyExpression.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class UnresolvedInstanceFunctionExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedInstanceFunctionExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testInstance);
    CPPUNIT_TEST (testInstanceReplaceable);
    CPPUNIT_TEST (testOverloads);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedInstanceFunctionExpression);

public:
    void setUp()
    {
        mInstance = new DummyExpression;

        FunctionParameter* intParam = new FunctionParameter("i", TYPE_INT);
        FunctionParameter* boolParam = new FunctionParameter("b", TYPE_BOOL);

        mOverloads = new Function*[3];
        mOverloads[0] = new Function("foo", TYPE_VOID);
        mOverloads[1] = new Function("foo", TYPE_VOID,
                                     &intParam, &intParam + 1);
        mOverloads[2] = new Function("foo", TYPE_VOID,
                                     &boolParam, &boolParam + 1);

        mExpr = new UnresolvedInstanceFunctionExpression(mInstance,
                                                         mOverloads,
                                                         mOverloads + 3);
    }

    void tearDown()
    {
        delete mExpr;
        delete [] mOverloads;
    }

    void testVisit()
    {
        UnresolvedInstanceFunctionExpressionTestVisitor v;
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

    void testOverloads()
    {
        UnresolvedInstanceFunctionExpression::overloads_iterator it =
          mExpr->overloads_begin();

        CPPUNIT_ASSERT (*it == mOverloads[0]);
        it++;
        CPPUNIT_ASSERT (*it == mOverloads[1]);
        it++;
        CPPUNIT_ASSERT (*it == mOverloads[2]);
        it++;
        CPPUNIT_ASSERT (it == mExpr->overloads_end());
    }

private:
    UnresolvedInstanceFunctionExpression* mExpr;
    Expression* mInstance;
    Function** mOverloads;
};

#endif
