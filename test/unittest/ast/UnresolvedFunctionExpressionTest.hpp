/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_FUNCTION_EXPRESSION_TEST_HPP
#define _UNRESOLVED_FUNCTION_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Function.hpp>
#include <ast/IntegerType.hpp>
#include <ast/BooleanType.hpp>
#include <ast/UnresolvedFunctionExpression.hpp>
#include <ast/UnknownType.hpp>
#include <ast/VoidType.hpp>
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class UnresolvedFunctionExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedFunctionExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testOverloads);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedFunctionExpression);

public:
    void setUp()
    {
        FunctionParameter* intParam = new FunctionParameter("i", TYPE_INT);
        FunctionParameter* boolParam = new FunctionParameter("b", TYPE_BOOL);

        mOverloads = new Function*[3];
        mOverloads[0] = new Function("foo", TYPE_VOID);
        mOverloads[1] = new Function("foo", TYPE_VOID,
                                     &intParam, &intParam + 1);
        mOverloads[2] = new Function("foo", TYPE_VOID,
                                     &boolParam, &boolParam + 1);

        mExpr = new UnresolvedFunctionExpression(mOverloads,
                                                 mOverloads + 3);
    }

    void tearDown()
    {
        delete mExpr;
        delete [] mOverloads;
    }

    void testVisit()
    {
        UnresolvedFunctionExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == TYPE_UNKNOWN);
    }

    void testOverloads()
    {
        UnresolvedFunctionExpression::overloads_iterator it =
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
    UnresolvedFunctionExpression* mExpr;
    Function** mOverloads;
};

#endif
